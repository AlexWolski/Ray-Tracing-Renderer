#pragma once

#include <memory>
#include <vector>
#include "rtVec3f.h"

using namespace std;

namespace rtGraphics
{
	typedef shared_ptr<vector<rtVec3f>> vecList;
	typedef shared_ptr<vector<int[3]>> intList;

	//A mesh object containing vertices and faces
	class rtMesh
	{
	private:
		vecList vertices;
		//The faces and normals vectors should always be the same size
		intList faces;
		vecList normals;

		///Normal Methods
		rtVec3f calculateNormal(rtVec3f vert0, rtVec3f vert1, rtVec3f vert2);
		rtVec3f calculateNormal(int faceIndex);
		void updateNormals();

	public:
		///Constructors
		rtMesh();
		rtMesh(const rtVec3f vertices[], int numVerts, const rtVec3f faces[][3], int numFaces);

		///Vertex Methods
		void addVert(const rtVec3f& vertex);
		vecList getVerts();
		void clearVerts();

		///Face Methods
		void addFace(int index1, int index2, int index3);
		intList getFaces();
		void clearFaces();

		///Normal Methods
		vecList getNormals();
	};

	///Constructors
	//Initialize the data members and calculate the normals
	rtMesh::rtMesh()
	{
		vertices = make_shared<vector<rtVec3f>>();
		faces = make_shared<vector<int[3]>>();
		normals = make_shared<vector<rtVec3f>>();

		updateNormals();
	}

	//Copy the vertices and faces, then calculate the normals
	rtMesh::rtMesh(const rtVec3f vertices[], int numVerts, const rtVec3f faces[][3], int numFaces)
	{
		this->vertices = make_shared<vector<rtVec3f>>(vertices, numVerts);
		this->faces = make_shared<vector<int[3]>>(faces, numFaces);
		normals = make_shared<vector<rtVec3f>>();

		updateNormals();
	}

	///Normal Methods
	//Calculates the normal of the face defined by the given vertices
	inline rtVec3f rtMesh::calculateNormal(rtVec3f vert0, rtVec3f vert1, rtVec3f vert2)
	{
		//Calculate the edge vertices
		rtVec3f e0 = vert1 - vert0;
		rtVec3f e1 = vert2 - vert1;

		//Find the normal by crossing e0 and e1
		rtVec3f normal = e0.cross(e1);
		//Normalize the vector
		normal /= normal.magnitude();

		return normal;
	}

	//Calculates the normal of the given face index
	inline rtVec3f rtMesh::calculateNormal(int faceIndex)
	{
		//Get the array of vertex indices for the given face
		int* face = faces->operator[](faceIndex);

		//Get the values of the three vertices
		rtVec3f& vert0 = vertices->operator[](face[0]);
		rtVec3f& vert1 = vertices->operator[](face[1]);
		rtVec3f& vert2 = vertices->operator[](face[2]);

		//Calculate and return the normal
		return calculateNormal(vert0, vert1, vert2);
	}

	//Updates the normal for each face
	inline void rtMesh::updateNormals()
	{
		for (int faceIndex = 0; faceIndex < faces->size(); faceIndex++)
			normals->operator[](faceIndex) = calculateNormal(faceIndex);
	}

	inline vecList rtMesh::getNormals()
	{
		return normals;
	}

	///Vertex Methods
	inline void rtMesh::addVert(const rtVec3f& vertex)
	{
		vertices->push_back(vertex);
	}

	inline vecList rtMesh::getVerts()
	{
		return vertices;
	}

	/* Clear all of the vertices
	 * Also clear all the faces and normals, since they depend on the vertices
	 */
	inline void rtMesh::clearVerts()
	{
		vertices->clear();
		faces->clear();
		normals->clear();
	}

	///Face Methods
	inline void rtMesh::addFace(int index1, int index2, int index3)
	{
		faces->push_back({ index1, index2, index3 });
	}

	inline intList rtMesh::getFaces()
	{
		return faces;
	}

	//Clear all of the faces and the normals that depend on them
	inline void rtMesh::clearFaces()
	{
		faces->clear();
		normals->clear();
	}
}