#pragma once

#include <memory>
#include <vector>
#include "rtNode.h"
#include "rtVec3f.h"
#include "rtMat.h"

using namespace std;

namespace rtGraphics
{
	class rtObject : rtNode
	{
	private:
		rtMat& material;

	public:
		rtMat& getMat();
		void setMat();
	};

	class rtSphere : rtObject
	{
	private:
		rtVec3f position;
		float radius;

	public:
		///Constructors
		rtSphere();
		rtSphere(const rtVec3f& position, float radius);

		///Getters
		rtVec3f getPosition() const;
		float getRadius() const;

		///Setters
		void setPosition(const rtVec3f& position);
		void setRadius(float radius);
	};

	typedef shared_ptr<vector<rtVec3f>> vecList;

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