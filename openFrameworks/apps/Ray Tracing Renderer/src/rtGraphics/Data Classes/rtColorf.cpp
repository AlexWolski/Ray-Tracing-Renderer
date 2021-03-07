#include "rtColorf.h"

namespace rtGraphics
{
	const float rtColorf::maxValue = 1.0f;

	const rtColorf rtColorf::white(maxValue, maxValue, maxValue);
	const rtColorf rtColorf::grey(maxValue/2.0f, maxValue/2.0f, maxValue/2.0f);
	const rtColorf rtColorf::black(0.0f, 0.0f, 0.0f);
	const rtColorf rtColorf::red(maxValue, 0.0f, 0.0f);
	const rtColorf rtColorf::green(0.0f, maxValue, 0.0f);
	const rtColorf rtColorf::blue(0.0f, 0.0f, maxValue);
	const rtColorf rtColorf::yellow(maxValue, maxValue, 0.0f);
	const rtColorf rtColorf::cyan(0.0f, maxValue, maxValue);
	const rtColorf rtColorf::magenta(maxValue, 0.0f, maxValue);
}