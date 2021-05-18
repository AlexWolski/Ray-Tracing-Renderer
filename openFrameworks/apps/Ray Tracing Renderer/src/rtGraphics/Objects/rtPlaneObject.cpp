#include "rtPlaneObject.h"

namespace rtGraphics
{
	//Ray-Plane Intersection
	shared_ptr<rtRayHit> rtPlaneObject::rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint)
	{
		//To-do: Implement torus ray intersection
		shared_ptr<rtRayHit> hitData = make_shared<rtRayHit>();
		hitData->hit = false;
		return hitData;
	}


	//Plane signed distance function
	shared_ptr<rtRayHit> rtPlaneObject::sdf(rtVec3f P)
	{
		rtVec3f localPoint = P - position;
		//To-do: cache the magnitude of the normal when it is set
		float dotProd = localPoint.dot(normal) + normal.magnitude();

		//Construct the rtRayHit struct and return it
		shared_ptr<rtRayHit> distData = make_shared<rtRayHit>();
		distData->distance = dotProd;
		distData->hitObject = this;
		return distData;
	}
}