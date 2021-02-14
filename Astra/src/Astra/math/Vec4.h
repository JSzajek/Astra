#pragma once

#include "Vec3.h"
#include <iostream>

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
		float x, y, z, w;
	public:
	
		/// <summary>
		/// Default constructor of the <see cref="Vec4"/> class.
		/// </summary>
		Vec4() = default;
	
		/// <summary>
		/// Initializes a new instance of the <see cref="Vec4"/> class.
		/// </summary>
		/// <param name="_x">The x value</param>
		/// <param name="_y">The y value</param>
		/// <param name="_z">The z value</param>
		/// <param name="_w">The w value</param>
		Vec4(const float& _x, const float& _y, const float& _z, const float& _w);
	
		/// <summary>
		/// Initializes a new instance of the <see cref="Vec4"/> class.
		/// </summary>
		/// <param name="_value">The value across the Vec4</param>
		Vec4(const float& _value);

		Vec4(const Vec3& other, float _w);

		Vec4(const Vec2& first, const Vec2& second);

		/// <summary>
		/// Copy constructor of the <see cref="Vec4"/> class.
		/// </summary>
		/// <param name="other"></param>
		Vec4(const Vec4& other);

		operator Math::Vec3() const;

		/// <summary>
		/// Element access operator override.
		/// </summary>
		/// <param name="index">The index of the element to access</param>
		/// <returns></returns>
		float& operator[](int index);

		const float operator[](int index) const;

		/// <summary>
		/// Adds the passed vector.
		/// </summary>
		/// <param name="other">The other Vec4 to add</param>
		/// <returns>The summation Vec4</returns>
		Vec4& Add(const Vec4& other);

		/// <summary>
		/// Subtracts the passed vector.
		/// </summary>
		/// <param name="other">The other Vec4 to subtract</param>
		/// <returns>The subtracted Vec4</returns>
		Vec4& Subtract(const Vec4& other);

		/// <summary>
		/// Multiplies the passed vector.
		/// </summary>
		/// <param name="other">The other Vec4 to multiply</param>
		/// <returns>The multiplied Vec4</returns>
		Vec4& Multiply(const Vec4& other);

		/// <summary>
		/// Multiplies the passed vector.
		/// </summary>
		/// <param name="other">The scalar to multiply</param>
		/// <returns>The multiplied Vec4</returns>
		Vec4& Multiply(const float& scalar);

		/// <summary>
		/// Divides the passed vector.
		/// </summary>
		/// <param name="other">The other Vec4 to divide</param>
		/// <returns>The divided Vec4</returns>
		Vec4& Divide(const Vec4& other);

		/// <summary>
		/// Divides the passed vector.
		/// </summary>
		/// <param name="other">The scalar to divide</param>
		/// <returns>The divided Vec4</returns>
		Vec4& Divide(const float& scalar);

		/// <summary>
		/// Addition operator override.
		/// </summary>
		/// <param name="other">The other Vec4 to add</param>
		/// <returns>The summation Vec4</returns>
		friend Vec4& operator+(Vec4 left, const Vec4& right);

		/// <summary>
		/// Subtraction operator override.
		/// </summary>
		/// <param name="other">The other Vec4 to subtract</param>
		/// <returns>The subtracted Vec4</returns>
		friend Vec4& operator-(Vec4 left, const Vec4& right);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="other">The other Vec4 to multiply</param>
		/// <returns>The multiplied Vec4</returns>
		friend Vec4& operator*(Vec4 left, const Vec4& right);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="scalar">The scalar to multiply</param>
		/// <returns>The multiplied Vec4</returns>
		friend Vec4& operator*(Vec4 left, const float& scalar);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="other">The other Vec4 to divide</param>
		/// <returns>The divided Vec4</returns>
		friend Vec4& operator/(Vec4 left, const Vec4& right);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="scalar">The scalar to divide</param>
		/// <returns>The divided Vec4</returns>
		friend Vec4& operator/(Vec4 left, const float& scalar);

		/// <summary>
		/// Addition operator override.
		/// </summary>
		/// <param name="other">The other Vec4 to add</param>
		/// <returns>The summation Vec4</returns>
		Vec4& operator+=(const Vec4& other);

		/// <summary>
		/// Subtraction operator override.
		/// </summary>
		/// <param name="other">The other Vec4 to subtract</param>
		/// <returns>The subtracted Vec4</returns>
		Vec4& operator-=(const Vec4& other);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="other">The other Vec4 to multiply</param>
		/// <returns>The multiplied Vec4</returns>
		Vec4& operator*=(const Vec4& other);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="scalar">The scalar to multiply</param>
		/// <returns>The multiplied Vec4</returns>
		Vec4& operator*=(const float& scalar);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="other">The other Vec4 to divide</param>
		/// <returns>The divided Vec4</returns>
		Vec4& operator/=(const Vec4& other);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="scalar">The scalar to divide</param>
		/// <returns>The divided Vec4</returns>
		Vec4& operator/=(const float& scalar);

		/// <summary>
		/// Equality operator override.
		/// </summary>
		/// <param name="comparison">The other object to compare to</param>
		/// <returns>Whether they are are equal</returns>
		bool operator==(const Vec4& other) const;

		/// <summary>
		/// Inequality operator override.
		/// </summary>
		/// <param name="comparison">The other object to compare to</param>
		/// <returns>Whether they are are inequal</returns>
		bool operator!=(const Vec4& other) const;
	
		/// <summary>
		/// Calculates the dot product with the passed vector.
		/// </summary>
		/// <param name="other">The vector to calculate the dot</param>
		/// <returns>The dot product of the two vectors</returns>
		float Dot(const Vec4& other) const;

		/// <summary>
		/// Calculates the euclidean distance to the passed vector.
		/// </summary>
		/// <param name="other">The other vector to calculate to</param>
		/// <returns>The euclidean distance to the passed vector</returns>
		float DistanceTo(const Vec4& other) const;

		/// <summary>
		/// Calculates the manhatten distance to the passed vector.
		/// </summary>
		/// <param name="other">The other vector to calculate to</param>
		/// <returns>The manhatten distance to the passed vector</returns>
		float ManhattenDistanceTo(const Vec4& other) const;

		/// <summary>
		/// Calculates the magnitude of the vector.
		/// </summary>
		/// <returns>The magnitude</returns>
		float Magnitude() const;
	
		/// <summary>
		/// Normalizes the vector.
		/// </summary>
		void Normalize();

		/// <summary>
		/// Calculates the direction to the passed vector.
		/// </summary>
		/// <param name="other">The other vector to calculate to</param>
		/// <returns></returns>
		float DirectionTo(const Vec4& other) const;

		/// <summary>
		/// Overloads the stream operator retrieving the Vec4's string.
		/// </summary>
		/// <param name="stream">The stream to append to</param>
		/// <param name="other">The Vec4 to convert</param>
		/// <returns>The modified stream</returns>
		friend std::ostream& operator<<(std::ostream& stream, const Vec4& other)
		{
			stream << "(" << other.x << ", " << other.y << ", " << other.z << ", " << other.w << ")";
			return stream;
		}
	};
}