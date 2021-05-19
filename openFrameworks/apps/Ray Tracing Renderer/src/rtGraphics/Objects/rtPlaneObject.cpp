#include "rtPlaneObject.h"

namespace rtGraphics
{
	//Ray-Plane Intersection
	rtRayHit rtPlaneObject::rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, rtRayHit originPoint)
	{
		//To-do: Implement torus ray intersection
		rtRayHit hitData;
		hitData.hit = false;
		return hitData;
	}


	//Plane signed distance function
	rtRayHit rtPlaneObject::sdf(rtVec3f P)
	{
		rtVec3f localPoint = P - position;
		//To-do: cache the magnitude of the normal when it is set
		float dotProd = localPoint.dot(normal) + normal.magnitude();

		//Construct the rtRayHit struct and return it
		rtRayHit distData;
		distData.distance = dotProd;
		distData.hitObject = this;
		return distData;
	}
}