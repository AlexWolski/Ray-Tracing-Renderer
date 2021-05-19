#pragma once

#include <vector>
#include <tuple>
#include "Data Classes/rtBoundingBox.h"

using namespace std;

namespace rtGraphics
{
	template <class T>
	class BVH
	{
	public:
		typedef vector<tuple<rtBoundingBox, T>> primitiveInfo

	public:
		BVH();
		BVH(primitiveInfo primitives);

		construct(primitiveInfo primitives);
	};
}