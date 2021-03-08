#pragma once

#include "rtObject.h"
#include "ofMain.h"

namespace rtGraphics
{
	/*
	 * A torus object defined by a position, major radius, and minor radius
	 * TO-DO: Move the position member into rtNode
	 */
	class rtTorusObject : public rtObject
	{
	private:
		rtVec3f center;
		rtVec3f rotation;
		float majorRadius;
		float minorRadius;

	public:
		///Constructors
		rtTorusObject();
		rtTorusObject(rtMat& material);
		rtTorusObject(const rtVec3f& position, const rtVec3f& rotation, float majorRadius, float minorRadius);
		rtTorusObject(const rtVec3f& position, const rtVec3f& rotation, float majorRadius, float minorRadius, rtMat& material);

		///Getters
		rtVec3f getCenter() const;
		rtVec3f getRotation() const;
		float getCircleRadius() const;
		float getTubeRadius() const;

		///Setters
		void setCenter(const rtVec3f& center);
		void setRotation(const rtVec3f& rotation);
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
		rotation = rtVec3f::zero;
		majorRadius = 1.0f;
		minorRadius = 0.2f;
	}

	inline rtTorusObject::rtTorusObject(rtMat& material) : rtObject(material)
	{
		center = rtVec3f::zero;
		rotation = rtVec3f::zero;
		majorRadius = 1.0f;
		minorRadius = 0.2f;
	}

	inline rtTorusObject::rtTorusObject(const rtVec3f& center, const rtVec3f& rotation, float majorRadius, float minorRadius) :
		rtObject(),
		center(center),
		rotation(rotation),
		majorRadius(majorRadius),
		minorRadius(minorRadius)
	{
	}

	inline rtTorusObject::rtTorusObject(const rtVec3f& center, const rtVec3f& rotation, float majorRadius, float minorRadius, rtMat& material) :
		rtObject(material),
		center(center),
		rotation(rotation),
		majorRadius(majorRadius),
		minorRadius(minorRadius)
	{
	}

	///In-line method definitions
	//Getters
	inline rtVec3f rtTorusObject::getCenter() const { return center; }
	inline rtVec3f rtTorusObject::getRotation() const { return rotation; }
	inline float rtTorusObject::getCircleRadius() const { return majorRadius; }
	inline float rtTorusObject::getTubeRadius() const { return minorRadius; }

	//Setters
	inline void rtTorusObject::setCenter(const rtVec3f& center) { this->center = center; }
	inline void rtTorusObject::setRotation(const rtVec3f& rotation) { this->rotation = rotation; }
	inline void rtTorusObject::setCircleRadius(float radius) { this->majorRadius = radius; }
	inline void rtTorusObject::setTubeRadius(float radius) { this->minorRadius = minorRadius; }
}