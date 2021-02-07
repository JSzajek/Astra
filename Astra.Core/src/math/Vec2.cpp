#include "Vec2.h"

namespace Astra::Math
{
	const Vec2 Vec2::X_Axis		= Vec2(1,0);
	const Vec2 Vec2::Y_Axis		= Vec2(0,1);
	const Vec2 Vec2::Zero		= Vec2(0);
	const Vec2 Vec2::One		= Vec2(1);
	const Vec2 Vec2::Left		= Vec2(-1,0);
	const Vec2 Vec2::Right		= Vec2(1,0);
	const Vec2 Vec2::Up			= Vec2(0,-1);
	const Vec2 Vec2::Down		= Vec2(0,1);

	Vec2::Vec2()
		: x(0), y(0)
	{
	}

	Vec2::Vec2(float _x, float _y)
		: x(_x), y(_y)
	{
	}

	Vec2::Vec2(float _value)
		: x(_value), y(_value)
	{
	}

	Vec2::Vec2(const Vec2& other)
		: x(other.x), y(other.y)
	{
	}

	float& Vec2::operator[](int index)
	{
		return index == 0 ? x : y;
	}

	Vec2& Vec2::Add(const Vec2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2& Vec2::Subtract(const Vec2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vec2 Vec2::Multiply(const Vec2& other)
	{
		return Vec2(x * other.x, y * other.y);
	}

	Vec2& Vec2::Multiply(const float& scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vec2& Vec2::Divide(const Vec2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	Vec2& Vec2::Divide(const float& scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	Vec2& operator+(Vec2 left, const Vec2& right)
	{
		return left.Add(right);
	}

	Vec2& operator-(Vec2 left, const Vec2& right)
	{
		return left.Subtract(right);
	}

	Vec2 operator*(Vec2 left, const Vec2& right)
	{
		return left.Multiply(right);
	}

	Vec2& operator*(Vec2 left, const float& scalar)
	{
		return left.Multiply(scalar);
	}

	Vec2& operator/(Vec2 left, const Vec2& right)
	{
		return left.Divide(right);
	}

	Vec2& operator/(Vec2 left, const float& scalar)
	{
		return left.Divide(scalar);
	}

	Vec2& Vec2::operator+=(const Vec2& other)
	{
		return Add(other);
	}

	Vec2& Vec2::operator-=(const Vec2& other)
	{
		return Subtract(other);
	}

	Vec2 Vec2::operator*=(const Vec2& other)
	{
		return Multiply(other);
	}

	Vec2& Vec2::operator*=(const float& scalar)
	{
		return Multiply(scalar);
	}

	Vec2& Vec2::operator/=(const Vec2& other)
	{
		return Divide(other);
	}

	Vec2& Vec2::operator/=(const float& scalar)
	{
		return Divide(scalar);
	}

	bool Vec2::operator==(const Vec2& other) const
	{
		return abs(x - other.x) < FLT_EPSILON &&
			   abs(y - other.y) < FLT_EPSILON;
	}

	bool Vec2::operator!=(const Vec2& other) const
	{
		return !(*this == other);
	}

	float Vec2::Dot(const Vec2& other) const
	{
		return (x * other.x) + (y * other.y);
	}

	float Vec2::DistanceTo(const Vec2& other) const
	{
		return sqrtf(powf(other.x - x, 2) + powf(other.y - y, 2));
	}

	float Vec2::ManhattenDistanceTo(const Vec2& other) const
	{
		return abs(x - other.x) + abs(y - other.y);
	}

	float Vec2::Magnitude() const
	{
		return sqrtf((x * x) + (y * y));
	}

	void Vec2::Normalize()
	{
		float norm = Magnitude();
		if (norm <= 0) { return; }
		x /= norm;
		y /= norm;
	}

	float Vec2::DirectionTo(const Vec2& other)
	{
		float norm = Magnitude() * other.Magnitude();
		if (norm <= 0)
		{
			return FLT_EPSILON;
		}
		return acosf(Dot(other) / norm);
	}
}