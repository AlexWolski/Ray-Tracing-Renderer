#pragma once

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

	public:
		BVH();
		BVH(primitiveInfo primitives);

		void construct(primitiveInfo primitives);
	};

	template <class T>
	inline BVH<T>::BVH()
	{

	}

	template <class T>
	inline BVH<T>::BVH(primitiveInfo primitives)
	{
		construct(primitives);
	}

	template <class T>
	inline void BVH<T>::construct(primitiveInfo primitives)
	{

	}
}