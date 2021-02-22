#include "astra_pch.h"

#include "Vec3.h"

namespace Astra::Math
{
	const Vec3 Vec3::X_Axis		= Vec3(1,  0,  0);
	const Vec3 Vec3::Y_Axis		= Vec3(0,  1,  0);
	const Vec3 Vec3::Z_Axis		= Vec3(0,  0,  1);
	const Vec3 Vec3::Zero		= Vec3(0);
	const Vec3 Vec3::One		= Vec3(1);
	const Vec3 Vec3::Left		= Vec3(-1, 0,  0);
	const Vec3 Vec3::Right		= Vec3(1,  0,  0);
	const Vec3 Vec3::Up			= Vec3(0,  1,  0);
	const Vec3 Vec3::Down		= Vec3(0, -1,  0);
	const Vec3 Vec3::Forward	= Vec3(0,  0, -1);
	const Vec3 Vec3::Back		= Vec3(0,  0,  1);

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
		ASTRA_ASSERT(index < 3, "Vec3: Out Of Index Axis Access.");
		return m_data[index];
	}

	const float Vec3::operator[](int index) const
	{
		ASTRA_ASSERT(index < 3, "Vec3: Out Of Index Axis Access.");
		return m_data[index];
	}

	Vec3 Vec3::operator+(const Vec3& r_val) const
	{
		return Vec3(x + r_val.x, y + r_val.y, z + r_val.z);
	}

	void Vec3::operator+=(const Vec3& r_val)
	{
		x += r_val.x;
		y += r_val.y;
		z += r_val.z;
	}

	Vec3 Vec3::operator-(const Vec3& r_val) const
	{
		return Vec3(x - r_val.x, y - r_val.y, z - r_val.z);
	}

	void Vec3::operator-=(const Vec3& r_val)
	{
		x -= r_val.x;
		y -= r_val.y;
		z -= r_val.z;
	}

	Vec3 Vec3::operator*(const Vec3& r_val) const
	{
		return Vec3(x * r_val.x, y * r_val.y, z * r_val.z);
	}

	Vec3 Vec3::operator*(const float r_val) const
	{
		return Vec3(x * r_val, y * r_val, z * r_val);
	}

	Vec3 Vec3::operator*(const double r_val) const
	{
		return Vec3(static_cast<float>(x * r_val), static_cast<float>(y * r_val), static_cast<float>(z * r_val));
	}

	Vec3 Vec3::operator*(const int r_val) const
	{
		return Vec3(x * r_val, y * r_val, z * r_val);
	}

	void Vec3::operator*=(const Vec3& r_val)
	{
		x *= r_val.x;
		y *= r_val.y;
		z *= r_val.z;
	}

	void Vec3::operator*=(const float r_val)
	{
		x *= r_val;
		y *= r_val;
		z *= r_val;
	}

	void Vec3::operator*=(const double r_val)
	{
		x = static_cast<float>(x * r_val);
		y = static_cast<float>(y * r_val);
		z = static_cast<float>(z * r_val);
	}

	void Vec3::operator*=(const int r_val)
	{
		x *= r_val;
		y *= r_val;
		z *= r_val;
	}

	Vec3 Vec3::operator/(const Vec3& r_val) const
	{
		return Vec3(x / r_val.x, y / r_val.y, z / r_val.z);
	}

	Vec3 Vec3::operator/(const float r_val) const
	{
		return Vec3(x / r_val, y / r_val, z / r_val);
	}

	void Vec3::operator/=(const Vec3& r_val)
	{
		x /= r_val.x;
		y /= r_val.y;
		z /= r_val.z;
	}

	void Vec3::operator/=(const float r_val)
	{
		x /= r_val;
		y /= r_val;
		z /= r_val;
	}

	Vec3 Vec3::operator-() const
	{
		return Vec3(-x, -y, -z);
	}

	bool Vec3::operator==(const Vec3& r_val) const
	{
		return x == r_val.x && y == r_val.y && z == r_val.z;
	}

	bool Vec3::operator!=(const Vec3& r_val) const
	{
		return x != r_val.x || y != r_val.y || z != r_val.z;
	}

	bool Vec3::operator<(const Vec3& r_val) const 
	{
		if (x == r_val.x)
		{
			if (y == r_val.y)
			{
				return z < r_val.z;
			}
			return y < r_val.y;
		}
		return x < r_val.x;
	}

	bool Vec3::operator>(const Vec3& r_val) const 
	{
		if (x == r_val.x)
		{
			if (y == r_val.y)
			{
				return z > r_val.z;
			}
			return y > r_val.y;
		}
		return x > r_val.x;
	}

	bool Vec3::operator<=(const Vec3& r_val) const 
	{
		if (x == r_val.x)
		{
			if (y == r_val.y)
			{
				return z <= r_val.z;
			}
			return y < r_val.y;
		}
		return x < r_val.x;
	}

	bool Vec3::operator>=(const Vec3& r_val) const 
	{
		if (x == r_val.x)
		{
			if (y == r_val.y)
			{
				return z >= r_val.z;
			}
			return y > r_val.y;
		}
		return x > r_val.x;
	}

	void Vec3::Rotate(const Vec3& axis, const float phi)
	{
		ASTRA_ASSERT(!IsNormalized(), "Vec3: The Normal Vector Must be Normalized.");

		// TODO: Introduce concept of mat3
		Vec3 sqrAxis(axis.x * axis.x, axis.y * axis.y, axis.z * axis.z);
		float _cos = std::cosf(phi);
		Vec3 axis_1, axis_2, axis_3;
		axis_1[0] = sqrAxis.x + _cos * (1.0f - sqrAxis.x);
		axis_2[1] = sqrAxis.y + _cos * (1.0f - sqrAxis.y);
		axis_3[2] = sqrAxis.z + _cos * (1.0f - sqrAxis.z);

		float _sin = std::sinf(phi);
		float t = 1 - _cos;

		float xyzt = axis.x * axis.y * t;
		float zyxs = axis.z * _sin;
		axis_1[1] = xyzt - zyxs;
		axis_2[0] = xyzt + zyxs;

		xyzt = axis.x * axis.z * t;
		zyxs = axis.y * _sin;
		axis_1[2] = xyzt + zyxs;
		axis_3[0] = xyzt - zyxs;

		xyzt = axis.y * axis.z * t;
		zyxs = axis.x * _sin;
		axis_2[2] = xyzt - zyxs;
		axis_3[1] = xyzt + zyxs;

		x = axis_1.Dot(*this);
		y = axis_2.Dot(*this); 
		z = axis_3.Dot(*this);
	}

	Vec3 Vec3::Rotated(const Vec3& axis, const float val) const
	{
		Vec3 result = *this;
		result.Rotate(axis, val);
		return result;
	}

	Vec3 Vec3::Sign() const
	{
		return Vec3(SGN(x), SGN(y), SGN(z));
	}

	Vec3 Vec3::Floor() const
	{
		return Vec3(std::floorf(x), std::floorf(y), std::floorf(z));
	}

	Vec3 Vec3::Ceil() const
	{
		return Vec3(std::ceilf(x), std::ceilf(y), std::ceilf(z));
	}

	Vec3 Vec3::Round() const
	{
		return Vec3(std::roundf(x), std::roundf(y), std::roundf(z));
	}

	void Vec3::Snap(const Vec3& r_val)
	{
		x = Math::Snapped(x, r_val.x);
		y = Math::Snapped(y, r_val.y);
		z = Math::Snapped(z, r_val.z);
	}

	Vec3 Vec3::Snapped(const Vec3& r_val) const
	{
		return Vec3(Math::Snapped(x, r_val.x), Math::Snapped(y, r_val.y), Math::Snapped(z, r_val.z));
	}

	void Vec3::Normalize()
	{
		float len = x * x + y * y + z * z;
		if (len != 0)
		{
			len = std::sqrtf(len);
			x /= len;
			y /= len;
			z /= len;
		}
	}

	Vec3 Vec3::Normalized() const
	{
		Vec3 result = *this;
		result.Normalize();
		return result;
	}

	bool Vec3::IsNormalized() const
	{
		return Math::IsEqualApprox(LengthSquared(), 1.0f, UNIT_EPSILON);
	}

	float Vec3::Length() const
	{
		return std::sqrtf((x * x) + (y * y) + (z * z));
	}

	float Vec3::LengthSquared() const
	{
		return (x * x) + (y * y) + (z * z);
	}

	void Vec3::SetAxis(int axis, float val)
	{
		ASTRA_ASSERT(axis < 3, "Vec3: Out Of Index Axis Access.");
		m_data[axis] = val;
	}

	float Vec3::GetAxis(int axis) const
	{
		ASTRA_ASSERT(axis < 3, "Vec3: Out Of Index Axis Access.");
		return operator[](axis);
	}

	int Vec3::MinAxis() const
	{
		return x < y ? (x < z ? 0 : 2) : (y < z ? 1 : 2);
	}

	int Vec3::MaxAxis() const
	{
		return x < y ? (y < z ? 2 : 1) : (x < z ? 2 : 0);
	}

	Vec3 Vec3::Clamped(const float val) const
	{
		float len = Length();
		Vec3 temp = *this;
		if (len > 0 && val < len)
		{
			temp /= len;
			temp *= val;
		}
		return temp;
	}

	float Vec3::DistanceTo(const Vec3& r_val) const
	{
		return (r_val - *this).Length();
	}

	float Vec3::DistanceSquaredTo(const Vec3& r_val) const
	{
		return (r_val - *this).LengthSquared();
	}

	float Vec3::AngleTo(const Vec3& r_val) const
	{
		return std::atan2f(Cross(r_val).Length(), Dot(r_val));
	}

	Vec3 Vec3::DirectionTo(const Vec3& r_val) const
	{
		Vec3 result(r_val.x - x, r_val.y - y, r_val.z - z);
		result.Normalize();
		return result;
	}

	Vec3 Vec3::Mix(const Vec3& r_val, float weight)
	{
		return (*this * (1.0f - weight)) + (r_val * weight);
	}

	float Vec3::Dot(const Vec3& r_val) const
	{
		return x * r_val.x + y * r_val.y + z * r_val.z;
	}

	Vec3 Vec3::Cross(const Vec3& r_val) const
	{
		return Vec3((y * r_val.z) - (z * r_val.y),
					(z * r_val.x) - (x * r_val.z),
					(x * r_val.y) - (y * r_val.x));
	}

	Vec3 Vec3::PosMod(const Vec3& r_val) const
	{
		return Vec3(PosModf(x, r_val.x), PosModf(y, r_val.y), PosModf(z, r_val.z));
	}

	Vec3 Vec3::PosMod(const float r_val) const
	{
		return Vec3(PosModf(x, r_val), PosModf(y, r_val), PosModf(z, r_val));
	}

	Vec3 Vec3::Project(const Vec3& r_val) const
	{
		return r_val * (Dot(r_val) / r_val.LengthSquared());
	}

	Vec3 Vec3::PlaneProject(const float val, const Vec3& r_val) const
	{
		return r_val - *this * (Dot(r_val) - val);
	}

	Vec3 Vec3::Lerp(const Vec3& r_val, const float weight) const
	{
		return Vec3(x + (weight * (r_val.x - x)),
					y + (weight * (r_val.y - y)),
					z + (weight * (r_val.z - z)));
	}

	Vec3 Vec3::Slerp(const Vec3& r_val, const float weight) const
	{
		ASTRA_ASSERT(!IsNormalized(), "Vec3: The Normal Vector Must be Normalized.");
		float theta = AngleTo(r_val);
		return Rotated(Cross(r_val).Normalized(), theta * weight);
	}

	Vec3 Vec3::CubicInterpolate(const Vec3& val, const Vec3& pre, const Vec3& pos_val, const float weight) const
	{
		Vec3 p_0 = pre;
		Vec3 p_1 = *this;
		Vec3 p_2 = val;
		Vec3 p_3 = pos_val;

		float t = weight;
		float t_2 = t * t;
		float t_3 = t_2 * t;

		Vec3 result;
		result = 0.5f * ((p_1 * 2.0f) + (-p_0 + p_2) * t
						+ (2.0f * p_0 - 5.0 * p_1 + 4 * p_2 - p_3) * t_2
						+ (-p_0 + 3.0f * p_1 - 3.0f * p_2 + p_3) * t_3);
		return result;
	}

	Vec3 Vec3::MoveToward(const Vec3& val, const float delta) const
	{
		Vec3 vec = *this;
		Vec3 vec_dis = val - vec;
		float len = vec_dis.Length();
		return len <= delta || len < CMP_EPSILON ? val : vec + vec_dis / len * delta;
	}

	Vec3 Vec3::Slide(const Vec3& norm) const
	{
		ASTRA_ASSERT(!norm.IsNormalized(), "Vec3: The Normal Vector Must be Normalized.");
		return *this - norm * this->Dot(norm);
	}

	Vec3 Vec3::Bounce(const Vec3& norm) const
	{
		return -Reflect(norm);
	}

	Vec3 Vec3::Reflect(const Vec3& norm) const
	{
		ASTRA_ASSERT(!norm.IsNormalized(), "Vec3: The Normal Vector Must be Normalized.");
		return 2.0f * norm * this->Dot(norm) - *this;
	}

	bool Vec3::IsEqualApprox(const Vec3& val) const
	{
		return Math::IsEqualApprox(x, val.x) && Math::IsEqualApprox(y, val.y) && Math::IsEqualApprox(z, val.z);
	}
}