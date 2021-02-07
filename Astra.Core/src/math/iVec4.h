#pragma once

#include <iostream>

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
		static const iVec4 Left;
		static const iVec4 Right;
		static const iVec4 Up;
		static const iVec4 Down;
		static const iVec4 Forward;
		static const iVec4 Back;
	public:
		int x, y, z, w;
	public:

		/// <summary>
		/// Default constructor of the <see cref="iVec4"/> class.
		/// </summary>
		iVec4() = default;

		/// <summary>
		/// Initializes a new instance of the <see cref="iVec4"/> class.
		/// </summary>
		/// <param name="_x">The x value</param>
		/// <param name="_y">The y value</param>
		/// <param name="_z">The z value</param>
		/// <param name="_w">The w value</param>
		iVec4(int _x, int _y, int _z, int _w);

		/// <summary>
		/// Initializes a new instance of the <see cref="iVec4"/> class.
		/// </summary>
		/// <param name="_value">The value across the iVec4</param>
		iVec4(const int _value);

		iVec4(const iVec2& first, const iVec2& second);

		/// <summary>
		/// Copy constructor of the <see cref="iVec4"/> class.
		/// </summary>
		/// <param name="other"></param>
		iVec4(const iVec4& other);

		/// <summary>
		/// Element access operator override.
		/// </summary>
		/// <param name="index">The index of the element to access</param>
		/// <returns></returns>
		int& operator[](int index);

		const int operator[](int index) const;

		/// <summary>
		/// Adds the passed vector.
		/// </summary>
		/// <param name="other">The other iVec4 to add</param>
		/// <returns>The summation iVec4</returns>
		iVec4& Add(const iVec4& other);

		/// <summary>
		/// Subtracts the passed vector.
		/// </summary>
		/// <param name="other">The other iVec4 to subtract</param>
		/// <returns>The subtracted iVec4</returns>
		iVec4& Subtract(const iVec4& other);

		/// <summary>
		/// Multiplies the passed vector.
		/// </summary>
		/// <param name="other">The other iVec4 to multiply</param>
		/// <returns>The multiplied iVec4</returns>
		iVec4& Multiply(const iVec4& other);

		/// <summary>
		/// Multiplies the passed vector.
		/// </summary>
		/// <param name="other">The scalar to multiply</param>
		/// <returns>The multiplied iVec4</returns>
		iVec4& Multiply(int scalar);

		/// <summary>
		/// Divides the passed vector.
		/// </summary>
		/// <param name="other">The other iVec4 to divide</param>
		/// <returns>The divided iVec4</returns>
		iVec4& Divide(const iVec4& other);

		/// <summary>
		/// Divides the passed vector.
		/// </summary>
		/// <param name="other">The scalar to divide</param>
		/// <returns>The divided iVec4</returns>
		iVec4& Divide(int scalar);

		/// <summary>
		/// Addition operator override.
		/// </summary>
		/// <param name="other">The other iVec4 to add</param>
		/// <returns>The summation iVec4</returns>
		friend iVec4& operator+(iVec4 left, const iVec4& right);

		/// <summary>
		/// Subtraction operator override.
		/// </summary>
		/// <param name="other">The other iVec4 to subtract</param>
		/// <returns>The subtracted iVec4</returns>
		friend iVec4& operator-(iVec4 left, const iVec4& right);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="other">The other iVec4 to multiply</param>
		/// <returns>The multiplied iVec4</returns>
		friend iVec4& operator*(iVec4 left, const iVec4& right);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="scalar">The scalar to multiply</param>
		/// <returns>The multiplied iVec4</returns>
		friend iVec4& operator*(iVec4 left, int scalar);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="other">The other iVec4 to divide</param>
		/// <returns>The divided iVec4</returns>
		friend iVec4& operator/(iVec4 left, const iVec4& right);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="scalar">The scalar to divide</param>
		/// <returns>The divided iVec4</returns>
		friend iVec4& operator/(iVec4 left, int scalar);

		/// <summary>
		/// Addition operator override.
		/// </summary>
		/// <param name="other">The other iVec4 to add</param>
		/// <returns>The summation iVec4</returns>
		iVec4& operator+=(const iVec4& other);

		/// <summary>
		/// Subtraction operator override.
		/// </summary>
		/// <param name="other">The other iVec4 to subtract</param>
		/// <returns>The subtracted iVec4</returns>
		iVec4& operator-=(const iVec4& other);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="other">The other iVec4 to multiply</param>
		/// <returns>The multiplied iVec4</returns>
		iVec4& operator*=(const iVec4& other);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="scalar">The scalar to multiply</param>
		/// <returns>The multiplied iVec4</returns>
		iVec4& operator*=(int scalar);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="other">The other iVec4 to divide</param>
		/// <returns>The divided iVec4</returns>
		iVec4& operator/=(const iVec4& other);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="scalar">The scalar to divide</param>
		/// <returns>The divided iVec4</returns>
		iVec4& operator/=(int scalar);

		/// <summary>
		/// Equality operator override.
		/// </summary>
		/// <param name="comparison">The other object to compare to</param>
		/// <returns>Whether they are are equal</returns>
		bool operator==(const iVec4& other) const;

		/// <summary>
		/// Inequality operator override.
		/// </summary>
		/// <param name="comparison">The other object to compare to</param>
		/// <returns>Whether they are are inequal</returns>
		bool operator!=(const iVec4& other) const;

		/// <summary>
		/// Calculates the dot product with the passed vector.
		/// </summary>
		/// <param name="other">The vector to calculate the dot</param>
		/// <returns>The dot product of the two vectors</returns>
		int Dot(const iVec4& other) const;

		/// <summary>
		/// Calculates the euclidean distance to the passed vector.
		/// </summary>
		/// <param name="other">The other vector to calculate to</param>
		/// <returns>The euclidean distance to the passed vector</returns>
		float DistanceTo(const iVec4& other) const;

		/// <summary>
		/// Calculates the manhatten distance to the passed vector.
		/// </summary>
		/// <param name="other">The other vector to calculate to</param>
		/// <returns>The manhatten distance to the passed vector</returns>
		int ManhattenDistanceTo(const iVec4& other) const;

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
		float DirectionTo(const iVec4& other) const;

		/// <summary>
		/// Overloads the stream operator retrieving the iVec4's string.
		/// </summary>
		/// <param name="stream">The stream to append to</param>
		/// <param name="other">The iVec4 to convert</param>
		/// <returns>The modified stream</returns>
		friend std::ostream& operator<<(std::ostream& stream, const iVec4& other)
		{
			stream << "(" << other.x << ", " << other.y << ", " << other.z << ", " << other.w << ")";
			return stream;
		}
	};
}