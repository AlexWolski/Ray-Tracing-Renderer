#include "rtRenderThreadPool.h"

namespace rtGraphics
{
	///Static data member initialization
	//Set the number of threads to the number of cores on the machine
	int rtRenderThreadPool::numThreads = thread::hardware_concurrency();

	RenderThreadData::RenderThreadData(renderMode RenderMode, shared_ptr<rtScene>scene, rtVec3f& camPos, float nearClip, float farClip,
		int maxBounces, ofPixels* bufferPixels, rtVec3f& firstPoint, rtVec3f& hStep, rtVec3f& vStep)
	{
		this->RenderMode = RenderMode;
		//Scene data
		this->objects = scene->getObjects();
		this->lights = scene->getLights();
		//Camera Data
		this->camPos = camPos;
		this->nearClip = nearClip;
		this->farClip = farClip;
		this->maxBounces = maxBounces;
		//Buffer data
		this->bufferPixels = bufferPixels;
		this->bufferWidth = bufferPixels->getWidth();
		this->bufferHeight = bufferPixels->getHeight();
		//Ray iteration data
		this->firstPoint = firstPoint;
		this->hStep = hStep;
		this->vStep = vStep;
	}

	//Renders a section of the frame buffer
	void RenderThread::threadedFunction()
	{
		//The current index in the buffer pixels array
		int bufferIndex = sharedData->bufferWidth * startRow * 3;

		//The first grid point of the current row. This vector stays on the left edge of the near clip plane and moves downwards
		rtVec3f currRow = sharedData->firstPoint + (sharedData->vStep * startRow);
		//The current grid point. This vector is in the same row as currRowStart and moves to the right
		rtVec3f R = currRow;
		//The direction vector from the camera to the current point
		rtVec3f D;

		//Iterate over all the grid points
		for (int row = startRow; row < endRow; row++)
		{
			for (int col = 0; col < sharedData->bufferPixels->getWidth(); col++)
			{
				//Find new direction vector
				D = (R - sharedData->camPos).normalize();

				rtColorf pixelColor;

				//Calculate the pixel color based on the current render mode
				switch (sharedData->RenderMode)
				{
				case renderMode::rayTrace:
					pixelColor = rtRenderer::rayTrace(sharedData->objects, sharedData->lights, sharedData->camPos, D, sharedData->nearClip, sharedData->farClip, 0, sharedData->maxBounces);
					break;

				case renderMode::rayMarch:
					//To-Do: Implement ray marching
					pixelColor = rtColorf::black;
					break;

				default:
					//If no render mode is selected, set it to black
					pixelColor = rtColorf::black;
				}
				
				
				//Write the color to the pixel buffer
				(*sharedData->bufferPixels)[bufferIndex++] = (int)(pixelColor.getR() * 255.0f);
				(*sharedData->bufferPixels)[bufferIndex++] = (int)(pixelColor.getG() * 255.0f);
				(*sharedData->bufferPixels)[bufferIndex++] = (int)(pixelColor.getB() * 255.0f);

				//Iterate to the next column
				R += sharedData->hStep;
			}

			//After a row is completed, move to the next row
			currRow += sharedData->vStep;
			R = currRow;
		}
	}


	rtRenderThreadPool::rtRenderThreadPool()
	{
		//Instantiate the thread pool
		threadPool = make_unique<RenderThread[]>(numThreads);
	}

	void rtRenderThreadPool::setData(renderMode RenderMode, shared_ptr<rtScene> scene, rtVec3f& camPos, rtVec3f& u, rtVec3f& v, rtVec3f& n,
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

		//Save the scene data and render settings in a struct
		sharedData = make_shared<RenderThreadData>(RenderMode, scene, camPos, nearClip, farClip, maxBounces, bufferPixels, firstPoint, hStep, vStep);

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

			//Set the shared data and section to render
			threadPool[threadIndex].setData(sharedData);
			threadPool[threadIndex].setSection(sectionStart, sectionEnd);

			//Move onto the next section
			sectionStart = sectionEnd;
		}
	}

	void rtRenderThreadPool::startThreads()
	{
		for (int threadIndex = 0; threadIndex < numThreads; threadIndex++)
			if (!threadPool[threadIndex].isThreadRunning())
				threadPool[threadIndex].startThread();
	}

	void rtRenderThreadPool::joinThreads()
	{
		for (int threadIndex = 0; threadIndex < numThreads; threadIndex++)
			if (threadPool[threadIndex].isThreadRunning())
				threadPool[threadIndex].waitForThread();
	}

	bool rtRenderThreadPool::threadsRunning()
	{
		for (int threadIndex = 0; threadIndex < numThreads; threadIndex++)
			if (~threadPool[threadIndex].isThreadRunning())
				return false;

		return true;
	}
}