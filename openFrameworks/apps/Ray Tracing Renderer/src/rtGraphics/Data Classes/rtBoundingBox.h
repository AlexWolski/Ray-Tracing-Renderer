#pragma once

#include "rtVec3f.h"

namespace rtGraphics
{
	class BoundingBox
	{
	private:
		BoundingBox();
		BoundingBox(rtVec3f min, rtVec3f max);

		rtVec3f min;
		rtVec3f max;

	public:
		rtVec3f getMin() const;
		rtVec3f getMax() const;
		void setMin(rtVec3f min);
		void setMax(rtVec3f max);
		bool intersect(rtVec3f P, rtVec3f D, float nearClip, float farClip);
	};

	//Constructors
	inline BoundingBox::BoundingBox() :
		min(rtVec3f::zero),
		max(rtVec3f::zero) {
	}

	inline BoundingBox::BoundingBox(rtVec3f min, rtVec3f max) :
		min(min),
		max(max) {
	}

	//Getters
	inline rtVec3f BoundingBox::getMin() const { return min; }
	inline rtVec3f BoundingBox::getMax() const { return max; }

	//Setters
	inline void BoundingBox::setMin(rtVec3f min) { this->min = min; }
	inline void BoundingBox::setMax(rtVec3f max) { this->max = max; }

	/*
	 * Uses the slab method to efficiently determine if a ray intersects the bounding box
	 * Adapted from http://psgraphics.blogspot.com/2016/02/new-simple-ray-box-test-from-andrew.html
	 */
	inline bool BoundingBox::intersect(rtVec3f P, rtVec3f D, float nearClip, float farClip)
	{
		for (int axis = 0; axis < 3; axis++)
		{
			float inv_direction = 1.0f / D[axis];
			float t0 = (min[axis] - P[axis]) * inv_direction;
			float t1 = (max[axis] - P[axis]) * inv_direction;

			if (inv_direction < 0.0f)
			{
				float temp = t0;
				t0 = t1;
				t1 = temp;
			}

			float tmin = t0 > nearClip ? t0 : nearClip;
			float tmax = t1 < farClip ? t1 : farClip;

			if (tmax <= tmin)
				return false;
		}

		return true;
	}
}