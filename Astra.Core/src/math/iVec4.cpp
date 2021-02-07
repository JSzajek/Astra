#include "iVec4.h"

namespace Astra::Math
{
	const iVec4 iVec4::X_Axis = iVec4(1, 0, 0, 0);
	const iVec4 iVec4::Y_Axis = iVec4(0, 1, 0, 0);
	const iVec4 iVec4::Z_Axis = iVec4(0, 0, 1, 0);
	const iVec4 iVec4::W_Axis = iVec4(0, 0, 0, 1);
	const iVec4 iVec4::Zero = iVec4(0);
	const iVec4 iVec4::One = iVec4(1);
	const iVec4 iVec4::Left = iVec4(-1, 0, 0, 0);
	const iVec4 iVec4::Right = iVec4(1, 0, 0, 0);
	const iVec4 iVec4::Up = iVec4(0, 1, 0, 0);
	const iVec4 iVec4::Down = iVec4(0, -1, 0, 0);
	const iVec4 iVec4::Forward = iVec4(0, 0, -1, 0);
	const iVec4 iVec4::Back = iVec4(0, 0, 1, 0);

	iVec4::iVec4(int _x, int _y, int _z, int _w)
		: x(_x), y(_y), z(_z), w(_w)
	{
	}

	iVec4::iVec4(int _value)
		: x(_value), y(_value), z(_value), w(_value)
	{
	}

	iVec4::iVec4(const iVec2& first, const iVec2& second)
		: x(first.x), y(first.y), z(second.x), w(second.y)
	{
	}

	iVec4::iVec4(const iVec4& other)
		: x(other.x), y(other.y), z(other.z), w(other.w)
	{
	}

	int& iVec4::operator[](int index)
	{
		return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
	}

	const int iVec4::operator[](int index) const
	{
		return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
	}

	iVec4& iVec4::Add(const iVec4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	iVec4& iVec4::Subtract(const iVec4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	iVec4& iVec4::Multiply(const iVec4& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}

	iVec4& iVec4::Multiply(int scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	iVec4& iVec4::Divide(const iVec4& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}

	iVec4& iVec4::Divide(int scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	iVec4& operator+(iVec4 left, const iVec4& right)
	{
		return left.Add(right);
	}

	iVec4& operator-(iVec4 left, const iVec4& right)
	{
		return left.Subtract(right);
	}

	iVec4& operator*(iVec4 left, const iVec4& right)
	{
		return left.Multiply(right);
	}

	iVec4& operator*(iVec4 left, int scalar)
	{
		return left.Multiply(scalar);
	}

	iVec4& operator/(iVec4 left, const iVec4& right)
	{
		return left.Divide(right);
	}

	iVec4& operator/(iVec4 left, int scalar)
	{
		return left.Divide(scalar);
	}

	iVec4& iVec4::operator+=(const iVec4& other)
	{
		return Add(other);
	}

	iVec4& iVec4::operator-=(const iVec4& other)
	{
		return Subtract(other);
	}

	iVec4& iVec4::operator*=(const iVec4& other)
	{
		return Multiply(other);
	}

	iVec4& iVec4::operator*=(int scalar)
	{
		return Multiply(scalar);
	}

	iVec4& iVec4::operator/=(const iVec4& other)
	{
		return Divide(other);
	}

	iVec4& iVec4::operator/=(int scalar)
	{
		return Divide(scalar);
	}

	bool iVec4::operator==(const iVec4& other) const
	{
		return abs(x - other.x) < FLT_EPSILON &&
			abs(y - other.y) < FLT_EPSILON &&
			abs(z - other.z) < FLT_EPSILON &&
			abs(w - other.w) < FLT_EPSILON;
	}

	bool iVec4::operator!=(const iVec4& other) const
	{
		return !(*this == other);
	}

	int iVec4::Dot(const iVec4& other) const
	{
		return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
	}

	float iVec4::DistanceTo(const iVec4& other) const
	{
		return static_cast<float>(sqrt(pow(other.x - x, 2) + pow(other.y - y, 2) + pow(other.z - z, 2) + pow(other.w - w, 2)));
	}

	int iVec4::ManhattenDistanceTo(const iVec4& other) const
	{
		return abs(x - other.x) + abs(y - other.y) + abs(z - other.z) + abs(w - other.w);
	}

	float iVec4::Magnitude() const
	{
		return static_cast<float>(sqrt((x * x) + (y * y) + (z * z) + (w * w)));
	}

	void iVec4::Normalize()
	{
		float norm = Magnitude();
		if (norm <= 0) { return; }
		x = static_cast<int>(x / norm);
		y = static_cast<int>(y / norm);
		z = static_cast<int>(z / norm);
		w = static_cast<int>(w / norm);
	}

	float iVec4::DirectionTo(const iVec4& other) const
	{
		float norm = Magnitude() * other.Magnitude();
		if (norm <= 0)
		{
			return FLT_EPSILON;
		}
		return acosf(Dot(other) / norm);
	}
}