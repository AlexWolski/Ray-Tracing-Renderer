#include "rtCylinderObject.h"

namespace rtGraphics
{
	//Ray-Cylinder Intersection
	shared_ptr<rtRayHit> rtCylinderObject::rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint)
	{
		//To-do: Implement cylinder ray intersection
		shared_ptr<rtRayHit> hitData = make_shared<rtRayHit>();
		hitData->hit = false;
		return hitData;
	}


	//Sphere signed distance function
	shared_ptr<rtRayHit> rtCylinderObject::sdf(rtVec3f P)
	{
		rtVec3f localPoint = P - position;
		localPoint.setY(0.0f);
		float dist = localPoint.magnitude() - radius;

		shared_ptr<rtRayHit> distData = make_shared<rtRayHit>();
		distData->distance = dist;
		distData->hitObject = this;

		return distData;
	}
}