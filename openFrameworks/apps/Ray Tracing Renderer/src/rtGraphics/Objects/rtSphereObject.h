#pragma once

#include "rtObject.h"

namespace rtGraphics
{
	/*
	 * A sphere object defined by a position and radius
	 * TO-DO: Move the position member into rtNode
	 */
	class rtSphereObject : public rtObject
	{
	private:
		rtVec3f center;
		float radius;

	public:
		///Constructors
		rtSphereObject();
		rtSphereObject(rtMat& material);
		rtSphereObject(const rtVec3f& position, float radius);
		rtSphereObject(const rtVec3f& position, float radius, rtMat& material);

		///Getters
		rtVec3f getCenter() const;
		float getRadius() const;

		///Setters
		void setCenter(const rtVec3f& center);
		void setRadius(float radius);

		///Inherited Methods
		rtRayHit rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, rtRayHit originPoint);
		rtRayHit sdf(rtVec3f P);
	};

	///Constructors
	inline rtSphereObject::rtSphereObject() : rtObject()
	{
		center = rtVec3f::zero;
		radius = 1.0f;
	}

	inline rtSphereObject::rtSphereObject(rtMat& material) : rtObject(material)
	{
		center = rtVec3f::zero;
		radius = 1.0f;
	}

	inline rtSphereObject::rtSphereObject(const rtVec3f& center, float radius) :
		rtObject(),
		center(center),
		radius(radius) {
	}

	inline rtSphereObject::rtSphereObject(const rtVec3f& center, float radius, rtMat& material) :
		rtObject(material),
		center(center),
		radius(radius) {
	}

	///In-line method definitions
	//Getters
	inline rtVec3f rtSphereObject::getCenter() const { return center; }
	inline float rtSphereObject::getRadius() const { return radius; }

	//Setters
	inline void rtSphereObject::setCenter(const rtVec3f& center) { this->center = center; }
	inline void rtSphereObject::setRadius(float radius) { this->radius = radius; }
}