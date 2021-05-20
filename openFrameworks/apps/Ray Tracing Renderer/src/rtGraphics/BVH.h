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
		typedef pair<rtBoundingBox, T> primitivePair;
		typedef vector<primitivePair> primitiveList;

	private:
		//A private, nested node class to form the binary tree
		struct ObjectNode
		{
			rtBoundingBox boundingBox;
			T object;
			shared_ptr<ObjectNode> left, right;
		};

		//A list of primitive objects to store and their bounding boxes
		primitiveList primitives;
		//The centroids of the bounding boxes
		vector<rtVec3f> centroids;
		//The root of the binary object tree
		shared_ptr<ObjectNode> rootNode;

		void computeCentroids();

	public:
		BVH() {};
		BVH(primitiveList primitives);

		//Construct the tree given a list of primitives and their bounding boxes
		void construct(primitiveList primitives);
		//A helper function that splits a group of primitives into two children nodes
		void branchNode(shared_ptr<ObjectNode> subTreeRoot, primitiveList primitives);

		//Creates a bounding box that encapsulates the given primitives
		rtBoundingBox encapsulae(primitiveList primitives);
		//Find on which axis the bounding box is longest
		int getLongestAxis(rtBoundingBox boundingBox);
	};


	//Constructor
	template <class T>
	inline BVH<T>::BVH(primitiveList primitives)
	{
		construct(primitives);
	}

	//Construct the tree given a list of primitives and their bounding boxes
	template <class T>
	inline void BVH<T>::construct(primitiveList primitives)
	{
		computeCentroids();
	}

	//Compute the centroid of each bounding box
	template <class T>
	inline void BVH<T>::computeCentroids()
	{
		centroids.clear();

		for (primitivePair pair : primitives)
		{
			rtBoundingBox boundingBox = pair.first;

			rtVec3f boxSize = boundingBox.getMax() - boundingBox.getMin();
			rtVec3f centroid = boundingBox.getMin() + (boxSize * 0.5f);

			centroids.push_back(centroid);
		}
	}

	//A helper function that splits a group of primitives into two children nodes
	template <class T>
	inline void BVH<T>::branchNode(shared_ptr<ObjectNode> subTreeRoot, primitiveList primitives)
	{

	}

	//Creates a bounding box that encapsulates the given primitives
	//TO-DO: Move to the bounding box class
	template <class T>
	inline rtBoundingBox BVH<T>::encapsulae(primitiveList primitives)
	{
		rtVec3f min(INFINITY);
		rtVec3f max(-INFINITY);

		//Loop through all objects in the list
		for (primitivePair pair : primitives)
		{
			rtBoundingBox boundingBox = pair.first;

			//Update the maximum and minimum points
			for (int axis = 0; axis < 3; axis++)
			{
				if (boundingBox[axis] < min[axis])
					min[axis] = boundingBox[axis];
				if (boundingBox[axis] > max[axis])
					max[axis] = boundingBox[axis];
			}
		}

		return rtBoundingBox(min, max);
	}

	//Finds the axis in which the given objects are the most spread out
	template <class T>
	inline int BVH<T>::getLongestAxis(rtBoundingBox boundingBox)
	{
		//Get the dimensions of the bounding box
		rtVec3f dim = boundingBox.getMax() - boundingBox.getMin();

		int maxAxis = 0;
		float maxRange = 0.0f;

		//Determine which axis is longest
		for (int axis = 0; axis < 3; axis++)
		{
			if (dim[axis] > maxRange)
			{
				maxAxis = axis;
				maxRange = dim[axis];
			}
		}

		return maxRange;
	}
}