#include "astra_pch.h"

#include "Vec2.h"

namespace Astra::Math
{
	const Vec2 Vec2::X_Axis = Vec2(1, 0);
	const Vec2 Vec2::Y_Axis = Vec2(0, 1);
	const Vec2 Vec2::Zero = Vec2(0);
	const Vec2 Vec2::One = Vec2(1);
	const Vec2 Vec2::Left = Vec2(-1, 0);
	const Vec2 Vec2::Right = Vec2(1, 0);
	const Vec2 Vec2::Up = Vec2(0, -1);
	const Vec2 Vec2::Down = Vec2(0, 1);

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
		ASTRA_ASSERT(index < 2, "Vec2: Out Of Index Axis Access.");
		return index ? y : x;
	}

	const float Vec2::operator[](int index) const
	{
		ASTRA_ASSERT(index < 2, "Vec2: Out Of Index Axis Access.");
		return index ? y : x;
	}

	Vec2 Vec2::operator+(const Vec2& r_val) const
	{
		return Vec2(x + r_val.x, y + r_val.y);
	}

	void Vec2::operator+=(const Vec2& r_val)
	{
		x += r_val.x;
		y += r_val.y;
	}

	Vec2 Vec2::operator-(const Vec2& r_val) const
	{
		return Vec2(x - r_val.x, y - r_val.y);
	}

	void Vec2::operator-=(const Vec2& r_val)
	{
		x -= r_val.x;
		y -= r_val.y;
	}

	Vec2 Vec2::operator*(const Vec2& r_val) const 
	{
		return Vec2(x * r_val.x, y * r_val.y);
	}

	Vec2 Vec2::operator*(const float r_val) const
	{
		return Vec2(x * r_val, y * r_val);
	}

	Vec2 Vec2::operator*(const double r_val) const
	{
		return Vec2(static_cast<float>(x * r_val), static_cast<float>(y * r_val));
	}

	Vec2 Vec2::operator*(const int r_val) const
	{
		return Vec2(x * r_val, y * r_val);
	}

	void Vec2::operator*=(const Vec2& r_val)
	{
		x *= r_val.x;
		y *= r_val.y;
	}

	void Vec2::operator*=(const float r_val)
	{
		x *= r_val;
		y *= r_val;
	}

	void Vec2::operator*=(const double r_val)
	{
		x = static_cast<float>(x * r_val);
		y = static_cast<float>(y * r_val);
	}

	void Vec2::operator*=(const int r_val)
	{
		x *= r_val;
		y *= r_val;
	}

	Vec2 Vec2::operator/(const Vec2& r_val) const
	{
		return Vec2(x / r_val.x, y / r_val.y);
	}

	Vec2 Vec2::operator/(const float r_val) const
	{
		return Vec2(x / r_val, y / r_val);
	}

	void Vec2::operator/=(const Vec2& r_val)
	{
		x /= r_val.x;
		y /= r_val.y;
	}

	void Vec2::operator/=(const float r_val)
	{
		x /= r_val;
		y /= r_val;
	}

	Vec2 Vec2::operator-() const
	{
		return Vec2(-x, -y);
	}

	bool Vec2::operator==(const Vec2& r_val) const
	{
		return x == r_val.x && y == r_val.y;
	}

	bool Vec2::operator!=(const Vec2& r_val) const
	{
		return x != r_val.x || y != r_val.y;
	}

	float Vec2::Angle() const
	{
		return std::atan2f(y, x);
	}

	Vec2 Vec2::Rotated(const float val) const
	{
		float _sin = std::sinf(val);
		float _cos = std::cosf(val);
		return Vec2(x * _cos - y * _sin, x * _sin + y * _cos);
	}

	Vec2 Vec2::Sign() const
	{
		return Vec2(SGN(x), SGN(y));
	}

	Vec2 Vec2::Floor() const
	{
		return Vec2(std::floorf(x), std::floorf(y));
	}

	Vec2 Vec2::Ceil() const
	{
		return Vec2(std::ceilf(x), std::ceilf(y));
	}

	Vec2 Vec2::Round() const
	{
		return Vec2(std::roundf(x), std::roundf(y));
	}

	Vec2 Vec2::Snapped(const Vec2& r_val) const
	{
		return Vec2(Math::Snapped(x, r_val.x), Math::Snapped(y, r_val.y));
	}

	void Vec2::Normalize()
	{
		float len = x * x + y * y;
		if (len != 0)
		{
			len = std::sqrtf(len);
			x /= len;
			y /= len;
		}
	}

	Vec2 Vec2::Normalized() const
	{
		Vec2 result = *this;
		result.Normalize();
		return result;
	}

	bool Vec2::IsNormalized() const
	{
		return Math::IsEqualApprox(LengthSquared(), 1.0f, UNIT_EPSILON);
	}

	float Vec2::Length() const
	{
		return std::sqrtf((x * x) + (y * y));
	}

	float Vec2::LengthSquared() const
	{
		return (x * x) + (y * y);
	}

	Vec2 Vec2::Clamped(const float val) const
	{
		float len = Length();
		Vec2 temp = *this;
		if (len > 0 && val < len)
		{
			temp /= len;
			temp *= val;
		}
		return temp;
	}

	float Vec2::DistanceTo(const Vec2& r_val) const
	{
		return std::sqrt(((x - r_val.x) * (x - r_val.x)) + ((y - r_val.y) * (y - r_val.y)));
	}

	float Vec2::DistanceSquaredTo(const Vec2& r_val) const
	{
		return ((x - r_val.x) * (x - r_val.x)) + ((y - r_val.y) * (y - r_val.y));
	}

	float Vec2::AngleTo(const Vec2& r_val) const
	{
		return std::atan2f(Cross(r_val), Dot(r_val));
	}

	float Vec2::AngleToPoint(const Vec2& point) const
	{
		return std::atan2f(y - point.y, x - point.x);
	}

	Vec2 Vec2::DirectionTo(const Vec2& r_val) const
	{
		Vec2 result(r_val.x - x, r_val.y - y);
		result.Normalize();
		return result;
	}

	float Vec2::Dot(const Vec2& r_val) const
	{
		return x * r_val.x + y * r_val.y;
	}

	float Vec2::Cross(const Vec2& r_val) const
	{
		return x * r_val.y - y * r_val.x;
	}

	Vec2 Vec2::PosMod(const Vec2& r_val) const
	{
		return Vec2(PosModf(x, r_val.x), PosModf(y, r_val.y));
	}

	Vec2 Vec2::PosMod(const float r_val) const
	{
		return Vec2(PosModf(x, r_val), PosModf(y, r_val));
	}

	Vec2 Vec2::Project(const Vec2& r_val) const
	{
		return r_val * (Dot(r_val) / r_val.LengthSquared());
	}

	Vec2 Vec2::PlaneProject(const float val, const Vec2& r_val) const
	{
		return r_val - *this * (Dot(r_val) - val);
	}

	Vec2 Vec2::Lerp(const Vec2& r_val, const float weight) const
	{
		Vec2 result = *this;
		result.x += (weight * (r_val.x - x));
		result.y += (weight * (r_val.y - y));
		return result;
	}

	Vec2 Vec2::Slerp(const Vec2& r_val, const float weight) const
	{
		ASTRA_ASSERT(!IsNormalized(), "Vec2: The Normal Vector Must be Normalized.");
		float theta = AngleTo(r_val);
		return Rotated(theta * weight);
	}

	Vec2 Vec2::CubicInterpolate(const Vec2& val, const Vec2& pre, const Vec2& pos_val, const float weight) const
	{
		Vec2 p_0 = pre;
		Vec2 p_1 = *this;
		Vec2 p_2 = val;
		Vec2 p_3 = pos_val;

		float t = weight;
		float t_2 = t * t;
		float t_3 = t_2 * t;

		Vec2 result;
		result = 0.5f * ((p_1 * 2.0f) + (-p_0 + p_2) * t 
						+ (2.0f * p_0 - 5.0 * p_1 + 4 * p_2 - p_3) * t_2 
						+ (-p_0 + 3.0f * p_1 - 3.0f * p_2 + p_3) * t_3);
		return result;
	}

	Vec2 Vec2::MoveToward(const Vec2& val, const float delta) const
	{
		Vec2 vec = *this;
		Vec2 vec_dis = val - vec;
		float len = vec_dis.Length();
		return len <= delta || len < CMP_EPSILON ? val : vec + vec_dis / len * delta;
	}

	Vec2 Vec2::Slide(const Vec2& norm) const
	{
		ASTRA_ASSERT(!norm.IsNormalized(), "Vec2: The Normal Vector Must be Normalized.");
		return *this - norm * this->Dot(norm);
	}

	Vec2 Vec2::Bounce(const Vec2& norm) const
	{
		return -Reflect(norm);
	}

	Vec2 Vec2::Reflect(const Vec2& norm) const
	{
		ASTRA_ASSERT(!norm.IsNormalized(), "Vec2: The Normal Vector Must be Normalized.");
		return 2.0f * norm * this->Dot(norm) - *this;
	}

	bool Vec2::IsEqualApprox(const Vec2& val) const
	{
		return Math::IsEqualApprox(x, val.x) && Math::IsEqualApprox(y, val.y);
	}
}