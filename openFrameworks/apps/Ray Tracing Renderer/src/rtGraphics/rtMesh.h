#pragma once

#include <memory>
#include <vector>
#include <array>
#include "rtVec3f.h"

using namespace std;

namespace rtGraphics
{
	typedef shared_ptr<vector<rtVec3f>> vecList;
	typedef shared_ptr<vector<array<int, 3>>> intList;

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
		rtMesh(const rtVec3f vertices[], int numVerts, const array<int, 3> faces[], int numFaces);

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
	inline rtMesh::rtMesh()
	{
		vertices = make_shared<vector<rtVec3f>>();
		faces = make_shared<vector<array<int, 3>>>();
		normals = make_shared<vector<rtVec3f>>();

		updateNormals();
	}

	//Copy the vertices and faces, then calculate the normals
	inline rtMesh::rtMesh(const rtVec3f vertices[], int numVerts, const array<int, 3> faces[], int numFaces)
	{
		//Copy the vertices and faces using memcpy
		memcpy(&this->vertices->operator[](0), &vertices[0], numVerts * sizeof(vertices[0]));
		memcpy(&this->faces->operator[](0), &faces[0], numFaces * sizeof(faces[0]));
		//Initialize the normals vector
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
		normal.normalize();

		return normal;
	}

	//Calculates the normal of the given face index
	inline rtVec3f rtMesh::calculateNormal(int faceIndex)
	{
		//Get the array of vertex indices for the given face
		array<int, 3>& face = faces->at(faceIndex);

		//Get the values of the three vertices
		rtVec3f& vert0 = vertices->at(face.at(0));
		rtVec3f& vert1 = vertices->at(face.at(1));
		rtVec3f& vert2 = vertices->at(face.at(2));

		//Calculate and return the normal
		return calculateNormal(vert0, vert1, vert2);
	}

	//Updates the normal for each face
	inline void rtMesh::updateNormals()
	{
		//Loop through the face indices
		for (int faceIndex = 0; faceIndex < faces->size(); faceIndex++)
		{
			rtVec3f normal = calculateNormal(faceIndex);

			//If the normals vector is too small, populate it with dummy variables. Then add the new normal.
			if (normals->size() <= faceIndex)
			{
				while (normals->size() < faceIndex)
					normals->push_back(rtVec3f::zero);

				normals->push_back(normal);
			}
			//Otherwise just set the normal at the given index
			else
				normals->at(faceIndex) = normal;
		}
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
	inline void rtMesh::addFace(int index0, int index1, int index2)
	{
		//Create a new array in the heap to store the face indices
		array<int, 3> face = { index0, index1, index2 };
		//Push the array pointer onto the vector
		faces->push_back(face);
		//Get the index of the new face
		int faceIndex = faces->size() - 1;
		//Calculate the normal and store it
		normals->push_back(calculateNormal(faceIndex));
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