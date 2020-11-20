#include "rtObject.h"

namespace rtGraphics
{
	//Sphere Intersection
	shared_ptr<rtRayHit> rtSphere::rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint)
	{
		//Create a struct to store the ray cast data.
		shared_ptr<rtRayHit> hitData = make_shared<rtRayHit>();
		//By default, set the hit flag to false
		hitData->hit = false;

		//Define an intermediate variable M as the vector from the sphere center to the ray origin
		rtVec3f M = P - center;

		//Calculate the dot product D*M and the magnitude of M
		float dotProd = D.dot(M);
		//The square of the magnitude the M vector. We can find this by just dotting M with itself
		float squaredMagM = M.dot(M);

		//The squared magnitude must be positive
		if (squaredMagM < 0)
			squaredMagM = -squaredMagM;

		//Calculate the discriminant of the quadratic formula
		float discriminant = (dotProd*dotProd) - (squaredMagM - radius * radius);

		//If the discriminant is less than 0, then the ray doesn't intersect the sphere
		if (discriminant < 0.0f)
			return hitData;

		//Compute the rest of the quadratic formula to find the intersection distance
		float sqrtDisc = sqrt(discriminant);
		float tSub = -dotProd - sqrtDisc;
		float tAdd = -dotProd + sqrtDisc;

		//The minimum positive intersection distance
		float t;
		
		//If the ray is on the object surface, ignore the intersection at that point
		if (originPoint != nullptr && originPoint->hitObject == this)
		{
			//Get the absolute value of each intersection distance
			float tSubAbs = abs(tSub);
			float tAddAbs = abs(tAdd);

			//If the ray it pointing outwards from the sphere, then it does not intersect
			if (tSubAbs > tAddAbs)
				return hitData;
			//If the ray is pointing in towards the sphere, use the intersection point on the other side
			else
				t = tAdd;
		}
		//If the ray is not on the object surface, use the minimum intersection distance
		else
		{
			//Use the minimum positive intersection distance
			if (tSub < 0.0f)
				t = tAdd;
			else
				t = tSub;
		}

		//If the  intersection point is still behind the ray origin, then the ray does not intersect
		if (t < 0.0f)
			return hitData;

		//Calculate the point of intersection and the normal at that point
		rtVec3f hitPoint = P + (D * t);
		rtVec3f hitNormal = hitPoint - center;
		hitNormal.normalize();

		//Store the hit data into the struct
		hitData->hit = true;
		hitData->hitObject = this;
		hitData->distance = t;
		hitData->hitPoint = hitPoint;
		hitData->hitNormal = hitNormal;

		//Return the intersection data
		return hitData;
	}

	//Mesh Intersection
	shared_ptr<rtRayHit> rtMeshObject::rayIntersect(rtVec3f P, rtVec3f D, float nearClip, float farClip, shared_ptr<rtRayHit> originPoint)
	{
		//Create a struct to store the ray cast data.
		shared_ptr<rtRayHit> hitData = make_shared<rtRayHit>();
		//By default, set the hit flag to false
		hitData->hit = false;

		//Determines if the ray origin is on the surface of this mesh
		bool onSurface = false;
		//The face that the ray origin is on
		int sourceFace;

		//If the ray origin is on the surface of this mesh, save the source face index
		if (originPoint != nullptr && originPoint->hitObject == this)
		{
			onSurface = true;
			sourceFace = originPoint->hitFaceIndex;
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
				rtVec3f r = P + (D*t);

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
					hitData->hit = true;
					hitData->hitObject = this;
					hitData->distance = tmin;
					hitData->hitPoint = r;
					hitData->hitNormal = normal;
					hitData->hitFaceIndex = faceIndex;
				}
			}
		}

		//Return the intersection data
		return hitData;
	}
}