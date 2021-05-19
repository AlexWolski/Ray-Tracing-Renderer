#include "rtTorusObject.h"

namespace rtGraphics
{
	//Ray-Torus Intersection
	rtRayHit rtTorusObject::rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, rtRayHit originPoint)
	{
		//To-do: Implement torus ray intersection
		rtRayHit hitData;
		hitData.hit = false;
		return hitData;
	}


	//Torus signed distance function
	rtRayHit rtTorusObject::sdf(rtVec3f P)
	{
		rtVec3f localPoint = P - center;

		//Project the point onto the xz axis of the torus
		rtVec3f projPoint(localPoint.getX(), 0.0f, localPoint.getZ());
		//The closest distance from the projected point to the major circle of the torus
		float projLength = projPoint.magnitude();
		float distMajor = projLength - majorRadius;

		//The vector from the major circle of the torus to the point
		rtVec3f majorVec(distMajor, localPoint.getY(), 0.0f);
		//Get the distance from the point to the major circle of the torus
		float majorDist = majorVec.magnitude();
		//The distance from the point to the surface of the torus
		float distance = majorDist - minorRadius;

		//Construct the rtRayHit struct and return it
		rtRayHit distData;
		distData.distance = distance;
		distData.hitObject = this;

		return distData;
	}
}