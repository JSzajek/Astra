#include "Vec3.h"

namespace Astra::Math
{

	Vec3::Vec3()
		: x(0), y(0), z(0)
	{
	}

	Vec3::Vec3(const float& _x, const float& _y, const float& _z)
		: x(_x), y(_y), z(_z)
	{
	}

	Vec3::Vec3(const float& _value)
		: x(_value), y(_value), z(_value)
	{
	}

	Vec3::Vec3(const Vec2& _other, const float& _z)
		: x(_other.x), y(_other.y), z(_z)
	{
	}

	Vec3::Vec3(const Vec3& other)
		: x(other.x), y(other.y), z(other.z)
	{
	}

	float& Vec3::operator[](int index)
	{
		return index == 0 ? x : index == 1 ? y : z;
	}

	Vec3& Vec3::Add(const Vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vec3& Vec3::Subtract(const Vec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vec3& Vec3::Multiply(const Vec3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Vec3& Vec3::Multiply(const float& scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	Vec3& Vec3::Divide(const Vec3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	Vec3& Vec3::Divide(const float& scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	Vec3& operator+(Vec3 left, const Vec3& right)
	{
		return left.Add(right);
	}

	Vec3& operator-(Vec3 left, const Vec3& right)
	{
		return left.Subtract(right);
	}

	Vec3& operator*(Vec3 left, const Vec3& right)
	{
		return left.Multiply(right);
	}

	Vec3& operator*(Vec3 left, const float& scalar)
	{
		return left.Multiply(scalar);
	}

	Vec3& operator/(Vec3 left, const Vec3& right)
	{
		return left.Divide(right);
	}

	Vec3& operator/(Vec3 left, const float& scalar)
	{
		return left.Divide(scalar);
	}

	Vec3& Vec3::operator+=(const Vec3& other)
	{
		return Add(other);
	}

	Vec3& Vec3::operator-=(const Vec3& other)
	{
		return Subtract(other);
	}

	Vec3& Vec3::operator*=(const Vec3& other)
	{
		return Multiply(other);
	}

	Vec3& Vec3::operator*=(const float& scalar)
	{
		return Multiply(scalar);
	}

	Vec3& Vec3::operator/=(const Vec3& other)
	{
		return Divide(other);
	}

	Vec3& Vec3::operator/=(const float& scalar)
	{
		return Divide(scalar);
	}

	bool Vec3::operator==(const Vec3& other) const
	{
		return abs(x - other.x) < FLT_EPSILON &&
			   abs(y - other.y) < FLT_EPSILON &&
			   abs(z - other.z) < FLT_EPSILON;
	}

	bool Vec3::operator!=(const Vec3& other) const
	{
		return !(*this == other);
	}

	float Vec3::Dot(const Vec3& other) const
	{
		return (x * other.x) + (y * other.y) + (z * other.z);
	}

	float Vec3::DistanceTo(const Vec3& other) const
	{
		return sqrtf(powf(other.x - x, 2) + powf(other.y - y, 2) + powf(other.z - z, 2));
	}

	float Vec3::ManhattenDistanceTo(const Vec3& other) const
	{
		return abs(x - other.x) + abs(y - other.y) + abs(z - other.z);
	}

	float Vec3::Magnitude() const
	{
		return sqrtf((x * x) + (y * y) + (z * z));
	}

	void Vec3::Normalize()
	{
		float norm = Magnitude();
		if (norm <= 0) { return; }
		x /= norm;
		y /= norm;
		z /= norm;
	}

	float Vec3::DirectionTo(const Vec3& other) const
	{
		float norm = Magnitude() * other.Magnitude();
		if (norm <= 0)
		{
			return FLT_EPSILON;
		}
		return acosf(Dot(other) / norm);
	}

	Vec3 Vec3::Cross(const Vec3& other) const
	{
		return Vec3((y * other.z) - (z * other.y), (z * other.x) - (x * other.z), (x * other.y) - (y * other.x));
	}

	Vec2 Vec3::ToVec2() const
	{
		return Vec2(x, z);
	}
}