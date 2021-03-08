#include "rtRenderer.h"

namespace rtGraphics
{
	rtRenderer::rtRenderer()
	{
		threadPool = make_unique<rtRenderThreadPool>();
	}

	void rtRenderer::render(renderMode RenderMode, shared_ptr<rtScene> scene, rtVec3f& camPos, rtVec3f& u, rtVec3f& v, rtVec3f& n,
		float hFov, float nearClip, float farClip, int maxBounces, ofPixels* bufferPixels)
	{
		//Wait for any currently running threads to finish first
		threadPool->joinThreads();
		//Set the render settings and start the threads
		threadPool->setData(RenderMode, scene, camPos, u, v, n, hFov, nearClip, farClip, maxBounces, bufferPixels);
		threadPool->startThreads();
	}

	void rtRenderer::waitForRender()
	{
		threadPool->joinThreads();
	}


	///Ray tracing methods
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
				bool shadow = isShadowTraced(objects, lightVector, hitData->hitPoint, lightDistSquared, nearClip, farClip, hitData);

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
	bool rtRenderer::isShadowTraced(objectSet& objects, rtVec3f& lightVector, rtVec3f& targetPoint, float lightDistSquared, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint)
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


	///Ray marching settings
	int rtRenderer::maxIters = 100;
	float rtRenderer::minHitDist = 0.01f;
	float rtRenderer::normalEps = 0.01f;

	///Ray marching methods
	//Ray trace a single ray and return the color at the intersection
	rtColorf rtRenderer::rayMarch(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, int currBounce, int maxBounces, shared_ptr<rtRayHit> originPoint)
	{
		//The intersection data of the closest intersection
		shared_ptr<rtRayHit> hitObject = rayMarch(objects, P, D, nearClip, farClip, originPoint);

		//If the ray didn't hit any objects, return a black pixel
		//TO-DO: Return the background color of the camera
		if (hitObject->hit)
			return rtColorf::green;
		//Otherwise calculate the lighting of the pixel
		else
			return rtColorf::black;
	}

	//Ray trace a single ray and return the ray hit data
	shared_ptr<rtRayHit> rtRenderer::rayMarch(objectSet& objects, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint)
	{
		//The intersection data of the closest intersection
		shared_ptr<rtRayHit> rayDist;
		//Make a copy of the ray to march forward
		rtVec3f marchedRay = P;

		//March the ray forward until it hits an object or the maximum number of iterations is reached
		for (int iteration = 0; iteration < maxIters; iteration++)
		{
			//The nearest distance between the ray and any of the objects
			float nearestDist = INFINITY;

			//Iterate over the all the objects
			for (auto objectPtr = objects->begin(); objectPtr != objects->end(); objectPtr++)
			{
				//Get a pointer to the current object
				rtObject* currObject = objectPtr->second;

				//Determine the distance from the ray to the current object
				rayDist = currObject->sdf(marchedRay);

				//Check if the object is within the view frustum
				if (rayDist->distance < farClip)
				{
					//If the ray hits the object, return the hit data
					if (abs(rayDist->distance) <= minHitDist)
					{
						//If this is the first iteration and the object is too close, skip the object
						if (iteration == 0 && rayDist->distance < nearClip)
							continue;

						rayDist->hit = true;
						return rayDist;
					}

					//Otherwise if the object is the nearest so far, save the distance
					if (rayDist->distance < nearestDist)
						nearestDist = rayDist->distance;
				}
			}

			//If the ray didn't hit any objects, march it forward by the distance to the nearest object
			marchedRay += (D * nearestDist);
		}

		//If the ray didn't hit any objects within the maximum iterations, the ray missed
		rayDist->hit = false;
		return rayDist;
	}
}