#pragma once

#include <stdlib.h>
#include "rtObject.h"
#include "rtMesh.h"
#include "../BVH.h"

namespace rtGraphics
{
	/*
	 * A mesh-based object that applies a transform to the mesh
	 * TO-DO: Replace the mesh member with a mesh name that
	 *        corresponds to a shared mesh in a data structure
	*/
	class rtMeshObject : public rtObject
	{
	private:
		//The raw mesh data
		rtMesh mesh;
		rtMesh::vecList vertices;
		rtMesh::intList faces;
		rtMesh::vecList normals;
		//A BVH containing each mesh face and its respective bounding box
		BVH<int> faceBVH;

		//Create a BVH for the current mesh
		void buildBVH();
		//Create a bounding box for the specified face
		rtBoundingBox createFaceBB(int faceIndex);

	public:
		///Constructors
		rtMeshObject();
		rtMeshObject(rtMesh& mesh);
		rtMeshObject(rtMesh& mesh, rtMat& material);

		///Getter & Setter
		rtMesh& getMesh();
		void setMesh(rtMesh& mesh);

		///Inherited Methods
		rtRayHit rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, rtRayHit originPoint);
		rtRayHit sdf(rtVec3f P);
	};
}