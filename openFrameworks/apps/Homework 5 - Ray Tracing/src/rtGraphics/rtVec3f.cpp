#include "rtVec3f.h"

namespace rtGraphics
{
	///Static Member definitions
	const rtVec3f rtVec3f::one(1.0f);
	const rtVec3f rtVec3f::zero(0.0f);
	const rtVec3f rtVec3f::up(0.0f, 1.0f, 0.0f);
	const rtVec3f rtVec3f::down(0.0f, -1.0f, 0.0f);
	const rtVec3f rtVec3f::forward(0.0f, 0.0f, 1.0f);
	const rtVec3f rtVec3f::back(0.0f, 0.0f, -1.0f);
	const rtVec3f rtVec3f::left(-1.0f, 0.0f, 0.0f);
	const rtVec3f rtVec3f::right(1.0f, 0.0f, 0.0f);
}