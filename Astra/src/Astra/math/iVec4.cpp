#include "astra_pch.h"

#include "iVec4.h"

namespace Astra::Math
{
	const iVec4 iVec4::X_Axis = iVec4(1, 0, 0, 0);
	const iVec4 iVec4::Y_Axis = iVec4(0, 1, 0, 0);
	const iVec4 iVec4::Z_Axis = iVec4(0, 0, 1, 0);
	const iVec4 iVec4::W_Axis = iVec4(0, 0, 0, 1);
	const iVec4 iVec4::Zero	  = iVec4(0);
	const iVec4 iVec4::One    = iVec4(1);

	iVec4::iVec4()
		: x(0), y(0), z(0), w(0)
	{
	}

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

	iVec4 iVec4::operator+(const iVec4& r_val) const
	{
		return iVec4(x + r_val.x, y + r_val.y, z + r_val.z, w + r_val.w);
	}

	void iVec4::operator+=(const iVec4& r_val)
	{
		x += r_val.x;
		y += r_val.y;
		z += r_val.z;
		w += r_val.w;
	}

	iVec4 iVec4::operator-(const iVec4& r_val) const
	{
		return iVec4(x - r_val.x, y - r_val.y, z - r_val.z, w - r_val.w);
	}

	void iVec4::operator-=(const iVec4& r_val)
	{
		x -= r_val.x;
		y -= r_val.y;
		z -= r_val.z;
		w -= r_val.w;
	}

	iVec4 iVec4::operator*(const iVec4& r_val) const
	{
		return iVec4(x * r_val.x, y * r_val.y, z * r_val.z, w * r_val.w);
	}

	iVec4 iVec4::operator*(const int r_val) const
	{
		return iVec4(x * r_val, y * r_val, z * r_val, w * r_val);
	}

	void iVec4::operator*=(const iVec4& r_val)
	{
		x *= r_val.x;
		y *= r_val.y;
		z *= r_val.z;
		w *= r_val.w;
	}

	void iVec4::operator*=(const int r_val)
	{
		x *= r_val;
		y *= r_val;
		z *= r_val;
		w *= r_val;
	}

	iVec4 iVec4::operator/(const iVec4& r_val) const
	{
		return iVec4(x / r_val.x, y / r_val.y, z / r_val.z, w / r_val.w);
	}

	iVec4 iVec4::operator/(const int r_val) const
	{
		return iVec4(x / r_val, y / r_val, z / r_val, w / r_val);
	}

	void iVec4::operator/=(const iVec4& r_val)
	{
		x /= r_val.x;
		y /= r_val.y;
		z /= r_val.z;
		w /= r_val.w;
	}

	void iVec4::operator/=(const int r_val)
	{
		x /= r_val;
		y /= r_val;
		z /= r_val;
		w /= r_val;
	}

	iVec4 iVec4::operator-() const
	{
		return iVec4(-x, -y, -z, -w);
	}

	bool iVec4::operator==(const iVec4& r_val) const
	{
		return x == r_val.x && y == r_val.y && z == r_val.z && w == r_val.w;
	}

	bool iVec4::operator!=(const iVec4& r_val) const
	{
		return x != r_val.x || y != r_val.y || z != r_val.z || w != r_val.w;
	}

	bool iVec4::operator<(const iVec4& r_val) const
	{
		if (x == r_val.x)
		{
			if (y == r_val.y)
			{
				if (z == r_val.z)
				{
					return w < r_val.w;
				}
				return z < r_val.z;
			}
			return y < r_val.y;
		}
		return x < r_val.x;
	}

	bool iVec4::operator>(const iVec4& r_val) const
	{
		if (x == r_val.x)
		{
			if (y == r_val.y)
			{
				if (z == r_val.z)
				{
					return w > r_val.w;
				}
				return z > r_val.z;
			}
			return y > r_val.y;
		}
		return x > r_val.x;
	}

	bool iVec4::operator<=(const iVec4& r_val) const
	{
		if (x == r_val.x)
		{
			if (y == r_val.y)
			{
				if (z == r_val.z)
				{
					return w <= r_val.w;
				}
				return z <= r_val.z;
			}
			return y < r_val.y;
		}
		return x < r_val.x;
	}

	bool iVec4::operator>=(const iVec4& r_val) const
	{
		if (x == r_val.x)
		{
			if (y == r_val.y)
			{
				if (z == r_val.z)
				{
					return w >= r_val.w;
				}
				return z >= r_val.z;
			}
			return y > r_val.y;
		}
		return x > r_val.x;
	}

	float iVec4::Length() const
	{
		return static_cast<float>(std::sqrt((x * x) + (y * y) + (z * z) + (w * w)));
	}

	int iVec4::LengthSquared() const
	{
		return (x * x) + (y * y) + (z * z) + (w * w);
	}

	void iVec4::SetAxis(int axis, int val)
	{
		ASTRA_ASSERT(axis < 4, "iVec4: Out Of Index Axis Access.");
		m_data[axis] = val;
	}

	int iVec4::GetAxis(int axis) const
	{
		ASTRA_ASSERT(axis < 4, "iVec4: Out Of Index Axis Access.");
		return operator[](axis);
	}

	int iVec4::MinAxis() const
	{
		// TODO Implement
		//return x < y ? (x < z ? 0 : 2) : (y < z ? 1 : 2);
		return 0;
	}

	int iVec4::MaxAxis() const
	{
		// TODO Implement
		//return x < y ? (y < z ? 2 : 1) : (x < z ? 2 : 0);
		return 0;
	}

	float iVec4::DistanceTo(const iVec4& r_val) const
	{
		return (r_val - *this).Length();
	}

	int iVec4::DistanceSquaredTo(const iVec4& r_val) const
	{
		return (r_val - *this).LengthSquared();
	}

	float iVec4::AngleTo(const iVec4& r_val) const
	{
		return std::atan2f(Cross(r_val).Length(), static_cast<float>(Dot(r_val)));
	}

	// Not correct?
	int iVec4::Dot(const iVec4& r_val) const
	{
		return x * r_val.x + y * r_val.y + z * r_val.z + w * r_val.w;
	}

	iVec4 iVec4::Cross(const iVec4& r_val) const
	{
		return iVec4((y * r_val.z) - (z * r_val.y),
					 (z * r_val.x) - (x * r_val.z),
					 (x * r_val.y) - (y * r_val.x), 1);
	}
}