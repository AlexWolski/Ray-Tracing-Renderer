#include "rtRenderer.h"

namespace rtGraphics
{
	rtRenderer::rtRenderer()
	{
		threadPool = make_unique<rtRenderThreadPool>();
	}

	void rtRenderer::waitForRender()
	{
		threadPool->joinThreads();
	}

	//Ray trace an entire scene
	void rtRenderer::rayTraceScene(shared_ptr<rtScene> scene, rtVec3f& camPos, rtVec3f& u, rtVec3f& v, rtVec3f& n,
		float hFov, float nearClip, float farClip, int maxBounces, ofPixels* bufferPixels)
	{
		//Wait for any currently running threads to finish first
		threadPool->joinThreads();
		//Set the render settings and start the threads
		threadPool->setData(scene, camPos, u, v, n, hFov, nearClip, farClip, maxBounces, bufferPixels);
		threadPool->startThreads();
	}

	//Ray trace a single ray and return the color at the intersection
	rtColorf rtRenderer::rayTrace(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, int currBounce, int maxBounces, shared_ptr<rtRayHit> originPoint)
	{
		//The intersection data of the closest intersection
		shared_ptr<rtRayHit> hitData = rayTrace(objects, P, D, nearClip, farClip, originPoint);

		//If the ray didn't intersect any objects, return a black pixel
		//TO-DO: Return the background color of the camera
		if (!hitData->hit)
			return rtColorf::black;
		//Otherwise calculate the lighting of the pixel
		else
		{
			//The final color to the drawn to the pixel
			rtColorf finalColor = rtColorf();
			//The material of the object
			rtMat objectMat = hitData->hitObject->getMat();
			//Get the reflectivity of the material
			float reflectivity = hitData->hitObject->getMat().getReflectivity();

			//The ambient and diffuse colors of the object
			rtColorf objectColor;
			//Store the specular color separate from the ambient and diffuse
			rtColorf specular;

			//Iterate over the all the lights
			for (auto lightPtr = lights->begin(); lightPtr != lights->end(); lightPtr++)
			{
				//Get a pointer to the current light
				rtLight* currLight = lightPtr->second;
				//Cache the ambient and light intensity
				float incidentIntensity = currLight->getIncidentIntensity();
				float ambientIntensity = currLight->getAmbientIntensity();

				//Calculate the vector pointing from the hit position towards the light
				rtVec3f lightVector = (currLight->getPosition() - hitData->hitPoint);
				//Get the squared distance from the hit position to the light before normalizing it
				float lightDistSquared = lightVector.magnitudeSquared();
				lightVector.normalize();
				
				//Determine if the current light hits the point or not
				bool shadow = isShadow(objects, lightVector, hitData->hitPoint, lightDistSquared, nearClip, farClip, hitData);

				//Add the ambient color if the object is not perfectly reflective, regardless of if the point is in shadow or not.
				if (reflectivity < 1.0f)
					objectColor += PhongShader::ambientColor((*currLight).getAmbient(), objectMat.getAmbient(), ambientIntensity);

				//If the point is not in shadow, check for specular and diffuse color as well
				if (!shadow)
				{
					//If the object is not perfectly reflective, calculate the ambient and diffuse colors
					if (reflectivity < 1.0f)
						objectColor += PhongShader::diffuseColor(lightVector, hitData->hitNormal, (*currLight).getDiffuse(), objectMat.getDiffuse(), incidentIntensity);

					//Calculate the specular color regardless of the reflectivity
					specular += PhongShader::specularColor(lightVector, D, hitData->hitNormal, (*currLight).getSpecular(), objectMat.getSpecular(), objectMat.getSmoothness(), incidentIntensity);
				}
			}

			//If the object is perfectly non-reflective, only use the object color
			if (reflectivity == 0.0f)
				finalColor = objectColor + specular;
			//If the object has some reflectivity, incorporate the reflected color
			else
			{
				//Bounce the ray off of the object and calculate the reflected color
				rtColorf reflectedColor = bounceRay(objects, lights, P, D, nearClip, farClip, currBounce, maxBounces, hitData);
				//Combine the object color and reflected color
				finalColor = (objectColor * (1 - reflectivity)) + (reflectedColor * reflectivity) + specular;
			}

			//Clamp the values of the final color and return it
			finalColor.clampColors();
			return finalColor;
		}
	}

	//Ray trace a single ray and return the ray hit data
	shared_ptr<rtRayHit> rtRenderer::rayTrace(objectSet& objects, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint)
	{
		//The intersection data of the closest intersection
		shared_ptr<rtRayHit> nearestHit = make_shared<rtRayHit>();
		//Set the hit data to a miss by default.
		nearestHit->hit = false;
		nearestHit->distance = INFINITY;

		//Iterate over the all the objects
		for (auto objectPtr = objects->begin(); objectPtr != objects->end(); objectPtr++)
		{
			//Get a pointer to the current object
			rtObject* currObject = objectPtr->second;

			//Determine if the ray intersects the object
			shared_ptr<rtRayHit> hitData = currObject->rayIntersect(P, D, nearClip, farClip, originPoint);

			//If the ray hit and the object is not obscured, save hit data
			if (hitData->hit && hitData->distance < nearestHit->distance)
				nearestHit = hitData;
		}

		//Return the hit data
		return nearestHit;
	}

	//Bounce a ray off of the object it hits and find the reflected color
	rtColorf rtRenderer::bounceRay(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D,
		float nearClip, float farClip, int currBounce, int maxBounces, shared_ptr<rtRayHit> hitData)
	{
		//If the ray has already bounced too many times return black
		if (currBounce >= maxBounces)
			return rtColorf::black;

		//Get the reflected ray
		rtVec3f reflectedRay = D.getReflected(hitData->hitNormal);
		//Trace the bounced ray to get the reflected color. The near clip is set to 0 since it is not needed
		return rayTrace(objects, lights, hitData->hitPoint, reflectedRay, 0.0f, farClip, ++currBounce, maxBounces, hitData);
	}

	//Determine if a given light shines on a point or is occluded
	bool rtRenderer::isShadow(objectSet& objects, rtVec3f& lightVector, rtVec3f& targetPoint, float lightDistSquared, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint)
	{
		//Cast a ray from the hit point towards the light source to check if the light is occluded
		shared_ptr<rtRayHit> shadowRay = rayTrace(objects, targetPoint, lightVector, nearClip, farClip, originPoint);

		//If they ray doesn't hit anything, return false
		if (!shadowRay->hit)
			return false;

		//Calculate the squared distance from the hit point to the nearest object
		float hitDist = shadowRay->distance;
		float hitDistSquared = hitDist * hitDist;

		//If the ray hits an object before the light, then the point is in shadow
		if (hitDistSquared < lightDistSquared)
			return true;

		//Otherwise the point is not in shadow
		return false;
	}
}