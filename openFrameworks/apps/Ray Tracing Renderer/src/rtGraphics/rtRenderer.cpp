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


	///Helper methods
	//Given a hit point, shade the point using the Phong shading method
	rtColorf rtRenderer::calcPixelColor(renderMode RenderMode, objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D,
		float nearClip, float farClip, int currBounce, int maxBounces, rtRayHit hitData)
	{
		//If the ray didn't intersect any objects, return a black pixel
				//TO-DO: Return the background color of the camera
		if (!hitData.hit)
			return rtColorf::black;
		//Otherwise calculate the lighting of the pixel
		else
		{
			//The final color to the drawn to the pixel
			rtColorf finalColor = rtColorf();
			//The material of the object
			rtMat objectMat = hitData.hitObject->getMat();
			//Get the reflectivity of the material
			float reflectivity = hitData.hitObject->getMat().getReflectivity();

			//The ambient and diffuse colors of the object
			rtColorf objectColor;
			//Store the specular color separate from the ambient and diffuse
			rtColorf specular;

			//Iterate over the all the lights
			for (int lightIndex = 0; lightIndex < lights->size(); lightIndex++)
			{
				//Get a pointer to the current light
				rtLight* currLight = lights->at(lightIndex);
				//Cache the ambient and light intensity
				float incidentIntensity = currLight->getIncidentIntensity();
				float ambientIntensity = currLight->getAmbientIntensity();

				//Calculate the vector pointing from the hit position towards the light
				rtVec3f lightVector = (currLight->getPosition() - hitData.hitPoint);
				//Get the squared distance from the hit position to the light before normalizing it
				float lightDistSquared = lightVector.magnitudeSquared();
				lightVector.normalize();

				//Determine if the current light hits the point or not
				bool shadow = isShadow(RenderMode, objects, lightVector, hitData.hitPoint, lightDistSquared, nearClip, farClip, hitData);

				//Add the ambient color if the object is not perfectly reflective, regardless of if the point is in shadow or not.
				if (reflectivity < 1.0f)
					objectColor += PhongShader::ambientColor((*currLight).getAmbient(), objectMat.getAmbient(), ambientIntensity);

				//If the point is not in shadow, check for specular and diffuse color as well
				if (!shadow)
				{
					//If the object is not perfectly reflective, calculate the ambient and diffuse colors
					if (reflectivity < 1.0f)
						objectColor += PhongShader::diffuseColor(lightVector, hitData.hitNormal, (*currLight).getDiffuse(), objectMat.getDiffuse(), incidentIntensity);

					//Calculate the specular color regardless of the reflectivity
					specular += PhongShader::specularColor(lightVector, D, hitData.hitNormal, (*currLight).getSpecular(), objectMat.getSpecular(), objectMat.getSmoothness(), incidentIntensity);
				}
			}

			//If the object is perfectly non-reflective, only use the object color
			if (reflectivity == 0.0f)
				finalColor = objectColor + specular;
			//If the object has some reflectivity, incorporate the reflected color
			else
			{
				//Bounce the ray off of the object and calculate the reflected color
				rtColorf reflectedColor = bounceRay(RenderMode, objects, lights, P, D, nearClip, farClip, currBounce, maxBounces, hitData);
				//Combine the object color and reflected color
				finalColor = (objectColor * (1 - reflectivity)) + (reflectedColor * reflectivity) + specular;
			}

			//Clamp the values of the final color and return it
			finalColor.clampColors();
			return finalColor;
		}
	}

	//Bounce a ray off of the object it hits and find the reflected color
	rtColorf rtRenderer::bounceRay(renderMode RenderMode, objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D,
		float nearClip, float farClip, int currBounce, int maxBounces, rtRayHit hitData)
	{
		//If the ray has already bounced too many times return black
		if (currBounce >= maxBounces)
			return rtColorf::black;

		//Get the reflected ray
		rtVec3f reflectedRay = D.getReflected(hitData.hitNormal);

		rtColorf color;

		//Calculate the pixel color based on the rendering mode
		switch (RenderMode)
		{
		case renderMode::rayTrace:
			color = rayTrace(objects, lights, hitData.hitPoint, reflectedRay, 0.0f, farClip, ++currBounce, maxBounces, hitData);
			break;
		case renderMode::rayMarch:
			color = rayMarch(objects, lights, hitData.hitPoint, reflectedRay, 0.0f, farClip, ++currBounce, maxBounces, hitData);
			break;
		default:
			//If no rendering mode was specified, return black
			return rtColorf::black;
		}

		return color;
	}

	//Determine if a given light shines on a point or is occluded
	bool rtRenderer::isShadow(renderMode RenderMode, objectSet& objects, rtVec3f& lightVector, rtVec3f& targetPoint, float lightDistSquared, float nearClip, float farClip, rtRayHit originPoint)
	{
		//Cast a ray from the hit point towards the light source to check if the light is occluded
		rtRayHit shadowRay;

		//Compute if the point is in shadow using the current rendering mode
		switch (RenderMode)
		{
		case renderMode::rayTrace:
			shadowRay = rayTrace(objects, targetPoint, lightVector, nearClip, farClip, originPoint);
			break;
		case renderMode::rayMarch:
			shadowRay = rayMarch(objects, targetPoint, lightVector, nearClip, farClip, originPoint);
			break;
		default:
			//If no rendering mode was specified, exit the method
			return false;
		}

		//If they ray doesn't hit anything, return false
		if (!shadowRay.hit)
			return false;

		//Calculate the squared distance from the hit point to the nearest object
		float hitDist = shadowRay.distance;
		float hitDistSquared = hitDist * hitDist;

		//If the ray hits an object before the light, then the point is in shadow
		if (hitDistSquared < lightDistSquared)
			return true;

		//Otherwise the point is not in shadow
		return false;
	}


	///Ray tracing methods
	//Ray trace a single ray and return the color at the intersection
	rtColorf rtRenderer::rayTrace(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, int currBounce, int maxBounces, rtRayHit originPoint)
	{
		//Find the closest object the ray hits
		rtRayHit hitData = rayTrace(objects, P, D, nearClip, farClip, originPoint);
		//Calculate the color of that point
		return calcPixelColor(renderMode::rayTrace, objects, lights, P, D, nearClip, farClip, currBounce, maxBounces, hitData);
	}

	//Ray trace a single ray and return the ray hit data
	rtRayHit rtRenderer::rayTrace(objectSet& objects, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, rtRayHit originPoint)
	{
		//The intersection data of the closest intersection
		rtRayHit nearestHit;
		//Set the hit data to a miss by default.
		nearestHit.hit = false;
		nearestHit.distance = INFINITY;

		//Iterate over the all the objects
		for (int objectIndex = 0; objectIndex < objects->size(); objectIndex++)
		{
			//Get a pointer to the current object
			rtObject* currObject = objects->at(objectIndex);

			//Determine if the ray intersects the object
			rtRayHit hitData = currObject->rayIntersect(P, D, nearClip, farClip, originPoint);

			//If the ray hit and the object is not obscured, save hit data
			if (hitData.hit && hitData.distance < nearestHit.distance)
				nearestHit = hitData;
		}

		//Return the hit data
		return nearestHit;
	}


	///Ray marching settings
	int rtRenderer::maxIters = 100;
	float rtRenderer::minHitDist = 0.01f;
	float rtRenderer::normalEps = 0.01f;

	///Ray marching methods
	//Ray trace a single ray and return the color at the intersection
	rtColorf rtRenderer::rayMarch(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, int currBounce, int maxBounces, rtRayHit originPoint)
	{
		//Find the closest object the ray hits
		rtRayHit hitData = rayMarch(objects, P, D, nearClip, farClip, originPoint);
		//Calculate the color of that point
		return calcPixelColor(renderMode::rayMarch, objects, lights, P, D, nearClip, farClip, currBounce, maxBounces, hitData);
	}

	//Ray trace a single ray and return the ray hit data
	rtRayHit rtRenderer::rayMarch(objectSet& objects, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, rtRayHit originPoint)
	{
		//The intersection data of the closest intersection
		rtRayHit hitData;
		//Make a copy of the ray to march forward
		rtVec3f marchedRay = P;

		//March the ray forward until it hits an object or the maximum number of iterations is reached
		for (int iteration = 0; iteration < maxIters; iteration++)
		{
			//The nearest distance between the ray and any of the objects
			float nearestDist = INFINITY;

			//Iterate over the all the objects
			for (int objectIndex = 0; objectIndex < objects->size(); objectIndex++)
			{
				//Get a pointer to the current object
				rtObject* currObject = objects->at(objectIndex);

				//Determine the distance from the ray to the current object
				hitData = currObject->sdf(marchedRay);

				//Check if the object is within the view frustum
				if (hitData.distance < farClip)
				{
					//If the ray hits the object, return the hit data
					if (abs(hitData.distance) <= minHitDist)
					{
						//If this is the first iteration and the object is too close, skip the object
						if (iteration == 0 && hitData.distance < nearClip)
							continue;

						//Store the hit data
						hitData.hit = true;
						hitData.hitPoint = marchedRay;
						//Calculate the normal
						updateNormalRM(hitData);
						
						return hitData;
					}

					//Otherwise if the object is the nearest so far, save the distance
					if (hitData.distance < nearestDist)
						nearestDist = hitData.distance;
				}
			}

			//If the ray didn't hit any objects, march it forward by the distance to the nearest object
			marchedRay += (D * nearestDist);
		}

		//If the ray didn't hit any objects within the maximum iterations, the ray missed
		hitData.hit = false;
		return hitData;
	}

	//Update the normal of an rtRayHit struct
	void rtRenderer::updateNormalRM(rtRayHit hitData)
	{
		float dp = hitData.distance;

		//Compute three points to sample the distance to the object
		rtVec3f hitPoint = hitData.hitPoint;
		rtVec3f samplePoint1(hitPoint.getX() - normalEps, hitPoint.getY(), hitPoint.getZ());
		rtVec3f samplePoint2(hitPoint.getX(), hitPoint.getY() - normalEps, hitPoint.getZ());
		rtVec3f samplePoint3(hitPoint.getX(), hitPoint.getY(), hitPoint.getZ() - normalEps);

		//Compute the components of the estimated normal
		float normX = dp - (hitData.hitObject->sdf(samplePoint1)).distance;
		float normY = dp - (hitData.hitObject->sdf(samplePoint2)).distance;
		float normZ = dp - (hitData.hitObject->sdf(samplePoint3)).distance;

		//Normalize the vector and store it in the rtRayHit struct
		hitData.hitNormal = rtVec3f(normX, normY, normZ);
		hitData.hitNormal.normalize();
	}
}