#pragma once

#include <memory>
#include <math.h>
#include <vector>
#include "ofPixels.h"
#include "ofThread.h"
#include "Data Classes/rtScene.h"
#include "PhongShader.h"

#define PIf 3.14159265f
#define degToRad(angleDegrees) ((angleDegrees) * PIf / 180.0f)

namespace rtGraphics
{
	class rtRenderer
	{
	private:
		//A threadable nested class that renders a portion of the final image
		class rayTraceThread : public ofThread
		{
		private:
			///Shared Data
			//Scene data
			static objectSet objects;
			static lightSet lights;
			//Camera data
			static rtVec3f camPos;
			static float nearClip, farClip;
			static int maxBounces;
			//Output buffer data
			static ofPixels* bufferPixels;
			static float bufferWidth, bufferHeight;
			//Grid data
			static rtVec3f firstRow, hStep, vStep;

			///Instance data
			int startRow, endRow;

			//Renders a section of the frame buffer
			void threadedFunction();

		public:
			//Set the shared data
			static void setData(shared_ptr<rtScene> scene, rtVec3f& camPos, float nearClip, float farClip, int maxBounces,
				ofPixels* bufferPixels, rtVec3f& firstRow, rtVec3f& hStep, rtVec3f& vStep);

			//Set the section of the image to render
			void setSection(int startRow, int endRow);
		};

		//TO-DO: Set the number of threads based on the CPU cores and hyper threads
		static int numThreads;
		//A pool of threads to render the image
		static rayTraceThread* threadPool;

		//Instantiates the thread pool
		static rayTraceThread* makeThreads();
		//Manages the threads
		static void startThreads();
		static void stopThreads();
		static void joinThreads();
		//Bounce the ray off of an object and calculate the color at the next intersection point
		static rtColorf bounceRay(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D,float nearClip, float farClip, int currBounce, int maxBounces, shared_ptr<rtRayHit> hitData);
		//Determine if a given light shines on the target point or is occluded. The ray hit point is required to resolve surface intersection issues.
		static bool isShadow(objectSet& objects, rtVec3f& lightVector, rtVec3f& targetPoint, float lightDistSquared, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint = nullptr);

	public:
		//Ray trace an entire scene
		static void rayTraceScene(shared_ptr<rtScene> scene, rtVec3f& camPos, rtVec3f& u, rtVec3f& v, rtVec3f& n,
			float hFov, float nearClip, float farClip, int maxBounces, ofPixels* bufferPixels);

		//Ray trace a single ray and return the color at the intersection. If the ray is a bounced ray, the ray hit data can be given to resolve surface intersection issues.
		static rtColorf rayTrace(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, int currBounce, int maxBounces, shared_ptr<rtRayHit> originPoint = nullptr);
		//Ray trace a single ray and return the ray hit data. If the ray is a bounced ray, the ray hit data can be given to resolve surface intersection issues.
		static shared_ptr<rtRayHit> rtRenderer::rayTrace(objectSet& objects, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, shared_ptr<rtRayHit> sourceObject = nullptr);
	};

	///In-line method definitions
	inline void rtRenderer::startThreads()
	{
		for (int threadIndex = 0; threadIndex < numThreads; threadIndex++)
			if (!threadPool[threadIndex].isThreadRunning())
				threadPool[threadIndex].startThread();
	}

	inline void rtRenderer::stopThreads()
	{
		for (int threadIndex = 0; threadIndex < numThreads; threadIndex++)
			if (threadPool[threadIndex].isThreadRunning())
				threadPool[threadIndex].stopThread();
	}

	inline void rtRenderer::joinThreads()
	{
		for (int threadIndex = 0; threadIndex < numThreads; threadIndex++)
			if (threadPool[threadIndex].isThreadRunning())
				threadPool[threadIndex].waitForThread();
	}
}