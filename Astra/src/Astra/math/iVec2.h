#pragma once

#include <iostream>
#include "MathsFuncs.h"

namespace Astra::Math
{
	/// <summary>
	/// Struct representing a 2-dimensional integer vector.
	/// </summary>
	struct iVec2
	{
	public:
		static const iVec2 X_Axis;
		static const iVec2 Y_Axis;
		static const iVec2 Zero;
		static const iVec2 One;
	public:
		union
		{
			int x;
			int width;
		};
		union
		{
			int y;
			int height;
		};
	public:
		iVec2();
		iVec2(int _x, int _y);
		iVec2(int _value);
		iVec2(const iVec2& other);

		int& operator[](int index);
		const int operator[](int index) const;

		iVec2 operator+(const iVec2& r_val) const;
		void operator+=(const iVec2& r_val);

		iVec2 operator-(const iVec2& r_val) const;
		void operator-=(const iVec2& r_val);

		iVec2 operator*(const iVec2& r_val) const;
		iVec2 operator*(const int r_val) const;

		void operator*=(const iVec2& r_val);
		void operator*=(const int r_val);

		iVec2 operator/(const iVec2& r_val) const;
		iVec2 operator/(const int r_val) const;

		void operator/=(const iVec2& r_val);
		void operator/=(const int r_val);

		iVec2 operator-() const;

		bool operator==(const iVec2& r_val) const;
		bool operator!=(const iVec2& r_val) const;

		inline bool operator<(const iVec2& r_val) const { return x == r_val.x ? (y < r_val.y) : (x < r_val.x); }
		inline bool operator>(const iVec2& r_val) const { return x == r_val.x ? (y > r_val.y) : (x > r_val.x); }
		inline bool operator<=(const iVec2& r_val) const { return x == r_val.x ? (y <= r_val.y) : (x < r_val.x); }
		inline bool operator>=(const iVec2& r_val) const { return x == r_val.x ? (y >= r_val.y) : (x > r_val.x); }

		float Angle() const;
		inline iVec2 Abs() const { return iVec2(std::abs(x), std::abs(y)); }
		inline float Aspect() const { return width / static_cast<float>(height); }

		inline iVec2 Min(const iVec2& r_val) const { iVec2(MIN(x, r_val.x), MIN(y, r_val.y)); }
		inline iVec2 Max(const iVec2& r_val) const { iVec2(MAX(x, r_val.x), MAX(y, r_val.y)); }

		#define TOSTRING2i(x, y) "(" #x ", " #y ")"
		inline std::string ToString() const { return TOSTRING2i(x, y); }

		friend std::ostream& operator<<(std::ostream& stream, const iVec2& other)
		{
			stream << other.ToString();
			return stream;
		}
	};

	inline iVec2 operator*(int scalar, const iVec2& p_vec) 
	{
		return p_vec * scalar;
	}
}