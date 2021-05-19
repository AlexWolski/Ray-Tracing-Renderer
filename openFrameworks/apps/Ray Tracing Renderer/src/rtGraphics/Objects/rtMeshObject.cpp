#include "rtMeshObject.h"

namespace rtGraphics
{
	//Ray-Mesh Intersection
	rtRayHit rtMeshObject::rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, rtRayHit originPoint)
	{
		//Create a struct to store the ray cast data.
		rtRayHit hitData;
		//By default, set the hit flag to false
		hitData.hit = false;

		//Determines if the ray origin is on the surface of this mesh
		bool onSurface = false;
		//The face that the ray origin is on
		int sourceFace;

		//If the ray origin is on the surface of this mesh, save the source face index
		if (originPoint.hitObject == this)
		{
			onSurface = true;
			sourceFace = originPoint.hitFaceIndex;
		}

		//The distance to the closest intersection point
		float tmin = farClip;
		//The index of the current triangle
		int faceIndex;

		//References to the vertex, face, and normal lists.
		vecList& vertices = mesh.getVerts();
		intList& faces = mesh.getFaces();
		vecList& normals = mesh.getNormals();

		//Iterate over all the triangles in the mesh
		for (int faceIndex = 0; faceIndex < faces->size(); faceIndex++)
		{
			//If the ray origin is on the current face, then it does not intersect
			if (onSurface && sourceFace == faceIndex)
				continue;

			//Get the array of vertex indices for the given face
			array<int, 3>& face = faces->at(faceIndex);
			//Get the first vertex
			rtVec3f p0 = vertices->at(face.at(0));
			//Get the normal using the face index
			rtVec3f normal = normals->at(faceIndex);

			//Calculate the plane constant
			float k = p0.dot(normal);
			//Calculate the distance to the plane intersection point
			float t = (k - P.dot(normal)) / (D.dot(normal));

			//If the ray is visible, determine if the ray hit the triangle
			if (t > nearClip && t < tmin)
			{
				//Calculate the intersection point using t
				rtVec3f r = P + (D * t);

				//Get the remaining two points
				rtVec3f p1 = vertices->at(face.at(1));
				rtVec3f p2 = vertices->at(face.at(2));

				//Find the edge vertices
				rtVec3f e0 = p1 - p0;
				rtVec3f e1 = p2 - p1;
				rtVec3f e2 = p0 - p2;

				//If the hit point is on the inside of each edge vector, it is inside the triangle
				if ((e0.getCrossed(r - p0)).dot(normal) >= 0 &&
					(e1.getCrossed(r - p1)).dot(normal) >= 0 &&
					(e2.getCrossed(r - p2)).dot(normal) >= 0)
				{
					//Update the distance of the closest intersection
					tmin = t;

					//Store the hit data into the struct
					hitData.hit = true;
					hitData.hitObject = this;
					hitData.distance = tmin;
					hitData.hitPoint = r;
					hitData.hitNormal = normal;
					hitData.hitFaceIndex = faceIndex;
				}
			}
		}

		//Return the intersection data
		return hitData;
	}


	//Mesh signed distance function
	rtRayHit rtMeshObject::sdf(rtVec3f P)
	{
		//To-do: Implement a triangle sdf method
		rtRayHit distData;
		distData.hit = false;
		distData.distance = INFINITY;

		return distData;
	}
}