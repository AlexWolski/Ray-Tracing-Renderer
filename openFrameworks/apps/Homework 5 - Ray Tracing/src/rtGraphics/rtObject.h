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

		virtual float rayIntersect(rtVec3f p, rtVec3f d, rtVec3f* hitPos, rtVec3f* hitNormal) = 0;
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

		///Inherited Methods
		float rayIntersect(rtVec3f p, rtVec3f d, rtVec3f* hitPos, rtVec3f* hitNormal);
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

	///Inherited methods
	inline float rtSphere::rayIntersect(rtVec3f p, rtVec3f d, rtVec3f* hitPos, rtVec3f* hitNormal)
	{
		rtVec3f m = p - center;

		float dotProd = d.dot(m);
		float magM = m.magnitude();

		float discriminant = (dotProd*dotProd) - (magM*magM - radius * radius);
		float sqrtDisc = sqrt(discriminant);
		float tSub = -dotProd - sqrtDisc;
		float tAdd = -dotProd + sqrtDisc;

		float t = (tSub < tAdd) ? tSub : tAdd;

		hitPos = &(p + (d * t));
		hitNormal = &(*hitPos - center).getNormalized();

		return t;
	}


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
		float rayIntersect(rtVec3f p, rtVec3f d, rtVec3f* hitPos, rtVec3f* hitNormal);
	};

	///In-line method definitions
	//Getter & Setter
	inline rtMesh& rtMeshObject::getMesh()			{ return mesh; }
	inline void rtMeshObject::setMesh(rtMesh& mesh)	{ this->mesh = mesh; }

	///Inherited methods
	inline float rtMeshObject::rayIntersect(rtVec3f p, rtVec3f d, rtVec3f* hitPos, rtVec3f* hitNormal)
	{
		float tmin = INFINITY;
		int faceIndex;

		vecList& vertices = mesh.getVerts();
		intList& faces = mesh.getFaces();
		vecList& normals = mesh.getNormals();

		for (int faceIndex = 0; faceIndex < faces->size(); faceIndex++)
		{
			//Get the array of vertex indices for the given face
			array<int, 3>& face = faces->at(faceIndex);
			//Get the first vertex
			rtVec3f p0 = vertices->at(face.at(0));
			//Get the normal using the face index
			rtVec3f normal = normals->at(faceIndex);

			float k = p0.dot(normal);
			float t = (k - p.dot(normal)) / (d.dot(normal));

			//If the ray is not obscured, determine if the ray hit the triangle
			if (t < tmin)
			{
				//Calculate the intersection point using t
				rtVec3f r = p + (d*t);

				//Get the remaining two points
				rtVec3f p1 = vertices->at(face.at(1));
				rtVec3f p2 = vertices->at(face.at(2));

				//Find the edge vertices
				rtVec3f e0 = p0 - p1;
				rtVec3f e1 = p2 - p1;
				rtVec3f e2 = p0 - p2;

				float result1 = (e0.getCrossed(r - p0)).dot(normal);
				float result2 = (e1.getCrossed(r - p0)).dot(normal);
				float result3 = (e2.getCrossed(r - p0)).dot(normal);

				if (result1 > 0 && result2 > 0 && result3 > 0)
				{
					tmin = t;
					hitPos = &r;
					hitNormal = &normal;
				}
			}
		}

		return 0.0f;
	}
}