#pragma once
#include "rtObject.h"
#include "rtMesh.h"

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
		rtMesh mesh;
		rtMesh::vecList vertices;
		rtMesh::intList faces;
		rtMesh::vecList normals;

	public:
		///Constructors
		rtMeshObject()
		{
			setMesh(rtMesh());
		}

		rtMeshObject(rtMesh& mesh)
		{
			setMesh(mesh);
		}

		rtMeshObject(rtMesh& mesh, rtMat& material) : rtObject(material)
		{
			setMesh(mesh);
		}

		///Getter & Setter
		rtMesh& getMesh();
		void setMesh(rtMesh& mesh);

		///Inherited Methods
		rtRayHit rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, rtRayHit originPoint);
		rtRayHit sdf(rtVec3f P);
	};

	///In-line method definitions
	//Getter & Setter
	inline rtMesh& rtMeshObject::getMesh()
	{
		return mesh;
	}

	inline void rtMeshObject::setMesh(rtMesh& mesh)
	{
		this->mesh = mesh;
		vertices = mesh.getVerts();
		faces = mesh.getFaces();
		normals = mesh.getNormals();
	}
}