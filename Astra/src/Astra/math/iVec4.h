#pragma once

#include <iostream>
#include "MathsFuncs.h"
#include "iVec2.h"

namespace Astra::Math
{
	/// <summary>
	/// Struct representing a 4-dimensional integer vector.
	/// </summary>
	struct iVec4
	{
	public:
		static const iVec4 X_Axis;
		static const iVec4 Y_Axis;
		static const iVec4 Z_Axis;
		static const iVec4 W_Axis;
		static const iVec4 Zero;
		static const iVec4 One;
	public:
		union
		{
			struct
			{
				int x, y, z, w;
			};
			int m_data[4];
		};
	public:
		iVec4();
		iVec4(int _x, int _y, int _z, int _w);
		iVec4(const int _value);
		iVec4(const iVec2& first, const iVec2& second);
		iVec4(const iVec4& other);

		int& operator[](int index);
		const int operator[](int index) const;

		iVec4 operator+(const iVec4& r_val) const;
		void operator+=(const iVec4& r_val);

		iVec4 operator-(const iVec4& r_val) const;
		void operator-=(const iVec4& r_val);

		iVec4 operator*(const iVec4& r_val) const;
		iVec4 operator*(const int r_val) const;

		void operator*=(const iVec4& r_val);
		void operator*=(const int r_val);

		iVec4 operator/(const iVec4& r_val) const;
		iVec4 operator/(const int r_val) const;

		void operator/=(const iVec4& r_val);
		void operator/=(const int r_val);

		iVec4 operator-() const;

		bool operator==(const iVec4& r_val) const;
		bool operator!=(const iVec4& r_val) const;

		inline bool operator<(const iVec4& r_val) const;
		inline bool operator>(const iVec4& r_val) const;
		inline bool operator<=(const iVec4& r_val) const;
		inline bool operator>=(const iVec4& r_val) const;

		float AngleTo(const iVec4& r_val) const;

		inline iVec4 Abs() const { return iVec4(std::abs(x), std::abs(y), std::abs(z), std::abs(w)); }

		float Length() const;
		int LengthSquared() const;

		void SetAxis(int axis, int val);
		int GetAxis(int axis) const;

		int MinAxis() const;
		int MaxAxis() const;

		float DistanceTo(const iVec4& r_val) const;
		int DistanceSquaredTo(const iVec4& r_val) const;

		int Dot(const iVec4& r_val) const;
		iVec4 Cross(const iVec4& r_val) const;

		#define TOSTRING4i(x, y, z, w) "(" #x ", " #y ", " #z ", " #w ")"
		inline std::string ToString() const { return TOSTRING4i(x, y, z, w); }

		friend std::ostream& operator<<(std::ostream& stream, const iVec4& other)
		{
			stream << other.ToString();
			return stream;
		}
	};

	inline iVec4 operator*(int scalar, const iVec4& vec) 
	{
		return vec * scalar;
	}
}