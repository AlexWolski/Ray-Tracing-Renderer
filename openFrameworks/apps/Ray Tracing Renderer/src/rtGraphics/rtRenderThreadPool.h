#pragma once

#include "memory.h"
#include "rtRenderer.h"

namespace rtGraphics
{
	//A struct containing the data shared by all threads in the pool
	struct RenderThreadData
	{
	public:
		RenderThreadData(shared_ptr<rtScene>scene, rtVec3f& camPos, float nearClip, float farClip,
			int maxBounces, ofPixels* bufferPixels, rtVec3f& firstPoint, rtVec3f& hStep, rtVec3f& vStep);

		//Scene data
		objectSet objects;
		lightSet lights;
		//Camera data
		rtVec3f camPos;
		float nearClip, farClip;
		int maxBounces;
		//Output buffer data
		ofPixels* bufferPixels;
		float bufferWidth, bufferHeight;
		//Grid data
		rtVec3f firstPoint, hStep, vStep;
	};


	//A threadable nested class that renders a portion of the final image
	class RenderThread : public ofThread
	{
	private:
		//Shared data
		shared_ptr<RenderThreadData> sharedData;
		//Instance data
		int startRow, endRow;

		//Renders a section of the frame buffer
		void threadedFunction();

	public:
		//Set the shared data
		void setData(shared_ptr<RenderThreadData> sharedData)
		{
			this->sharedData = sharedData;
		}

		//Set the section of the image to render
		void setSection(int startRow, int endRow)
		{
			this->startRow = startRow;
			this->endRow = endRow;
		}
	};


	class rtRenderThreadPool
	{
	private:
		//The number of threads in the pool
		static int numThreads;
		//Shared thread data
		shared_ptr<RenderThreadData> sharedData;
		//A pool of threads to render the image
		unique_ptr<RenderThread[]> threadPool;

	public:
		//Initialize a pool of threads given the render settings and scene
		rtRenderThreadPool(shared_ptr<rtScene> scene, rtVec3f& camPos, rtVec3f& u, rtVec3f& v, rtVec3f& n,
			float hFov, float nearClip, float farClip, int maxBounces, ofPixels* bufferPixels);

		//Stop any running threads
		~rtRenderThreadPool()
		{
			stopThreads();
		}

		//Thread management methods
		void startThreads();
		void stopThreads();
		void joinThreads();
	};
}