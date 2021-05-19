#include "rtSphereObject.h"

namespace rtGraphics
{
	//Ray-Sphere Intersection
	rtRayHit rtSphereObject::rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, rtRayHit originPoint)
	{
		//Create a struct to store the ray cast data.
		rtRayHit hitData;
		//By default, set the hit flag to false
		hitData.hit = false;

		//Define an intermediate variable M as the vector from the sphere center to the ray origin
		rtVec3f M = P - center;

		//Calculate the dot product D*M and the magnitude of M
		float dotProd = D.dot(M);
		//The square of the magnitude the M vector. We can find this by just dotting M with itself
		float squaredMagM = M.dot(M);

		//The squared magnitude must be positive
		if (squaredMagM < 0)
			squaredMagM = -squaredMagM;

		//Calculate the discriminant of the quadratic formula
		float discriminant = (dotProd * dotProd) - (squaredMagM - radius * radius);

		//If the discriminant is less than 0, then the ray doesn't intersect the sphere
		if (discriminant < 0.0f)
			return hitData;

		//Compute the rest of the quadratic formula to find the intersection distance
		float sqrtDisc = sqrt(discriminant);
		float tSub = -dotProd - sqrtDisc;
		float tAdd = -dotProd + sqrtDisc;

		//The minimum positive intersection distance
		float t;

		//If the ray is on the object surface, ignore the intersection at that point
		if (originPoint.hitObject == this)
		{
			//Get the absolute value of each intersection distance
			float tSubAbs = abs(tSub);
			float tAddAbs = abs(tAdd);

			//If the ray it pointing outwards from the sphere, then it does not intersect
			if (tSubAbs > tAddAbs)
				return hitData;
			//If the ray is pointing in towards the sphere, use the intersection point on the other side
			else
				t = tAdd;
		}
		//If the ray is not on the object surface, use the minimum intersection distance
		else
		{
			//Use the minimum positive intersection distance
			if (tSub < 0.0f)
				t = tAdd;
			else
				t = tSub;
		}

		//If the  intersection point is still behind the ray origin, then the ray does not intersect
		if (t < 0.0f)
			return hitData;

		//Calculate the point of intersection and the normal at that point
		rtVec3f hitPoint = P + (D * t);
		rtVec3f hitNormal = hitPoint - center;
		hitNormal.normalize();

		//Store the hit data into the struct
		hitData.hit = true;
		hitData.hitObject = this;
		hitData.distance = t;
		hitData.hitPoint = hitPoint;
		hitData.hitNormal = hitNormal;

		//Return the intersection data
		return hitData;
	}


	//Sphere signed distance function
	rtRayHit rtSphereObject::sdf(rtVec3f P)
	{
		rtRayHit distData;
		distData.distance = (P - center).magnitude() - radius;
		distData.hitObject = this;

		return distData;
	}
}