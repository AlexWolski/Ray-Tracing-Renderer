#pragma once

#include <algorithm>
#include "ofMain.h"

namespace rtGraphics
{
	class rtScene
	{
		vector<ofLight> lightList;
		vector<of3dPrimitive> objectList;
	};
}