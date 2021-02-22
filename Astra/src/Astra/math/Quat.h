#pragma once

#include <iostream>
#include "MathsFuncs.h"
#include "Astra/math/Vec3.h"

namespace Astra::Math
{
	/// <summary>
	/// Struct representing a quaterion.
	/// </summary>
	struct Quat
	{
	public:
		struct
		{
			float x, y, z, w;
		};
		float m_data[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	public:
		Quat();
		Quat(const Vec3& axis, float angle);

		Quat(float xQuat, float yQuat, float zQuat, float wQuat);
		Quat(const Vec3& first, const Vec3& second);
		Quat(const Quat& other);

		float& operator[](int index);
		const float operator[](int index) const;

		Quat& operator=(const Quat& r_val);

		void operator+=(const Quat& r_val);
		Quat operator+(const Quat& r_val) const;
		
		void operator-=(const Quat& r_val);
		Quat operator-(const Quat& r_val) const;
		
		void operator*=(const Quat& r_val);
		Quat operator*(const Quat& r_val) const;
		Quat operator*(const Vec3& r_val) const;
		Quat operator*(float r_val) const;
		
		void operator/=(float r_val);
		Quat operator/(float r_val) const;

		
		Quat operator-() const;
	
		bool operator==(const Quat& r_val) const;
		bool operator!=(const Quat& r_val) const;

		float Dot(const Quat& r_val) const;

		void Normalize();
		Quat Normalized() const;
		bool IsNormalized() const;

		float Length() const;
		float LengthSquared() const;

		Quat Inverse() const;

		Quat Slerp(const Quat& r_val, const float weight) const;

		bool IsEqualApprox(const Quat& val) const;

		Vec3 XForm(const Vec3& vec) const;
		Vec3 XFormInverted(const Vec3& vec) const;

		#define TOSTRINGQ4(x, y, z, w) "(" #x ", " #y ", " #z ", " #w ")"
		inline std::string ToString() const { return TOSTRINGQ4(x, y, z, w); }

		friend std::ostream& operator<<(std::ostream& stream, const Quat& other)
		{
			stream << other.ToString();
			return stream;
		}
	};

	inline Quat operator*(float scalar, const Quat& quat)
	{
		return quat * scalar;
	}
}