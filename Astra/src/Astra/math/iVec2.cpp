#include "astra_pch.h"

#include "iVec2.h"

namespace Astra::Math
{
	const iVec2 iVec2::X_Axis	= iVec2(1,  0);
	const iVec2 iVec2::Y_Axis	= iVec2(0,  1);
	const iVec2 iVec2::Zero		= iVec2(0);
	const iVec2 iVec2::One		= iVec2(1);

	iVec2::iVec2()
		: x(0), y(0)
	{
	}

	iVec2::iVec2(int _x, int _y)
		: x(_x), y(_y)
	{
	}

	iVec2::iVec2(int _value)
		: x(_value), y(_value)
	{
	}

	iVec2::iVec2(const iVec2& other)
		: x(other.x), y(other.y)
	{
	}

	int& iVec2::operator[](int index)
	{
		ASTRA_ASSERT(index < 2, "Vec2: Out Of Index Axis Access.");
		return index ? y : x;
	}

	const int iVec2::operator[](int index) const
	{
		ASTRA_ASSERT(index < 2, "Vec2: Out Of Index Axis Access.");
		return index ? y : x;
	}

	iVec2 iVec2::operator+(const iVec2& r_val) const
	{
		return iVec2(x + r_val.x, y + r_val.y);
	}

	void iVec2::operator+=(const iVec2& r_val)
	{
		x += r_val.x;
		y += r_val.y;
	}

	iVec2 iVec2::operator-(const iVec2& r_val) const
	{
		return iVec2(x - r_val.x, y - r_val.y);
	}

	void iVec2::operator-=(const iVec2& r_val)
	{
		x -= r_val.x;
		y -= r_val.y;
	}

	iVec2 iVec2::operator*(const iVec2& r_val) const
	{
		return iVec2(x * r_val.x, y * r_val.y);
	}

	iVec2 iVec2::operator*(const int r_val) const
	{
		return iVec2(x * r_val, y * r_val);
	}

	void iVec2::operator*=(const iVec2& r_val)
	{
		x *= r_val.x;
		y *= r_val.y;
	}

	void iVec2::operator*=(const int r_val)
	{
		x *= r_val;
		y *= r_val;
	}

	iVec2 iVec2::operator/(const iVec2& r_val) const
	{
		return iVec2(x / r_val.x, y / r_val.y);
	}

	iVec2 iVec2::operator/(const int r_val) const
	{
		return iVec2(x / r_val, y / r_val);
	}

	void iVec2::operator/=(const iVec2& r_val)
	{
		x /= r_val.x;
		y /= r_val.y;
	}

	void iVec2::operator/=(const int r_val)
	{
		x /= r_val;
		y /= r_val;
	}

	iVec2 iVec2::operator-() const
	{
		return iVec2(-x, -y);
	}

	bool iVec2::operator==(const iVec2& r_val) const
	{
		return x == r_val.x && y == r_val.y;
	}

	bool iVec2::operator!=(const iVec2& r_val) const
	{
		return x != r_val.x || y != r_val.y;
	}

	float iVec2::Angle() const
	{
		return static_cast<float>(std::atan2(y, x));
	}
}