#pragma once

#include "../rtObject.h"

/*
	 * A sphere object defined by a position and radius
	 * TO-DO: Move the position member into rtNode
	 */
class rtSphere : public rtObject
{
private:
	rtVec3f center;
	float radius;

public:
	///Constructors
	rtSphere();
	rtSphere(rtMat& material);
	rtSphere(const rtVec3f& position, float radius);
	rtSphere(const rtVec3f& position, float radius, rtMat& material);

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
inline rtSphere::rtSphere() : rtObject()
{
	center = rtVec3f::zero;
	radius = 1.0f;
}

inline rtSphere::rtSphere(rtMat& material) : rtObject(material)
{
	center = rtVec3f::zero;
	radius = 1.0f;
}

inline rtSphere::rtSphere(const rtVec3f& center, float radius) :
	rtObject(),
	center(center),
	radius(radius) {
}

inline rtSphere::rtSphere(const rtVec3f& center, float radius, rtMat& material) :
	rtObject(material),
	center(center),
	radius(radius) {
}

///In-line method definitions
//Getters
inline rtVec3f rtSphere::getCenter() const { return center; }
inline float rtSphere::getRadius() const { return radius; }

//Setters
inline void rtSphere::setCenter(const rtVec3f& center) { this->center = center; }
inline void rtSphere::setRadius(float radius) { this->radius = radius; }