#pragma once

#include <memory>
#include <math.h>
#include <vector>
#include "ofPixels.h"
#include "ofThread.h"
#include "Data Classes/rtScene.h"
#include "Data Classes/Data Types.h"
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

		///Constants for ray marching
		//The maximum number of ray march iterations
		static int maxIters;
		//The minimum distance between a ray and an object that would be considered a hit
		static float minHitDist;
		//The epsilon value to use when calculating the normal of an sdf
		static float normalEps;

		///Ray tracing methods
		//Bounce the ray off of an object and calculate the color at the next intersection point
		static rtColorf bounceRayRT(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D,float nearClip, float farClip, int currBounce, int maxBounces, shared_ptr<rtRayHit> hitData);
		//Determine, using ray tracing, if a given light shines on the target point or is occluded. The ray hit point is required to resolve surface intersection issues.
		static bool isShadowRT(objectSet& objects, rtVec3f& lightVector, rtVec3f& targetPoint, float lightDistSquared, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint = nullptr);

		///Ray marching methods
		//Update the normal of an rtRayHit struct
		static void updateNormalRM(shared_ptr<rtRayHit>);
	public:
		//Initialize the thread pool
		rtRenderer();
		//Render the scene
		void render(renderMode RenderMode, shared_ptr<rtScene> scene, rtVec3f& camPos, rtVec3f& u, rtVec3f& v, rtVec3f& n,
			float hFov, float nearClip, float farClip, int maxBounces, ofPixels* bufferPixels);
		//Wait for the current render to complete
		void waitForRender();

		///Ray tracing methods
		//Ray trace a single ray and return the color at the intersection. If the ray is a bounced ray, the ray hit data can be given to resolve surface intersection issues.
		static rtColorf rayTrace(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, int currBounce, int maxBounces, shared_ptr<rtRayHit> originPoint = nullptr);
		//Ray trace a single ray and return the ray hit data. If the ray is a bounced ray, the ray hit data can be given to resolve surface intersection issues.
		static shared_ptr<rtRayHit> rayTrace(objectSet& objects, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, shared_ptr<rtRayHit> sourceObject = nullptr);

		///Ray marching methods
		//Ray march a single ray and return the color at the intersection. If the ray is a bounced ray, the ray hit data can be given to resolve surface intersection issues.
		static rtColorf rayMarch(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, int currBounce, int maxBounces, shared_ptr<rtRayHit> originPoint = nullptr);
		//Ray march a single ray and return the closest object. If the ray is a bounced ray, the ray distance data can be given to resolve surface intersection issues.
		static shared_ptr<rtRayHit> rayMarch(objectSet& objects, rtVec3f& P, rtVec3f& D, float nearClip, float farClip, shared_ptr<rtRayHit> sourceObject = nullptr);
	};
}