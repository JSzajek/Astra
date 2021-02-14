#include "astra_pch.h"

#include "iVec2.h"

namespace Astra::Math
{
	const iVec2 iVec2::X_Axis = iVec2(1, 0);
	const iVec2 iVec2::Y_Axis = iVec2(0, 1);
	const iVec2 iVec2::Zero = iVec2(0);
	const iVec2 iVec2::One = iVec2(1);
	const iVec2 iVec2::Left = iVec2(-1, 0);
	const iVec2 iVec2::Right = iVec2(1, 0);
	const iVec2 iVec2::Up = iVec2(0, -1);
	const iVec2 iVec2::Down = iVec2(0, 1);

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
		return index == 0 ? x : y;
	}

	iVec2& iVec2::Add(const iVec2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	iVec2& iVec2::Subtract(const iVec2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	iVec2 iVec2::Multiply(const iVec2& other)
	{
		return iVec2(x * other.x, y * other.y);
	}

	iVec2& iVec2::Multiply(int scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	iVec2& iVec2::Divide(const iVec2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	iVec2& iVec2::Divide(int scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	iVec2& operator+(iVec2 left, const iVec2& right)
	{
		return left.Add(right);
	}

	iVec2& operator-(iVec2 left, const iVec2& right)
	{
		return left.Subtract(right);
	}

	iVec2 operator*(iVec2 left, const iVec2& right)
	{
		return left.Multiply(right);
	}

	iVec2& operator*(iVec2 left, int scalar)
	{
		return left.Multiply(scalar);
	}

	iVec2& operator/(iVec2 left, const iVec2& right)
	{
		return left.Divide(right);
	}

	iVec2& operator/(iVec2 left, int scalar)
	{
		return left.Divide(scalar);
	}

	iVec2& iVec2::operator+=(const iVec2& other)
	{
		return Add(other);
	}

	iVec2& iVec2::operator-=(const iVec2& other)
	{
		return Subtract(other);
	}

	iVec2 iVec2::operator*=(const iVec2& other)
	{
		return Multiply(other);
	}

	iVec2& iVec2::operator*=(int scalar)
	{
		return Multiply(scalar);
	}

	iVec2& iVec2::operator/=(const iVec2& other)
	{
		return Divide(other);
	}

	iVec2& iVec2::operator/=(int scalar)
	{
		return Divide(scalar);
	}

	bool iVec2::operator==(const iVec2& other) const
	{
		return abs(x - other.x) < FLT_EPSILON &&
			abs(y - other.y) < FLT_EPSILON;
	}

	bool iVec2::operator!=(const iVec2& other) const
	{
		return !(*this == other);
	}

	int iVec2::Dot(const iVec2& other) const
	{
		return (x * other.x) + (y * other.y);
	}

	float iVec2::DistanceTo(const iVec2& other) const
	{
		return static_cast<float>(sqrt(pow(other.x - x, 2) + pow(other.y - y, 2)));
	}

	int iVec2::ManhattenDistanceTo(const iVec2& other) const
	{
		return abs(x - other.x) + abs(y - other.y);
	}

	float iVec2::Magnitude() const
	{
		return static_cast<float>(sqrt((x * x) + (y * y)));
	}

	void iVec2::Normalize()
	{
		float norm = Magnitude();
		if (norm <= 0) { return; }
		x = static_cast<int>(x / norm);
		y = static_cast<int>(y / norm);
	}

	float iVec2::DirectionTo(const iVec2& other)
	{
		float norm = Magnitude() * other.Magnitude();
		if (norm <= 0)
		{
			return FLT_EPSILON;
		}
		return acosf(Dot(other) / norm);
	}
}