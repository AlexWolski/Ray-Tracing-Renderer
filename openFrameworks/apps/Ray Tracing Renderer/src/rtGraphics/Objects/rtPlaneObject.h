#pragma once

#include "rtObject.h"

namespace rtGraphics
{
	/*
	 * A plane object defined by a position and normal
	 * TO-DO: Move the position member into rtNode
	 */
	class rtPlaneObject : public rtObject
	{
	private:
		rtVec3f position;
		rtVec3f normal;

	public:
		///Constructors
		rtPlaneObject();
		rtPlaneObject(rtMat& material);
		rtPlaneObject(const rtVec3f& position, const rtVec3f& normal);
		rtPlaneObject(const rtVec3f& position, const rtVec3f& normal, rtMat& material);

		///Getters
		rtVec3f getPosition() const;
		rtVec3f getNormal() const;

		///Setters
		void setPosition(const rtVec3f& position);
		void setNormal(const rtVec3f& normal);

		///Inherited Methods
		shared_ptr<rtRayHit> rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint = nullptr);
		shared_ptr<rtRayHit> sdf(rtVec3f P);
	};

	///Constructors
	inline rtPlaneObject::rtPlaneObject() : rtObject()
	{
		normal = rtVec3f(0.0f, 1.0f, 0.0f);
		position = rtVec3f::zero;
	}

	inline rtPlaneObject::rtPlaneObject(rtMat& material) : rtObject(material)
	{
		normal = rtVec3f(0.0f, 1.0f, 0.0f);
		position = rtVec3f::zero;
	}

	inline rtPlaneObject::rtPlaneObject(const rtVec3f& position, const rtVec3f& normal) :
		rtObject(),
		position(position),
		normal(normal.getNormalized())
	{
	}

	inline rtPlaneObject::rtPlaneObject(const rtVec3f& position, const rtVec3f& normal, rtMat& material) :
		position(position),
		normal(normal.getNormalized())
	{
	}

	///In-line method definitions
	//Getters
	inline rtVec3f rtPlaneObject::getPosition() const { return position; }
	inline rtVec3f rtPlaneObject::getNormal() const { return normal; }

	//Setters
	inline void rtPlaneObject::setPosition(const rtVec3f& position) { this->position = position; }
	inline void rtPlaneObject::setNormal(const rtVec3f& normal) { this->normal = normal.getNormalized(); }
}