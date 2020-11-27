#include "rtRenderer.h"

namespace rtGraphics
{
	///rtRenderer static members
	int rtRenderer::numThreads;
	rtRenderer::rayTraceThread* rtRenderer::threadPool = rtRenderer::makeThreads();

	///rtRenderer Methods
	//Create an array of threads and return it
	rtRenderer::rayTraceThread* rtRenderer::makeThreads()
	{
		//Set the default number of threads to 8
		numThreads = 8;
		//Return an array of threads
		return new rayTraceThread[numThreads];
	}

	//Ray trace an entire scene
	void rtRenderer::rayTraceScene(shared_ptr<rtScene> scene, rtVec3f& camPos, rtVec3f& u, rtVec3f& v, rtVec3f& n,
		float hFov, float nearClip, float farClip, int maxBounces, ofPixels* bufferPixels)
	{
		//Cache the pixel buffer dimensions as floats
		float bufferWidth = bufferPixels->getWidth();
		float bufferHeight = bufferPixels->getHeight();

		//The width and height of the near clip plane based on the FOV and distance to the clip plane
		float halfClipWidth = sin(degToRad(hFov / 2)) * nearClip;
		float halfClipHeight = halfClipWidth * (bufferHeight / bufferWidth);

		//Calculate the point in the center of the near clip plane using the camera position and look vector
		rtVec3f clipCenter = camPos + (-n * nearClip);
		//Get the vectors pointing from the center of the screen to the left edge and top edge of the near clip plane
		rtVec3f widthVector = u * halfClipWidth;
		rtVec3f heightVector = v * halfClipHeight;
		//The distance between each grid point of the near clip plane in world space
		rtVec3f hStep = (widthVector * -2) / bufferWidth;
		rtVec3f vStep = (heightVector * -2) / bufferHeight;

		//The first grid point, at the top-left corner
		rtVec3f firstPoint = clipCenter + widthVector + heightVector;

		//Set the shared data of the threads
		rayTraceThread::setData(scene, camPos, nearClip, farClip, maxBounces, bufferPixels, firstPoint, hStep, vStep);

		//The minimum number of rows each thread will render
		int baseRows = bufferHeight / numThreads;
		//The number of threads that will render an additional row
		int extraRows = bufferHeight - (numThreads * baseRows);
		//The starting row of the next section
		int sectionStart = 0;

		//Spawn threads to render all of the image
		for (int threadIndex = 0; threadIndex < numThreads; threadIndex++)
		{
			//Calculate the end of the section
			int sectionEnd = sectionStart + baseRows;

			//Add an extra row until all the extra rows are allocated
			if (threadIndex < extraRows)
				sectionEnd++;

			//Set the section to render and start the thread
			threadPool[threadIndex].setSection(sectionStart, sectionEnd);
			threadPool[threadIndex].startThread();

			//Move onto the next section
			sectionStart = sectionEnd;
		}

		//Join all the threads
		for (int threadIndex = 0; threadIndex < numThreads; threadIndex++)
			threadPool[threadIndex].waitForThread();
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
				float intensity = lightIntensity(objects, currLight, nearClip, farClip, hitData);

				//Add the ambient color if the object is not perfectly reflective, regardless of if the point is in shadow or not.
				if (reflectivity < 1.0f)
					objectColor += ambientColor((*currLight).getAmbient(), objectMat.getAmbient(), ambientIntensity);

				//If the point is not in shadow, check for specular and diffuse color as well
				if (intensity != 0.0f)
				{
					//If the object is not perfectly reflective, calculate the ambient and diffuse colors
					if (reflectivity < 1.0f)
						objectColor += diffuseColor(lightVector, hitData->hitNormal, (*currLight).getDiffuse(), objectMat.getDiffuse(), incidentIntensity) * intensity;

					//Calculate the specular color regardless of the reflectivity
					specular += specularColor(lightVector, D, hitData->hitNormal, (*currLight).getSpecular(), objectMat.getSpecular(), objectMat.getSmoothness(), incidentIntensity) * intensity;
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
	float rtRenderer::lightIntensity(objectSet& objects, rtLight* light, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint)
	{
		//The number of rays to cast towards the sphere light
		static int lightSamples = 10;
		//The radius of the light sources
		static int lightRadius = 5.0f;

		//The number of rays that reach the light source
		float numLightRays = 0.0f;

		//The point that we are calculating the lighting of
		rtVec3f rayOrigin = originPoint->hitPoint;

		for (int sample = 0; sample < lightSamples; sample++)
		{
			//Generate random floats for calculating the spherical coordinates
			float u = randf(0.0f, 1.0f);
			float v = randf(0.0f, 1.0f);

			//Calculate the spherical coordinates
			float q = 2 * PIf * u;
			float f = acos((2 * v) - 1);

			//Calculate the Cartesian coordinates
			rtVec3f lightPoint = rtVec3f();
			lightPoint.setX(lightRadius * cos(q) * sin(f));
			lightPoint.setY(lightRadius * sin(q) * sin(f));
			lightPoint.setZ(lightRadius * cos(f));

			//The random point in world space
			rtVec3f targetPoint = light->getPosition() + lightPoint;

			//Calculate the vector pointing from the hit position towards the light
			rtVec3f lightVector = (targetPoint - rayOrigin);
			//Get the squared distance from the hit position to the random point before normalizing it
			float lightDistSquared = lightVector.magnitudeSquared();
			lightVector.normalize();

			//Cast a ray from the hit point towards the light source to check if the light is occluded
			shared_ptr<rtRayHit> lightRay = rayTrace(objects, rayOrigin, lightVector, nearClip, farClip, originPoint);

			//If they ray doesn't hit any objects, then it reached the light source
			if (!lightRay->hit)
				numLightRays++;
			//Otherwise, determine if it hit the object or the light source first
			else
			{
				//Calculate the squared distance from the hit point to the nearest object
				float hitDist = lightRay->distance;
				float hitDistSquared = hitDist * hitDist;

				//If the ray hits the light before the object, then it reached the light
				if (lightDistSquared <= hitDistSquared)
					numLightRays++;
			}
		}

		//Return the ratio of light rays that hit to total light rays
		return (numLightRays / lightSamples);
	}

	//A thread-safe function for generating random numbers
	float rtRenderer::randf(float min, float max)
	{
		thread_local static std::random_device rd;
		thread_local static std::mt19937 rng(rd());
		thread_local std::uniform_real_distribution<float> urd;
		return urd(rng, decltype(urd)::param_type{ min, max });
	}


	///rayTraceThread static members
	objectSet rtRenderer::rayTraceThread::objects;
	lightSet rtRenderer::rayTraceThread::lights;
	rtVec3f rtRenderer::rayTraceThread::camPos;
	float rtRenderer::rayTraceThread::nearClip;
	float rtRenderer::rayTraceThread::farClip;
	int rtRenderer::rayTraceThread::maxBounces;
	ofPixels* rtRenderer::rayTraceThread::bufferPixels;
	float rtRenderer::rayTraceThread::bufferWidth;
	float rtRenderer::rayTraceThread::bufferHeight;
	rtVec3f rtRenderer::rayTraceThread::firstRow;
	rtVec3f rtRenderer::rayTraceThread::hStep;
	rtVec3f rtRenderer::rayTraceThread::vStep;

	///rayTraceThread methods
	//Set the shared data used to render the image
	void rtRenderer::rayTraceThread::setData(shared_ptr<rtScene>scene, rtVec3f& camPos, float nearClip, float farClip,
		int maxBounces, ofPixels* bufferPixels, rtVec3f& firstRow, rtVec3f& hStep, rtVec3f& vStep)
	{
		//Scene data
		objects = scene->getObjects();
		lights = scene->getLights();
		//Camera Data
		rayTraceThread::camPos = camPos;
		rayTraceThread::nearClip = nearClip;
		rayTraceThread::farClip = farClip;
		rayTraceThread::maxBounces = maxBounces;
		//Buffer data
		rayTraceThread::bufferPixels = bufferPixels;
		bufferWidth = bufferPixels->getWidth();
		bufferHeight = bufferPixels->getHeight();
		//Ray iteration data
		rayTraceThread::firstRow = firstRow;
		rayTraceThread::hStep = hStep;
		rayTraceThread::vStep = vStep;
	}

	//Set the section of the image to render
	void rtRenderer::rayTraceThread::setSection(int startRow, int endRow)
	{
		this->startRow = startRow;
		this->endRow = endRow;
	}

	//Renders a section of the frame buffer
	void rtRenderer::rayTraceThread::threadedFunction()
	{
		//The current index in the buffer pixels array
		int bufferIndex = bufferWidth * startRow * 3;

		//The first grid point of the current row. This vector stays on the left edge of the near clip plane and moves downwards
		rtVec3f currRow = firstRow + (vStep * startRow);
		//The current grid point. This vector is in the same row as currRowStart and moves to the right
		rtVec3f R = currRow;
		//The direction vector from the camera to the current point
		rtVec3f D;

		//Iterate over all the grid points
		for (int row = startRow; row < endRow; row++)
		{
			for (int col = 0; col < bufferPixels->getWidth(); col++)
			{
				//Find new direction vector
				D = (R - camPos).normalize();
				//Calculate the color of the pixel
				rtColorf pixelColor = rayTrace(objects, lights, camPos, D, nearClip, farClip, 0, maxBounces);
				//Write the color to the pixel buffer
				(*bufferPixels)[bufferIndex++] = (int)(pixelColor.getR() * 255.0f);
				(*bufferPixels)[bufferIndex++] = (int)(pixelColor.getG() * 255.0f);
				(*bufferPixels)[bufferIndex++] = (int)(pixelColor.getB() * 255.0f);

				//Iterate to the next column
				R += hStep;
			}

			//After a row is completed, move to the next row
			currRow += vStep;
			R = currRow;
		}
	}
}