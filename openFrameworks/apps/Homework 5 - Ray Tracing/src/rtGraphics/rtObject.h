#pragma once

#include <memory>
#include <vector>
#include <math.h>
#include "rtNode.h"
#include "rtVec3f.h"
#include "rtMat.h"
#include "rtMesh.h"

using namespace std;

namespace rtGraphics
{
	/*
	 * A wrapper class for ray traceable objects
	 * TO-DO: Replace the material member with a material name that
	 *        corresponds to a shared material in a data structure
	 */
	class rtObject : public rtNode
	{
	private:
		rtMat material;

	public:
		rtObject() : material(rtMat()) {}
		rtObject(rtMat& material) : material(material) {}
		rtMat& getMat();
		void setMat(const rtMat& material);

		virtual float rayIntersect(rtVec3f P, rtVec3f D, shared_ptr<rtVec3f> hitPos, shared_ptr<rtVec3f> hitNormal) = 0;
	};

	///In-line method definitions
	inline rtMat& rtObject::getMat()
	{
		return material;
	}

	inline void rtObject::setMat(const rtMat& material)
	{
		this->material = material;
	}


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
		/*
		 * Returns the distance between the camera and the intersection point
		 * Additionally, sets the hitPos and hitNormal vectors pointers
		 * If the result is less than 0, then the ray did not intersect the object
		 */
		float rayIntersect(rtVec3f P, rtVec3f D, shared_ptr<rtVec3f> hitPos, shared_ptr<rtVec3f> hitNormal);
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
	inline rtVec3f rtSphere::getCenter() const	{ return center; }
	inline float rtSphere::getRadius() const	{ return radius; }
	
	//Setters
	inline void rtSphere::setCenter(const rtVec3f& center)	{ this->center = center;  }
	inline void rtSphere::setRadius(float radius)			{ this->radius = radius; }

	/*
	 * A mesh-based object that applies a transform to the mesh
	 * TO-DO: Replace the mesh member with a mesh name that
	 *        corresponds to a shared mesh in a data structure
	*/
	class rtMeshObject : public rtObject
	{
	private:
		rtMesh mesh;

	public:
		///Constructors
		rtMeshObject() : mesh(rtMesh()) {}
		rtMeshObject(rtMesh& mesh) : mesh(mesh) {}
		rtMeshObject(rtMesh& mesh, rtMat& material) : rtObject(material), mesh(mesh) {}

		///Getter & Setter
		rtMesh& getMesh();
		void setMesh(rtMesh& mesh);

		///Inherited Methods
		float rayIntersect(rtVec3f P, rtVec3f D, shared_ptr<rtVec3f> hitPos, shared_ptr<rtVec3f> hitNormal);
	};

	///In-line method definitions
	//Getter & Setter
	inline rtMesh& rtMeshObject::getMesh()			{ return mesh; }
	inline void rtMeshObject::setMesh(rtMesh& mesh)	{ this->mesh = mesh; }
}