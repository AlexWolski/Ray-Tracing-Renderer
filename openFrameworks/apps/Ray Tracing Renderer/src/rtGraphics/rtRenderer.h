#pragma once

#include <memory>
#include <math.h>
#include <vector>
#include "ofPixels.h"
#include "ofThread.h"
#include "Data Classes/rtScene.h"
#include "PhongShader.h"
#include "rtRenderThreadPool.h"

#define PIf 3.14159265f
#define degToRad(angleDegrees) ((angleDegrees) * PIf / 180.0f)

namespace rtGraphics
{
	//Forward declare rtRenderThreadPool to prevent a circular dependency
	class rtRenderThreadPool;

	class rtRenderer
	{
	private:
		//The pool of render threads
		unique_ptr<rtRenderThreadPool> threadPool;

		//Bounce the ray off of an object and calculate the color at the next intersection point
		static rtColorf bounceRay(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D,float nearClip, float farClip, int currBounce, int maxBounces, shared_ptr<rtRayHit> hitData);
		//Determine if a given light shines on the target point or is occluded. The ray hit point is required to resolve surface intersection issues.
		static bool isShadow(objectSet& objects, rtVec3f& lightVector, rtVec3f& targetPoint, float lightDistSquared, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint = nullptr);

	public:
		//Initialize the thread pool
		rtRenderer(shared_ptr<rtScene> scene, rtVec3f& camPos, rtVec3f& u, rtVec3f& v, rtVec3f& n,
			float hFov, float nearClip, float farClip, int maxBounces, ofPixels* bufferPixels);

		//Ray trace an entire scene
		static void rayTraceScene(shared_ptr<rtScene> scene, rtVec3f& camPos, rtVec3f& u, rtVec3f& v, rtVec3f& n,
			float hFov, float nearClip, float farClip, int maxBounces, ofPixels* bufferPixels);

		//Ray trace a single ray and return the color at the intersection. If the ray is a bounced ray, the ray hit data can be given to resolve surface intersection issues.
		static rtColorf rayTrace(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, int currBounce, int maxBounces, shared_ptr<rtRayHit> originPoint = nullptr);
		//Ray trace a single ray and return the ray hit data. If the ray is a bounced ray, the ray hit data can be given to resolve surface intersection issues.
		static shared_ptr<rtRayHit> rtRenderer::rayTrace(objectSet& objects, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, shared_ptr<rtRayHit> sourceObject = nullptr);
	};
}