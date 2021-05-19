#pragma once

#include "rtVec3f.h"

namespace rtGraphics
{
	class rtBoundingBox
	{
	private:
		rtVec3f min;
		rtVec3f max;

	public:
		rtBoundingBox();
		rtBoundingBox(rtVec3f min, rtVec3f max);

		rtVec3f getMin() const;
		rtVec3f getMax() const;
		void setMin(rtVec3f min);
		void setMax(rtVec3f max);
		bool intersect(rtVec3f P, rtVec3f D, float nearClip = 0.0f, float farClip = INFINITY) const;
	};

	//Constructors
	inline rtBoundingBox::rtBoundingBox() :
		min(rtVec3f::zero),
		max(rtVec3f::zero) {
	}

	inline rtBoundingBox::rtBoundingBox(rtVec3f min, rtVec3f max) :
		min(min),
		max(max) {
	}

	//Getters
	inline rtVec3f rtBoundingBox::getMin() const { return min; }
	inline rtVec3f rtBoundingBox::getMax() const { return max; }

	//Setters
	inline void rtBoundingBox::setMin(rtVec3f min) { this->min = min; }
	inline void rtBoundingBox::setMax(rtVec3f max) { this->max = max; }

	/*
	 * Uses the slab method to efficiently determine if a ray intersects the bounding box
	 * Adapted from http://psgraphics.blogspot.com/2016/02/new-simple-ray-box-test-from-andrew.html
	 */
	inline bool rtBoundingBox::intersect(rtVec3f P, rtVec3f D, float nearClip, float farClip) const
	{
		for (int axis = 0; axis < 3; axis++)
		{
			float inv_direction = 1.0f / D[axis];
			float t0, t1;

			if (inv_direction >= 0.0f)
			{
				t0 = (min[axis] - P[axis]) * inv_direction;
				t1 = (max[axis] - P[axis]) * inv_direction;
			}
			else
			{
				t0 = (max[axis] - P[axis]) * inv_direction;
				t1 = (min[axis] - P[axis]) * inv_direction;
			}

			float tmin = t0 > nearClip ? t0 : nearClip;
			float tmax = t1 < farClip ? t1 : farClip;

			if (tmax <= tmin)
				return false;
		}

		return true;
	}
}