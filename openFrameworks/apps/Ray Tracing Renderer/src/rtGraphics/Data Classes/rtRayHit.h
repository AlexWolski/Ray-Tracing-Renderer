#pragma once

#include "rtVec3f.h"
#include "../Objects/rtObject.h"

namespace rtGraphics
{
	//Forward declare rtObject to prevent a circular dependency
	class rtObject;

	//Stores the data associated with a ray cast hit
	struct rtRayHit
	{
		bool hit = false;
		rtObject* hitObject;
		float distance;
		rtVec3f hitPoint;
		rtVec3f hitNormal;
		//Only relevant for mesh objects
		int hitFaceIndex;
	};
}