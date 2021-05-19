#pragma once

#include "rtObject.h"

namespace rtGraphics
{
	/*
	 * A cylinder object defined by a position and radius
	 * TO-DO: Move the position member into rtNode
	 *        Add an axis member to define which axis the cylinder aligns with
	 */
	class rtCylinderObject : public rtObject
	{
	private:
		rtVec3f position;
		float radius;

	public:
		///Constructors
		rtCylinderObject();
		rtCylinderObject(rtMat& material);
		rtCylinderObject(const rtVec3f& position, float radius);
		rtCylinderObject(const rtVec3f& position, float radius, rtMat& material);

		///Getters
		rtVec3f getPosition() const;
		float getRadius() const;

		///Setters
		void setPosition(const rtVec3f& position);
		void setRadius(float radius);

		///Inherited Methods
		rtRayHit rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, rtRayHit originPoint);
		rtRayHit sdf(rtVec3f P);
	};

	///Constructors
	inline rtCylinderObject::rtCylinderObject() : rtObject()
	{
		position = rtVec3f::zero;
		radius = 1.0f;
	}

	inline rtCylinderObject::rtCylinderObject(rtMat& material) : rtObject(material)
	{
		position = rtVec3f::zero;
		radius = 1.0f;
	}

	inline rtCylinderObject::rtCylinderObject(const rtVec3f& position, float radius) :
		rtObject(),
		position(position),
		radius(radius) {
	}

	inline rtCylinderObject::rtCylinderObject(const rtVec3f& position, float radius, rtMat& material) :
		rtObject(material),
		position(position),
		radius(radius) {
	}

	///In-line method definitions
	//Getters
	inline rtVec3f rtCylinderObject::getPosition() const { return position; }
	inline float rtCylinderObject::getRadius() const { return radius; }

	//Setters
	inline void rtCylinderObject::setPosition(const rtVec3f& position) { this->position = position; }
	inline void rtCylinderObject::setRadius(float radius) { this->radius = radius; }
}