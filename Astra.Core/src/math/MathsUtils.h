#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

namespace Astra::Math
{
	inline float ToRadians(float degrees)
	{
		return degrees * (float)M_PI / 180.0f;
	}

	inline float ToDegrees(float radians)
	{
		return radians * 180.0f / (float)M_PI;
	}

	template <typename T> void Clamp(T& value, const T& low, const T& high)
	{
		value = value < low ? low : (value > high ? high : value);
	}
}