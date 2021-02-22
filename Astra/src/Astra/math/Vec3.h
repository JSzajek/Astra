#pragma once

#include <iostream>
#include "MathsFuncs.h"
#include "Vec2.h"

namespace Astra::Math
{
	/// <summary>
	/// Struct representing a 3-dimensional vector.
	/// </summary>
	struct Vec3
	{
	public:
		static const Vec3 X_Axis;
		static const Vec3 Y_Axis;
		static const Vec3 Z_Axis;
		static const Vec3 Zero;
		static const Vec3 One;
		static const Vec3 Left;
		static const Vec3 Right;
		static const Vec3 Up;
		static const Vec3 Down;
		static const Vec3 Forward;
		static const Vec3 Back;
	public:
		union
		{
			struct
			{
				float x, y, z;
			};
			float m_data[3];
		};
	public:
		Vec3();
		Vec3(const float& _x, const float& _y, const float& _z);
		Vec3(const Vec2& _other, const float& _z);
		Vec3(const float& _value);
		Vec3(const Vec3& other);

		float& operator[](int index);
		const float operator[](int index) const;

		Vec3 operator+(const Vec3& r_val) const;
		void operator+=(const Vec3& r_val);

		Vec3 operator-(const Vec3& r_val) const;
		void operator-=(const Vec3& r_val);

		Vec3 operator*(const Vec3& r_val) const;
		Vec3 operator*(const float r_val) const;
		Vec3 operator*(const double r_val) const;
		Vec3 operator*(const int r_val) const;

		void operator*=(const Vec3& r_val);
		void operator*=(const float r_val);
		void operator*=(const double r_val);
		void operator*=(const int r_val);

		Vec3 operator/(const Vec3& r_val) const;
		Vec3 operator/(const float r_val) const;

		void operator/=(const Vec3& r_val);
		void operator/=(const float r_val);

		Vec3 operator-() const;

		bool operator==(const Vec3& r_val) const;
		bool operator!=(const Vec3& r_val) const;

		inline bool operator<(const Vec3& r_val) const;
		inline bool operator>(const Vec3& r_val) const;
		inline bool operator<=(const Vec3& r_val) const;
		inline bool operator>=(const Vec3& r_val) const;

		inline Vec3 Abs() const { return Vec3(std::abs(x), std::abs(y), std::abs(z)); }

		void Rotate(const Vec3& axis, const float val);
		Vec3 Rotated(const Vec3& axis, const float val) const;

		Vec3 Sign() const;
		Vec3 Floor() const;
		Vec3 Ceil() const;
		Vec3 Round() const;
		
		void Snap(const Vec3& r_val);
		Vec3 Snapped(const Vec3& r_val) const;

		void Normalize();
		Vec3 Normalized() const;
		bool IsNormalized() const;

		float Length() const;
		float LengthSquared() const;

		void SetAxis(int axis, float val);
		float GetAxis(int axis) const;

		int MinAxis() const;
		int MaxAxis() const;

		Vec3 Clamped(const float val) const;

		float DistanceTo(const Vec3& r_val) const;
		float DistanceSquaredTo(const Vec3& r_val) const;

		float AngleTo(const Vec3& r_val) const;

		Vec3 DirectionTo(const Vec3& r_val) const;

		Vec3 Mix(const Vec3& r_val, float weight);

		float Dot(const Vec3& r_val) const;
		Vec3 Cross(const Vec3& r_val) const;

		Vec3 PosMod(const Vec3& r_val) const;
		Vec3 PosMod(const float r_val) const;
		Vec3 Project(const Vec3& r_val) const;

		Vec3 PlaneProject(const float val, const Vec3& r_val) const;
		Vec3 Lerp(const Vec3& r_val, const float weight) const;
		Vec3 Slerp(const Vec3& r_val, const float weight) const;
		Vec3 CubicInterpolate(const Vec3& val, const Vec3& pre, const Vec3& pos_val, const float weight) const;
		Vec3 MoveToward(const Vec3& val, const float delta) const;

		Vec3 Slide(const Vec3& norm) const;
		Vec3 Bounce(const Vec3& norm) const;
		Vec3 Reflect(const Vec3& norm) const;

		bool IsEqualApprox(const Vec3& val) const;

		#define TOSTRING3(x, y, z) "(" #x ", " #y ", " #z ")"
		inline std::string ToString() const { return TOSTRING3(x, y, z); }

		inline Vec2 ToVec2() const { return Vec2(x, z); }
		inline operator Vec2() const { return Vec2(x, z); }

		friend std::ostream& operator<<(std::ostream& stream, const Vec3& other)
		{
			stream << other.ToString();
			return stream;
		}
	};

	inline Vec3 operator*(float scalar, const Vec3& vec) 
	{
		return vec * scalar;
	}

	inline Vec3 operator*(double scalar, const Vec3& vec) 
	{
		return vec * scalar;
	}

	inline Vec3 operator*(int scalar, const Vec3& vec) 
	{
		return vec * scalar;
	}
}