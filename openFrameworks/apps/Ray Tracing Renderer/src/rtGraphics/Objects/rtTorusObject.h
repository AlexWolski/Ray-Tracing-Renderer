#pragma once

#include "rtObject.h"

namespace rtGraphics
{
	/*
	 * A sphere object defined by a position and radius
	 * TO-DO: Move the position member into rtNode
	 */
	class rtTorusObject : public rtObject
	{
	private:
		rtVec3f center;
		float majorRadius;
		float minorRadius;

	public:
		///Constructors
		rtTorusObject();
		rtTorusObject(rtMat& material);
		rtTorusObject(const rtVec3f& position, float majorRadius, float minorRadius);
		rtTorusObject(const rtVec3f& position, float majorRadius, float minorRadius, rtMat& material);

		///Getters
		rtVec3f getCenter() const;
		float getCircleRadius() const;
		float getTubeRadius() const;

		///Setters
		void setCenter(const rtVec3f& center);
		void setCircleRadius(float majorRadius);
		void setTubeRadius(float minorRadius);

		///Inherited Methods
		shared_ptr<rtRayHit> rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint = nullptr);
		shared_ptr<rtRayHit> sdf(rtVec3f P);
	};

	///Constructors
	inline rtTorusObject::rtTorusObject() : rtObject()
	{
		center = rtVec3f::zero;
		majorRadius = 1.0f;
		minorRadius = 0.2f;
	}

	inline rtTorusObject::rtTorusObject(rtMat& material) : rtObject(material)
	{
		center = rtVec3f::zero;
		majorRadius = 1.0f;
		minorRadius = 0.2f;
	}

	inline rtTorusObject::rtTorusObject(const rtVec3f& center, float majorRadius, float minorRadius) :
		rtObject(),
		center(center),
		majorRadius(majorRadius),
		minorRadius(minorRadius)
	{
	}

	inline rtTorusObject::rtTorusObject(const rtVec3f& center, float majorRadius, float minorRadius, rtMat& material) :
		rtObject(material),
		center(center),
		majorRadius(majorRadius),
		minorRadius(minorRadius)
	{
	}

	///In-line method definitions
	//Getters
	inline rtVec3f rtTorusObject::getCenter() const { return center; }
	inline float rtTorusObject::getCircleRadius() const { return majorRadius; }
	inline float rtTorusObject::getTubeRadius() const { return minorRadius; }

	//Setters
	inline void rtTorusObject::setCenter(const rtVec3f& center) { this->center = center; }
	inline void rtTorusObject::setCircleRadius(float radius) { this->majorRadius = radius; }
	inline void rtTorusObject::setTubeRadius(float radius) { this->minorRadius = minorRadius; }
}