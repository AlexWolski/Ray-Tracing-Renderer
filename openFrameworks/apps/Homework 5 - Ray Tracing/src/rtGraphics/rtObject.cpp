#include "rtObject.h"

namespace rtGraphics
{
	//Sphere Intersection
	float rtSphere::rayIntersect(rtVec3f P, rtVec3f D, shared_ptr<rtVec3f> hitPos, shared_ptr<rtVec3f> hitNormal, float nearClip, float farClip)
	{
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
			return -1.0f;

		//Compute the rest of the quadratic formula to find the intersection distance
		float sqrtDisc = sqrt(discriminant);
		float tSub = -dotProd - sqrtDisc;
		float tAdd = -dotProd + sqrtDisc;

		//Get the smaller of the two distances
		float t = (tSub < tAdd) ? tSub : tAdd;

		//Calculate and set the point of intersection and its normal
		*hitPos = P + (D * t);
		*hitNormal = *hitPos - center;
		hitNormal->normalize();

		//Return the intersection distance
		return t;
	}

	//Mesh Intersection
	float rtMeshObject::rayIntersect(rtVec3f P, rtVec3f D, shared_ptr<rtVec3f> hitPos, shared_ptr<rtVec3f> hitNormal, float nearClip, float farClip)
	{
		float t;
		float tmin = farClip;
		int faceIndex;

		vecList& vertices = mesh.getVerts();
		intList& faces = mesh.getFaces();
		vecList& normals = mesh.getNormals();

		//Iterate over all the triangles in the mesh
		for (int faceIndex = 0; faceIndex < faces->size(); faceIndex++)
		{
			//Get the array of vertex indices for the given face
			array<int, 3>& face = faces->at(faceIndex);
			//Get the first vertex
			rtVec3f p0 = vertices->at(face.at(0));
			//Get the normal using the face index
			rtVec3f normal = normals->at(faceIndex);

			//Calculate the plane constant
			float k = p0.dot(normal);
			//Calculate the plane intersection point
			t = (k - P.dot(normal)) / (D.dot(normal));

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
					tmin = t;
					*hitPos = r;
					*hitNormal = normal;
				}
			}
		}

		return tmin;
	}
}