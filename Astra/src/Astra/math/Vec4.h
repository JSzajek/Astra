#pragma once

#include <iostream>
#include "MathsFuncs.h"
#include "Vec3.h"

namespace Astra::Math
{
	/// <summary>
	/// Struct representing a 4-dimensional vector.
	/// </summary>
	struct Vec4
	{
	public:
		static const Vec4 X_Axis;
		static const Vec4 Y_Axis;
		static const Vec4 Z_Axis;
		static const Vec4 W_Axis;
		static const Vec4 Zero;
		static const Vec4 One;
		static const Vec4 Left;
		static const Vec4 Right;
		static const Vec4 Up;
		static const Vec4 Down;
		static const Vec4 Forward;
		static const Vec4 Back;
	public:
		union
		{
			struct
			{
				float x, y, z, w;
			};
			float m_data[4];
		};
	public:
		Vec4();
		Vec4(const float& _x, const float& _y, const float& _z, const float& _w);
		Vec4(const float& _value);
		Vec4(const Vec3& other, float _w);
		Vec4(const Vec2& first, const Vec2& second);
		Vec4(const Vec4& other);

		float& operator[](int index);
		const float operator[](int index) const;

		Vec4 operator+(const Vec4& r_val) const;
		void operator+=(const Vec4& r_val);

		Vec4 operator-(const Vec4& r_val) const;
		void operator-=(const Vec4& r_val);

		Vec4 operator*(const Vec4& r_val) const;
		Vec4 operator*(const float r_val) const;
		Vec4 operator*(const double r_val) const;
		Vec4 operator*(const int r_val) const;

		void operator*=(const Vec4& r_val);
		void operator*=(const float r_val);
		void operator*=(const double r_val);
		void operator*=(const int r_val);

		Vec4 operator/(const Vec4& r_val) const;
		Vec4 operator/(const float r_val) const;

		void operator/=(const Vec4& r_val);
		void operator/=(const float r_val);

		Vec4 operator-() const;

		bool operator==(const Vec4& r_val) const;
		bool operator!=(const Vec4& r_val) const;

		bool operator<(const Vec4& r_val) const;
		bool operator>(const Vec4& r_val) const;
		bool operator<=(const Vec4& r_val) const;
		bool operator>=(const Vec4& r_val) const;

		inline Vec4 Abs() const { return Vec4(std::abs(x), std::abs(y), std::abs(z), std::abs(w)); }

		Vec4 Sign() const;
		Vec4 Floor() const;
		Vec4 Ceil() const;
		Vec4 Round() const;

		void Snap(const Vec4& r_val);
		Vec4 Snapped(const Vec4& r_val) const;

		void Normalize();
		Vec4 Normalized() const;
		bool IsNormalized() const;

		float Length() const;
		float LengthSquared() const;

		void SetAxis(int axis, float val);
		float GetAxis(int axis) const;

		int MinAxis() const;
		int MaxAxis() const;

		Vec4 Clamped(const float val) const;

		float DistanceTo(const Vec4& r_val) const;
		float DistanceSquaredTo(const Vec4& r_val) const;

		float AngleTo(const Vec4& r_val) const;

		Vec4 DirectionTo(const Vec4& r_val) const;

		float Dot(const Vec4& r_val) const;
		Vec4 Cross(const Vec4& r_val) const;

		Vec4 PosMod(const Vec4& r_val) const;
		Vec4 PosMod(const float r_val) const;
		Vec4 Project(const Vec4& r_val) const;
		Vec4 PlaneProject(const float val, const Vec4& r_val) const;

		bool IsEqualApprox(const Vec4& val) const;

		#define TOSTRING4(x, y, z, w) "(" #x ", " #y ", " #z ", " #w ")"
		inline std::string ToString() const { return TOSTRING4(x, y, z, w); }

		inline Vec3 ToVec3() const { return Vec3(x, y, z); }
		inline operator Vec3() const { return Vec3(x, y, z); }

		friend std::ostream& operator<<(std::ostream& stream, const Vec4& other)
		{
			stream << other.ToString();
			return stream;
		}
	};

	inline Vec4 operator*(float scalar, const Vec4& vec) 
	{
		return vec * scalar;
	}

	inline Vec4 operator*(double scalar, const Vec4& vec) 
	{
		return vec * scalar;
	}

	inline Vec4 operator*(int scalar, const Vec4& vec) 
	{
		return vec * scalar;
	}
}