#pragma once

#include <memory>
#include <vector>
#include "rtNode.h"
#include "rtVec3f.h"
#include "rtMat.h"

using namespace std;

namespace rtGraphics
{
	/*
	 * A wrapper class for ray traceable objects
	 * TO-DO: Replace the material member with a material name that
	 *        corresponds to a shared material in a hash table
	 */
	class rtObject : rtNode
	{
	private:
		rtMat material;

	public:
		rtObject();
		rtObject(rtMat& material);
		rtMat& getMat();
		void setMat(const rtMat& material);
	};

	///Constructors
	rtObject::rtObject() : material(rtMat()) {}
	rtObject::rtObject(rtMat& material) : material(material) {}

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
	class rtSphere : rtObject
	{
	private:
		rtVec3f position;
		float radius;

	public:
		///Constructors
		rtSphere();
		rtSphere(rtMat& material);
		rtSphere(const rtVec3f& position, float radius);
		rtSphere(const rtVec3f& position, float radius, rtMat& material);

		///Getters
		rtVec3f getPosition() const;
		float getRadius() const;

		///Setters
		void setPosition(const rtVec3f& position);
		void setRadius(float radius);
	};

	///Constructors
	rtSphere::rtSphere() : rtObject()
	{
		position = rtVec3f::zero;
		radius = 1.0f;
	}

	rtSphere::rtSphere(rtMat& material) : rtObject(material)
	{
		position = rtVec3f::zero;
		radius = 1.0f;
	}

	rtSphere::rtSphere(const rtVec3f& position, float radius) :
		rtObject(),
		position(position),
		radius(radius) {
	}

	rtSphere::rtSphere(const rtVec3f& position, float radius, rtMat& material) :
		rtObject(material),
		position(position),
		radius(radius) {
	}

	///In-line method definitions
	//Getters
	inline rtVec3f rtSphere::getPosition() const	{ return position; }
	inline float rtSphere::getRadius() const		{ return radius; }
	
	//Setters
	void rtSphere::setPosition(const rtVec3f& position)	{ this->position = position;  }
	void rtSphere::setRadius(float radius)				{ this->radius = radius; }


	typedef shared_ptr<vector<rtVec3f>> vecList;

	/*
	 * A mesh object containing vertices and faces
	 * TO-DO: Create an encapsulating rtMeshObject class
	 *        whose instances can share meshes
	*/
	class rtMesh : rtObject
	{
	private:
		vecList vertices;
		vector<int[3]> faces;
		vecList normals;

		void calculateNormal(int index);
		void calculateNormals();

	public:
		///Constructors
		rtMesh();
		rtMesh(const rtVec3f vertices[], const rtVec3f indices[]);

		///Vertex Methods
		void addVert(const rtVec3f& vertex);
		vecList& getVerts();
		void clearVerts();

		///Index Methods
		void addFace(int index1, int index2, int index3);
		vector<int[3]>& getFaces();
		void clearFaces();

		///Normal Methods
		rtVec3f& getNormals();
	};
}