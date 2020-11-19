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

	//Ray trace a single ray
	rtColorf rtRenderer::rayTrace(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, int currBounce, int maxBounces)
	{
		//The distance from the camera to the intersection point
		float minT = INFINITY;
		//The intersection data of the closest intersection
		shared_ptr<rtRayHit> nearestHit;

		//Iterate over the all the objects
		for (auto objectPtr = objects->begin(); objectPtr != objects->end(); objectPtr++)
		{
			//Get a pointer to the current object
			rtObject* currObject = objectPtr->second;
			//Determine if the ray intersects the object
			shared_ptr<rtRayHit> hitData = currObject->rayIntersect(P, D, nearClip, farClip);

			//If the ray hit and the object is not obscured, save the ray parameter and object address
			if (hitData->hit && hitData->distance < minT)
			{
				//Update the distance of the closest intersection
				minT = hitData->distance;
				//Update the hit data of the nearest intersection point
				nearestHit = hitData;
			}
		}

		//If the ray didn't intersect any objects, return a black pixel
		//TO-DO: Return the background color of the camera
		if (minT < nearClip || minT > farClip)
			return rtColorf::black;
		//Otherwise calculate the lighting of the pixel
		else
		{
			//Get the reflectivity of the material
			float reflectivity = nearestHit->hitObject->getMat().getReflectivity();

			//If the object isn't reflective at all, return only the object color
			if (reflectivity == 0.0f)
				return calculateColor(nearestHit, D, lights);
			//If the object is perfectly reflective, returned only the reflected color
			else if (reflectivity == 1.0f)
				return bounceRay(objects, lights, P, D, nearClip, farClip, currBounce, maxBounces, nearestHit);
			//If the object is partially reflective, blend the object and reflected colors
			else
			{
				//Calculate the color of the object
				rtColorf objectColor = calculateColor(nearestHit, D, lights);
				rtColorf reflectedColor = bounceRay(objects, lights, P, D, nearClip, farClip, currBounce, maxBounces, nearestHit);

				rtColorf finalColor = (objectColor * (1 - reflectivity)) + (reflectedColor * reflectivity);
				finalColor.clampColors();

				return finalColor;
			}
		}
	}

	//Bounce a ray off of the object it hits and find the reflected color
	rtColorf rtRenderer::bounceRay(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D,
		float nearClip, float farClip, int currBounce, int maxBounces, shared_ptr<rtRayHit> hitData)
	{
		//If the ray has already bounced too many times return black
		if (currBounce >= maxBounces)
			return rtColorf::black;

		//Negate the ray direction
		rtVec3f negatedRay = -D;
		//Reflect the negated ray to get the direction of the reflected ray
		rtVec3f reflectedRay = negatedRay.getReflected(hitData->hitNormal);
		//Traced the bounced ray to get the reflected color. The near clip is set to 0 since it is not needed
		return rayTrace(objects, lights, hitData->hitPoint, reflectedRay, 0.0f, farClip, ++currBounce, maxBounces);
	}

	//Ray trace a single ray
	rtColorf rtRenderer::calculateColor(shared_ptr<rtRayHit> hitData, rtVec3f n, lightSet& lights)
	{
		//The final color to the drawn to the pixel
		rtColorf finalColor = rtColorf();
		//The material of the object
		rtMat objectMat = hitData->hitObject->getMat();

		//Iterate over the all the lights
		for (auto lightPtr = lights->begin(); lightPtr != lights->end(); lightPtr++)
		{
			//Get a pointer to the current light
			rtLight* currLight = lightPtr->second;
			//Cache the ambient and light intensity
			float incidentIntensity = currLight->getIncidentIntensity();
			float ambientIntensity = currLight->getAmbientIntensity();

			//Calculate the light vector, which is used in both diffuse and specular lighting
			rtVec3f lightVector = (currLight->getPosition() - hitData->hitPoint);
			lightVector.normalize();

			//Add the three types of lighting from this light to the final color
			finalColor += ambientColor((*currLight).getAmbient(), objectMat.getAmbient(), ambientIntensity);
			finalColor += diffuseColor(lightVector, hitData->hitNormal, (*currLight).getDiffuse(), objectMat.getDiffuse(), incidentIntensity);
			finalColor += specularColor(lightVector, n, hitData->hitNormal, (*currLight).getSpecular(), objectMat.getSpecular(), objectMat.getSmoothness(), incidentIntensity);
			//Clamp the values of the color
			finalColor.clampColors();

			return finalColor;
		}
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