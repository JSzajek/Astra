#pragma once

#include <iostream>
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
		float x, y, z;
	public:
	
		/// <summary>
		/// Initializes a empty instance of the <see cref="Vec3"/> class.
		/// </summary>
		Vec3();
	
		/// <summary>
		/// Initializes a new instance of the <see cref="Vec3"/> class.
		/// </summary>
		/// <param name="_x">The x value</param>
		/// <param name="_y">The y value</param>
		/// <param name="_z">The z value</param>
		Vec3(const float& _x, const float& _y, const float& _z);

		/// <summary>
		/// Initializes a new instance of the <see cref="Vec3"/> class.
		/// </summary>
		/// <param name="_other">The vector containing the x and y values</param>
		/// <param name="_z">The z value</param>
		Vec3(const Vec2& _other, const float& _z);

		/// <summary>
		/// Initializes a new instance of the <see cref="Vec3"/> class.
		/// </summary>
		/// <param name="_value">The value across the Vec3</param>
		Vec3(const float& _value);
	
		/// <summary>
		/// Copy constructor of the <see cref="Vec3"/> class.
		/// </summary>
		/// <param name="other"></param>
		Vec3(const Vec3& other);

		/// <summary>
		/// Element access operator override.
		/// </summary>
		/// <param name="index">The index of the element to access</param>
		/// <returns></returns>
		float& operator[](int index);

		/// <summary>
		/// Adds the passed vector.
		/// </summary>
		/// <param name="other">The other Vec3 to add</param>
		/// <returns>The summation Vec3</returns>
		Vec3& Add(const Vec3& other);

		/// <summary>
		/// Subtracts the passed vector.
		/// </summary>
		/// <param name="other">The other Vec3 to subtract</param>
		/// <returns>The subtracted Vec3</returns>
		Vec3& Subtract(const Vec3& other);

		/// <summary>
		/// Multiplies the passed vector.
		/// </summary>
		/// <param name="other">The other Vec3 to multiply</param>
		/// <returns>The multiplied Vec3</returns>
		Vec3& Multiply(const Vec3& other);

		/// <summary>
		/// Multiplies the passed vector.
		/// </summary>
		/// <param name="other">The scalar to multiply</param>
		/// <returns>The multiplied Vec3</returns>
		Vec3& Multiply(const float& scalar);

		/// <summary>
		/// Divides the passed vector.
		/// </summary>
		/// <param name="other">The other Vec3 to divide</param>
		/// <returns>The divided Vec3</returns>
		Vec3& Divide(const Vec3& other);

		/// <summary>
		/// Divides the passed vector.
		/// </summary>
		/// <param name="other">The scalar to divide</param>
		/// <returns>The divided Vec3</returns>
		Vec3& Divide(const float& scalar);

		/// <summary>
		/// Addition operator override.
		/// </summary>
		/// <param name="other">The other Vec3 to add</param>
		/// <returns>The summation Vec3</returns>
		friend Vec3 operator+(Vec3 left, const Vec3& right);

		/// <summary>
		/// Subtraction operator override.
		/// </summary>
		/// <param name="other">The other Vec3 to subtract</param>
		/// <returns>The subtracted Vec3</returns>
		friend Vec3 operator-(Vec3 left, const Vec3& right);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="other">The other Vec3 to multiply</param>
		/// <returns>The multiplied Vec3</returns>
		friend Vec3 operator*(Vec3 left, const Vec3& right);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="scalar">The scalar to multiply</param>
		/// <returns>The multiplied Vec3</returns>
		friend Vec3 operator*(Vec3 left, const float& scalar);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="other">The other Vec3 to divide</param>
		/// <returns>The divided Vec3</returns>
		friend Vec3 operator/(Vec3 left, const Vec3& right);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="scalar">The scalar to divide</param>
		/// <returns>The divided Vec3</returns>
		friend Vec3 operator/(Vec3 left, const float& scalar);

		/// <summary>
		/// Addition operator override.
		/// </summary>
		/// <param name="other">The other Vec3 to add</param>
		/// <returns>The summation Vec3</returns>
		Vec3& operator+=(const Vec3& other);

		/// <summary>
		/// Subtraction operator override.
		/// </summary>
		/// <param name="other">The other Vec3 to subtract</param>
		/// <returns>The subtracted Vec3</returns>
		Vec3& operator-=(const Vec3& other);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="other">The other Vec3 to multiply</param>
		/// <returns>The multiplied Vec3</returns>
		Vec3& operator*=(const Vec3& other);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="scalar">The scalar to multiply</param>
		/// <returns>The multiplied Vec3</returns>
		Vec3& operator*=(const float& scalar);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="other">The other Vec3 to divide</param>
		/// <returns>The divided Vec3</returns>
		Vec3& operator/=(const Vec3& other);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="scalar">The scalar to divide</param>
		/// <returns>The divided Vec3</returns>
		Vec3& operator/=(const float& scalar);

		/// <summary>
		/// Equality operator override.
		/// </summary>
		/// <param name="comparison">The other object to compare to</param>
		/// <returns>Whether they are are equal</returns>
		bool operator==(const Vec3& other) const;

		/// <summary>
		/// Inequality operator override.
		/// </summary>
		/// <param name="comparison">The other object to compare to</param>
		/// <returns>Whether they are are inequal</returns>
		bool operator!=(const Vec3& other) const;
	
		/// <summary>
		/// Calculates the dot product with the passed vector.
		/// </summary>
		/// <param name="other">The vector to calculate the dot</param>
		/// <returns>The dot product of the two vectors</returns>
		float Dot(const Vec3& other) const;

		float DistanceToSquared(const Vec3& other) const;
		
		/// <summary>
		/// Calculates the euclidean distance to the passed vector.
		/// </summary>
		/// <param name="other">The other vector to calculate to</param>
		/// <returns>The euclidean distance to the passed vector</returns>
		float DistanceTo(const Vec3& other) const;

		/// <summary>
		/// Calculates the manhatten distance to the passed vector.
		/// </summary>
		/// <param name="other">The other vector to calculate to</param>
		/// <returns>The manhatten distance to the passed vector</returns>
		float ManhattenDistanceTo(const Vec3& other) const;

		/// <summary>
		/// Calculates the magnitude of the vector.
		/// </summary>
		/// <returns>The magnitude</returns>
		float MagnitudeSquared() const;

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
		/// <returns>The direction to the passed vector</returns>
		float DirectionTo(const Vec3& other) const;

		/// <summary>
		/// Calculates the cross product to the passed vector.
		/// </summary>
		/// <param name="other">The other vector to calculate to</param>
		/// <returns>The cross product</returns>
		Vec3 Cross(const Vec3& other) const;

		/// <summary>
		/// Converts the vector into a 2-dimensional vector.
		/// </summary>
		/// <returns>The 2-dimensional vector</returns>
		Vec2 ToVec2() const;

		/// <summary>
		/// Overloads the stream operator retrieving the vec3's string.
		/// </summary>
		/// <param name="stream">The stream to append to</param>
		/// <param name="other">The vec3 to convert</param>
		/// <returns>The modified stream</returns>
		friend std::ostream& operator<<(std::ostream& stream, const Vec3& other)
		{
			stream << "(" << other.x << ", " << other.y << ", " << other.z << ")";
			return stream;
		}
	};
}