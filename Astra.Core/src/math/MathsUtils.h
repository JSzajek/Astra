#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

namespace Astra::Math
{
	inline float ToRadians(float degrees)
	{
		return degrees * (float)M_PI / 180.0f;
	}
}