#pragma once

#include <memory>
#include <vector>
#include <math.h>
#include "../rtNode.h"
#include "../Data Classes/rtVec3f.h"
#include "../Data Classes/rtMat.h"
#include "../Data Classes/rtRayHit.h"

using namespace std;

namespace rtGraphics
{
	/*
	 * A wrapper class for ray traceable objects
	 * TO-DO: Replace the material member with a material name that
	 *        corresponds to a shared material in a data structure
	 */
	class rtObject : public rtNode
	{
	private:
		rtMat material;

	public:
		rtObject() : material(rtMat()) {}
		rtObject(rtMat& material) : material(material) {}
		rtMat& getMat();
		void setMat(const rtMat& material);

		/*
		 * Used for ray tracing
		 * Determines if a ray hits this object and returns the intersection data
		 * An optional rtRayHit struct can be passed for rays originating from an object to prevent intersections at the ray origin
		 */
		virtual shared_ptr<rtRayHit> rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint = nullptr) = 0;

		/*
		 * Used for ray marching
		 * Calculates the shortest distance between the given point and the object
		 */
		virtual shared_ptr<rtRayHit> sdf(rtVec3f P) = 0;
	};

	///In-line method definitions
	inline rtMat& rtObject::getMat()
	{
		return material;
	}

	inline void rtObject::setMat(const rtMat& material)
	{
		this->material = material;
	}
}