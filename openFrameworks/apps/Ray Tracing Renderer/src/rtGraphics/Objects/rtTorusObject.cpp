#include "rtTorusObject.h"

namespace rtGraphics
{
	//Ray-Torus Intersection
	shared_ptr<rtRayHit> rtTorusObject::rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint)
	{
		//To-do: Implement torus ray intersection
		shared_ptr<rtRayHit> hitData = make_shared<rtRayHit>();
		hitData->hit = false;
		return hitData;
	}


	//Torus signed distance function
	shared_ptr<rtRayHit> rtTorusObject::sdf(rtVec3f P)
	{
		rtVec3f localPoint = P - center;

		//Convert the point to an openFrameworks vector
		ofVec3f ofLocalPoint(localPoint.getX(), localPoint.getY(), localPoint.getZ());
		//Rotate the point
		ofLocalPoint.rotate(rotation.getX(), ofVec3f(1.0f, 0.0f, 0.0f));
		ofLocalPoint.rotate(rotation.getY(), ofVec3f(0.0f, 1.0f, 0.0f));
		ofLocalPoint.rotate(rotation.getZ(), ofVec3f(0.0f, 0.0f, 1.0f));
		//Convert the point back
		localPoint.set(ofLocalPoint.x, ofLocalPoint.y, ofLocalPoint.z);

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
		shared_ptr<rtRayHit> distData = make_shared<rtRayHit>();
		distData->distance = distance;
		distData->hitObject = this;

		return distData;
	}
}