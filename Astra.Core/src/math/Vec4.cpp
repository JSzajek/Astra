#include "Vec4.h"

namespace Astra::Math
{
	Vec4::Vec4(const float& _x, const float& _y, const float& _z, const float& _w)
		: x(_x), y(_y), z(_z), w(_w)
	{
	}

	Vec4::Vec4(const Vec4& other)
		: x(other.x), y(other.y), z(other.z), w(other.w)
	{
	}

	float& Vec4::operator[](int index)
	{
		return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
	}

	Vec4& Vec4::Add(const Vec4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	Vec4& Vec4::Subtract(const Vec4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	Vec4& Vec4::Multiply(const Vec4& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}

	Vec4& Vec4::Multiply(const float& scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	Vec4& Vec4::Divide(const Vec4& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}

	Vec4& Vec4::Divide(const float& scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	Vec4& operator+(Vec4 left, const Vec4& right)
	{
		return left.Add(right);
	}

	Vec4& operator-(Vec4 left, const Vec4& right)
	{
		return left.Subtract(right);
	}

	Vec4& operator*(Vec4 left, const Vec4& right)
	{
		return left.Multiply(right);
	}

	Vec4& operator*(Vec4 left, const float& scalar)
	{
		return left.Multiply(scalar);
	}

	Vec4& operator/(Vec4 left, const Vec4& right)
	{
		return left.Divide(right);
	}

	Vec4& operator/(Vec4 left, const float& scalar)
	{
		return left.Divide(scalar);
	}

	Vec4& Vec4::operator+=(const Vec4& other)
	{
		return Add(other);
	}

	Vec4& Vec4::operator-=(const Vec4& other)
	{
		return Subtract(other);
	}

	Vec4& Vec4::operator*=(const Vec4& other)
	{
		return Multiply(other);
	}

	Vec4& Vec4::operator*=(const float& scalar)
	{
		return Multiply(scalar);
	}

	Vec4& Vec4::operator/=(const Vec4& other)
	{
		return Divide(other);
	}

	Vec4& Vec4::operator/=(const float& scalar)
	{
		return Divide(scalar);
	}

	bool Vec4::operator==(const Vec4& other) const
	{
		return abs(x - other.x) < FLT_EPSILON &&
			   abs(y - other.y) < FLT_EPSILON &&
			   abs(z - other.z) < FLT_EPSILON &&
			   abs(w - other.w) < FLT_EPSILON;
	}

	bool Vec4::operator!=(const Vec4& other) const
	{
		return !(*this == other);
	}

	float Vec4::Dot(const Vec4& other) const
	{
		return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
	}

	float Vec4::DistanceTo(const Vec4& other) const
	{
		return sqrtf(powf(other.x - x, 2) + powf(other.y - y, 2) + powf(other.z - z, 2) + powf(other.w - w, 2));
	}

	float Vec4::ManhattenDistanceTo(const Vec4& other) const
	{
		return abs(x - other.x) + abs(y - other.y) + abs(z - other.z) + abs(w - other.w);
	}

	float Vec4::Magnitude() const
	{
		return sqrtf((x * x) + (y * y) + (z * z) + (w * w));
	}

	void Vec4::Normalize()
	{
		float norm = Magnitude();
		if (norm <= 0) { return; }
		x /= norm;
		y /= norm;
		z /= norm;
		w /= norm;
	}

	float Vec4::DirectionTo(const Vec4& other) const
	{
		float norm = Magnitude() * other.Magnitude();
		if (norm <= 0)
		{
			return FLT_EPSILON;
		}
		return acosf(Dot(other) / norm);
	}
}