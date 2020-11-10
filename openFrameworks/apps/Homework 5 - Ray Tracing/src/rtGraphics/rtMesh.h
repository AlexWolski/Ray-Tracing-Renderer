#pragma once

#include <memory>
#include <vector>
#include "rtVec3f.h"

using namespace std;

namespace rtGraphics
{
	typedef shared_ptr<vector<rtVec3f>> vecList;

	//A mesh object containing vertices and faces
	class rtMesh
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