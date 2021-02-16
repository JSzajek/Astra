#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#include "Vec2.h"
#include "Vec3.h"

namespace Astra::Math
{
	inline float BarryCentric(Vec3 p1, Vec3 p2, Vec3 p3, Vec2 pos)
	{
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

	static inline float Random()
	{
		return static_cast <float> (rand()) / RAND_MAX;
	}

	static inline int RandomRange(int min, int max)
	{
		return rand() % (max - min + 1) + min;
	}

	static inline int DecimalNumbers(float number, unsigned int precision) {
		float trunc = number - static_cast<int>(number);
		return static_cast<int>(trunc * pow(10, precision));
	}
}