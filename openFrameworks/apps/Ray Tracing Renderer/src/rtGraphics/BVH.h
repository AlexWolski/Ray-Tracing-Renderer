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
			shared_ptr<ObjectNode> left, right = nullptr;
		};

		//A list of primitive objects to store and their bounding boxes
		primitiveList primitives;
		//The root of the binary object tree
		shared_ptr<ObjectNode> rootNode;

		rtVec3f computeCentroid(rtBoundingBox boundingBox);

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
		//Calculate the bounding box that encapsulates all the primitives
		rtBoundingBox rootBoundingBox = encapsulae(primitives);

		//Create the root node
		rootNode.reset();
		rootNode = make_shared<ObjectNode>();
		rootNode->boundingBox = rootBoundingBox;

		//Recursively build the BVH
		branchNode(rootNode, primitives);
	}

	//Compute the centroid of a bounding box
	template <class T>
	inline rtVec3f BVH<T>::computeCentroid(rtBoundingBox boundingBox)
	{
		rtVec3f boxSize = boundingBox.getMax() - boundingBox.getMin();
		rtVec3f centroid = boundingBox.getMin() + (boxSize * 0.5f);

		return centroid;
	}

	//A helper function that splits a group of primitives into two children nodes
	template <class T>
	inline void BVH<T>::branchNode(shared_ptr<ObjectNode> subTreeRoot, primitiveList primitives)
	{
		//If only one primitive remains, this is a leaf node
		if (primitives.size() == 1)
		{
			primitivePair pair = primitives[0];
			subTreeRoot->boundingBox = pair.first;
			subTreeRoot->object = pair.second;

			return;
		}
		//If there are two or more nodes remaining, recursively split them
		else
		{
			//Create a bounding box for the primitives
			rtBoundingBox boundingBox = encapsulae(primitives);
			subTreeRoot->boundingBox = boundingBox;
			//Determine the axis to split on
			int splitAxis = getLongestAxis(boundingBox);

			//Determine the threshold value between the two groups
			float min = boundingBox.getMin()[splitAxis];
			float max = boundingBox.getMax()[splitAxis];
			float threshold = min + (max - min) * 0.5;

			primitiveList leftGroup;
			primitiveList rightGroup;

			//Split all primitives into either the left group or right group
			for (int primitiveIndex = 0; primitiveIndex < primitives.size(); primitiveIndex++)
			{
				primitivePair pair = primitives.at(primitiveIndex);
				rtVec3f centroid = computeCentroid(pair.first);

				if (centroid[splitAxis] < threshold)
					leftGroup.push_back(pair);
				else
					rightGroup.push_back(pair);
			}

			//If one group is empty, forcibly split the groups
			if (leftGroup.size() == 0)
			{
				for (int index = 0; index < rightGroup.size() / 2; index++)
				{
					leftGroup.push_back(rightGroup.back());
					rightGroup.pop_back();
				}
			}

			if (rightGroup.size() == 0)
			{
				for (int index = 0; index < leftGroup.size() / 2; index++)
				{
					rightGroup.push_back(leftGroup.back());
					leftGroup.pop_back();
				}
			}

			//Recursively generate the rest of the tree
			if (leftGroup.size() > 0)
			{
				subTreeRoot->left = make_shared<ObjectNode>();
				branchNode(subTreeRoot->left, leftGroup);
			}

			if (rightGroup.size() > 0)
			{
				subTreeRoot->right = make_shared<ObjectNode>();
				branchNode(subTreeRoot->right, rightGroup);
			}
		}
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
			rtVec3f minBB = boundingBox.getMin();
			rtVec3f maxBB = boundingBox.getMax();

			//Update the maximum and minimum points
			for (int axis = 0; axis < 3; axis++)
			{
				if (minBB[axis] < min[axis])
					min[axis] = minBB[axis];
				if (maxBB[axis] > max[axis])
					max[axis] = maxBB[axis];
			}
		}

		return rtBoundingBox(min, max);
	}

	//Finds the axis in which the given objects are the most spread out
	//TO-DO: Move to the bounding box class
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

		return maxAxis;
	}
}