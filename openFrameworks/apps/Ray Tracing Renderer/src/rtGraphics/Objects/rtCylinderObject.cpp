#include "rtCylinderObject.h"

namespace rtGraphics
{
	//Ray-Cylinder Intersection
	rtRayHit rtCylinderObject::rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, rtRayHit originPoint)
	{
		//To-do: Implement cylinder ray intersection
		rtRayHit hitData;
		hitData.hit = false;
		return hitData;
	}


	//Sphere signed distance function
	rtRayHit rtCylinderObject::sdf(rtVec3f P)
	{
		rtVec3f localPoint = P - position;
		localPoint.setY(0.0f);
		float dist = localPoint.magnitude() - radius;

		rtRayHit distData;
		distData.distance = dist;
		distData.hitObject = this;

		return distData;
	}
}