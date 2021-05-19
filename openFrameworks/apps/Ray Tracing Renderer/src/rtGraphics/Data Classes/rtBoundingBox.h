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

	inline bool BoundingBox::intersect(rtVec3f P, rtVec3f D, float nearClip, float farClip)
	{
		return false;
	}
}