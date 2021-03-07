#pragma once

#include "../rtObject.h"

namespace rtGraphics
{
	/*
	 * A sphere object defined by a position and radius
	 * TO-DO: Move the position member into rtNode
	 */
	class sphereObject : public rtObject
	{
	private:
		rtVec3f center;
		float radius;

	public:
		///Constructors
		sphereObject();
		sphereObject(rtMat& material);
		sphereObject(const rtVec3f& position, float radius);
		sphereObject(const rtVec3f& position, float radius, rtMat& material);

		///Getters
		rtVec3f getCenter() const;
		float getRadius() const;

		///Setters
		void setCenter(const rtVec3f& center);
		void setRadius(float radius);

		///Inherited Method
		shared_ptr<rtRayHit> rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint = nullptr);
	};

	///Constructors
	inline sphereObject::sphereObject() : rtObject()
	{
		center = rtVec3f::zero;
		radius = 1.0f;
	}

	inline sphereObject::sphereObject(rtMat& material) : rtObject(material)
	{
		center = rtVec3f::zero;
		radius = 1.0f;
	}

	inline sphereObject::sphereObject(const rtVec3f& center, float radius) :
		rtObject(),
		center(center),
		radius(radius) {
	}

	inline sphereObject::sphereObject(const rtVec3f& center, float radius, rtMat& material) :
		rtObject(material),
		center(center),
		radius(radius) {
	}

	///In-line method definitions
	//Getters
	inline rtVec3f sphereObject::getCenter() const { return center; }
	inline float sphereObject::getRadius() const { return radius; }

	//Setters
	inline void sphereObject::setCenter(const rtVec3f& center) { this->center = center; }
	inline void sphereObject::setRadius(float radius) { this->radius = radius; }
}