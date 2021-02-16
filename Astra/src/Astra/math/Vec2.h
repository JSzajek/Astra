#pragma once

#include <iostream>
#include "MathsFuncs.h"

namespace Astra::Math
{
	/// <summary>
	/// Struct representing a 2-dimensional vector.
	/// </summary>
	struct Vec2
	{
	public:
		static const Vec2 X_Axis;
		static const Vec2 Y_Axis;
		static const Vec2 Zero;
		static const Vec2 One;
		static const Vec2 Left;
		static const Vec2 Right;
		static const Vec2 Up;
		static const Vec2 Down;
	public:
		union
		{
			float x;
			float width;
		};
		union
		{
			float y;
			float height;
		};
	public:
		Vec2();
		Vec2(float _x, float _y);
		Vec2(float _value);
		Vec2(const Vec2& other);

		float& operator[](int index);
		const float operator[](int index) const;

		Vec2 operator+(const Vec2& r_val) const;
		void operator+=(const Vec2& r_val);

		Vec2 operator-(const Vec2& r_val) const;
		void operator-=(const Vec2& r_val);

		Vec2 operator*(const Vec2& r_val) const;
		Vec2 operator*(const float r_val) const;
		Vec2 operator*(const double r_val) const;
		Vec2 operator*(const int r_val) const;

		void operator*=(const Vec2& r_val);
		void operator*=(const float r_val);
		void operator*=(const double r_val);
		void operator*=(const int r_val);

		Vec2 operator/(const Vec2& r_val) const;
		Vec2 operator/(const float r_val) const;

		void operator/=(const Vec2& r_val);
		void operator/=(const float r_val);

		Vec2 operator-() const;

		bool operator==(const Vec2& r_val) const;
		bool operator!=(const Vec2& r_val) const;

		inline bool operator<(const Vec2& r_val) const { return x == r_val.x ? (y < r_val.y) : (x < r_val.x); }
		inline bool operator>(const Vec2& r_val) const { return x == r_val.x ? (y > r_val.y) : (x > r_val.x); }
		inline bool operator<=(const Vec2& r_val) const { return x == r_val.x ? (y <= r_val.y) : (x < r_val.x); }
		inline bool operator>=(const Vec2& r_val) const { return x == r_val.x ? (y >= r_val.y) : (x > r_val.x); }

		float Angle() const;

		inline Vec2 Abs() const { return Vec2(std::abs(x), std::abs(y)); }

		Vec2 Rotated(const float val) const;
		inline Vec2 Orthogonal() const { return Vec2(y, -x); }

		Vec2 Sign() const;
		Vec2 Floor() const;
		Vec2 Ceil() const;
		Vec2 Round() const;
		Vec2 Snapped(const Vec2& r_val) const;
		inline float Aspect() const { return width / static_cast<float>(height); }

		void Normalize();
		Vec2 Normalized() const;
		bool IsNormalized() const;

		float Length() const;
		float LengthSquared() const;

		inline Vec2 Min(const Vec2& r_val) const { Vec2(MIN(x, r_val.x), MIN(y, r_val.y)); }
		inline Vec2 Max(const Vec2& r_val) const { Vec2(MAX(x, r_val.x), MAX(y, r_val.y)); }
		Vec2 Clamped(const float val) const;

		float DistanceTo(const Vec2& r_val) const;
		float DistanceSquaredTo(const Vec2& r_val) const;

		float AngleTo(const Vec2& r_val) const;
		float AngleToPoint(const Vec2& point) const;

		Vec2 DirectionTo(const Vec2& r_val) const;

		float Dot(const Vec2& r_val) const;
		float Cross(const Vec2& r_val) const;

		Vec2 PosMod(const Vec2& r_val) const;
		Vec2 PosMod(const float r_val) const;
		Vec2 Project(const Vec2& r_val) const;

		Vec2 PlaneProject(const float val, const Vec2& r_val) const;
		Vec2 Lerp(const Vec2& r_val, const float weight) const;
		Vec2 Slerp(const Vec2& r_val, const float weight) const;
		Vec2 CubicInterpolate(const Vec2& val, const Vec2& pre, const Vec2& pos_val, const float weight) const;
		Vec2 MoveToward(const Vec2& val, const float delta) const;

		Vec2 Slide(const Vec2& norm) const;
		Vec2 Bounce(const Vec2& norm) const;
		Vec2 Reflect(const Vec2& norm) const;

		bool IsEqualApprox(const Vec2& val) const;

		#define TOSTRING2(x, y) "(" #x ", " #y ")"
		inline std::string ToString() const { return TOSTRING2(x, y); }

		friend std::ostream& operator<<(std::ostream& stream, const Vec2& other)
		{
			stream << other.ToString();
			return stream;
		}
	};

	inline Vec2 operator*(float scalar, const Vec2& vec) 
	{
		return vec * scalar;
	}

	inline Vec2 operator*(double scalar, const Vec2& vec) 
	{
		return vec * scalar;
	}
	
	inline Vec2 operator*(int scalar, const Vec2& vec) 
	{
		return vec * scalar;
	}
}