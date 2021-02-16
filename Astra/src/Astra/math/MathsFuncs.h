#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#include "MathsDefines.h"

namespace Astra::Math
{
	static inline float PosModf(float x, float y)
	{
		float value = fmodf(x, y);
		if ((value < 0 && y > 0) || (value > 0 && y < 0))
		{
			value += y;
		}
		value += 0.0;
		return value;
	}

	static inline float Snapped(float val, float step)
	{
		if (step != 0)
		{
			val = std::floorf(val / step + 0.5f) * step;
		}
		return val;
	}

	static inline bool IsEqualApprox(float a, float b)
	{
		if (a == b)
		{
			return true;
		}
		float tol = CMP_EPSILON * std::abs(a);
		if (tol < CMP_EPSILON)
		{
			tol = CMP_EPSILON;
		}
		return std::abs(a - b) < tol;
	}

	static inline bool IsEqualApprox(float a, float b, float tolerance)
	{
		if (a == b)
		{
			return true;
		}
		return std::abs(a - b) < tolerance;
	}

	inline float ToRadians(float degrees)
	{
		return degrees * PI / 180.0f;
	}

	inline float ToDegrees(float radians)
	{
		return radians * 180.0f / PI;
	}

	template <typename T> void Clamp(T& value, const T& low, const T& high)
	{
		value = value < low ? low : (value > high ? high : value);
	}

	template <typename T> T Clamp(T value, const T& low, const T& high)
	{
		return value < low ? low : (value > high ? high : value);
	}
}