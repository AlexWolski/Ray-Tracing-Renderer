#pragma once

#include "../rtMesh.h"
#include "../rtObject.h"

namespace rtGraphics
{
	/*
	 * A mesh-based object that applies a transform to the mesh
	 * TO-DO: Replace the mesh member with a mesh name that
	 *        corresponds to a shared mesh in a data structure
	*/
	class meshObject : public rtObject
	{
	private:
		rtMesh mesh;

	public:
		///Constructors
		meshObject() : mesh(rtMesh()) {}
		meshObject(rtMesh& mesh) : mesh(mesh) {}
		meshObject(rtMesh& mesh, rtMat& material) : rtObject(material), mesh(mesh) {}

		///Getter & Setter
		rtMesh& getMesh();
		void setMesh(rtMesh& mesh);

		///Inherited Methods
		shared_ptr<rtRayHit> rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint = nullptr);
	};

	///In-line method definitions
	//Getter & Setter
	inline rtMesh& meshObject::getMesh() { return mesh; }
	inline void meshObject::setMesh(rtMesh& mesh) { this->mesh = mesh; }
}