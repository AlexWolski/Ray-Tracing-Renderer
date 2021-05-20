#pragma once

#include <memory>
#include <vector>
#include <utility>
#include "Data Classes/rtBoundingBox.h"

using namespace std;

namespace rtGraphics
{
	template <class T>
	class BVH
	{
	public:
		typedef vector<pair<rtBoundingBox, T>> primitiveInfo;

	private:
		//A private, nested node class to form the binary tree
		struct ObjectNode
		{
			rtBoundingBox boundingBox;
			T object;
			shared_ptr<ObjectNode> left, right;
		};

		//A list of primitive objects to store and their bounding boxes
		primitiveInfo primitives;
		//The centroids of the bounding boxes
		vector<rtVec3f> centroids;
		//The root of the binary object tree
		shared_ptr<ObjectNode> rootNode;

		void computeCentroids();

	public:
		BVH() {};
		BVH(primitiveInfo primitives);

		void construct(primitiveInfo primitives);
	};


	//Constructor
	template <class T>
	inline BVH<T>::BVH(primitiveInfo primitives)
	{
		construct(primitives);
	}

	//Construct the tree given a list of primitives and their bounding boxes
	template <class T>
	inline void BVH<T>::construct(primitiveInfo primitives)
	{
		computeCentroids();
	}

	//Compute the centroid of each bounding box
	template <class T>
	inline void BVH<T>::computeCentroids()
	{
		centroids.clear();

		for (pair<rtBoundingBox, T> primitive : primitives)
		{
			rtBoundingBox boundingBox = primitive.first;

			rtVec3f boxSize = boundingBox.getMax() - boundingBox.getMin();
			rtVec3f centroid = boundingBox.getMin() + (boxSize * 0.5f);

			centroids.push_back(centroid);
		}
	}
}