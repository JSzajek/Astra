#pragma once

#include <iostream>

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
		static const iVec2 Left;
		static const iVec2 Right;
		static const iVec2 Up;
		static const iVec2 Down;
	public:
		int x, y;
	public:
		/// <summary>
		/// Initializes a empty instance of the <see cref="iVec2"/> class.
		/// </summary>
		iVec2();

		/// <summary>
		/// Initializes a new instance of the <see cref="iVec2"/> class.
		/// </summary>
		/// <param name="_x">The x value</param>
		/// <param name="_y">The y value</param>
		iVec2(int _x, int _y);

		/// <summary>
		/// Initializes a new instance of the <see cref="iVec2"/> class.
		/// </summary>
		/// <param name="_value">The value across the iVec2</param>
		iVec2(int _value);

		/// <summary>
		/// Copy constructor of the <see cref="iVec2"/> class.
		/// </summary>
		/// <param name="other"></param>
		iVec2(const iVec2& other);

		/// <summary>
		/// Element access operator override.
		/// </summary>
		/// <param name="index">The index of the element to access</param>
		/// <returns></returns>
		int& operator[](int index);

		/// <summary>
		/// Adds the passed vector.
		/// </summary>
		/// <param name="other">The other iVec2 to add</param>
		/// <returns>The summation iVec2</returns>
		iVec2& Add(const iVec2& other);

		/// <summary>
		/// Subtracts the passed vector.
		/// </summary>
		/// <param name="other">The other iVec2 to subtract</param>
		/// <returns>The subtracted iVec2</returns>
		iVec2& Subtract(const iVec2& other);

		/// <summary>
		/// Multiplies the passed vector.
		/// </summary>
		/// <param name="other">The other iVec2 to multiply</param>
		/// <returns>The multiplied iVec2</returns>
		iVec2 Multiply(const iVec2& other);

		/// <summary>
		/// Multiplies the passed vector.
		/// </summary>
		/// <param name="other">The scalar to multiply</param>
		/// <returns>The multiplied iVec2</returns>
		iVec2& Multiply(int scalar);

		/// <summary>
		/// Divides the passed vector.
		/// </summary>
		/// <param name="other">The other iVec2 to divide</param>
		/// <returns>The divided iVec2</returns>
		iVec2& Divide(const iVec2& other);

		/// <summary>
		/// Divides the passed vector.
		/// </summary>
		/// <param name="other">The scalar to divide</param>
		/// <returns>The divided iVec2</returns>
		iVec2& Divide(int scalar);

		/// <summary>
		/// Addition operator override.
		/// </summary>
		/// <param name="other">The other iVec2 to add</param>
		/// <returns>The summation iVec2</returns>
		friend iVec2& operator+(iVec2 left, const iVec2& right);

		/// <summary>
		/// Subtraction operator override.
		/// </summary>
		/// <param name="other">The other iVec2 to subtract</param>
		/// <returns>The subtracted iVec2</returns>
		friend iVec2& operator-(iVec2 left, const iVec2& right);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="other">The other iVec2 to multiply</param>
		/// <returns>The multiplied iVec2</returns>
		friend iVec2 operator*(iVec2 left, const iVec2& right);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="scalar">The scalar to multiply</param>
		/// <returns>The multiplied iVec2</returns>
		friend iVec2& operator*(iVec2 left, int scalar);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="other">The other iVec2 to divide</param>
		/// <returns>The divided iVec2</returns>
		friend iVec2& operator/(iVec2 left, const iVec2& right);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="scalar">The scalar to divide</param>
		/// <returns>The divided iVec2</returns>
		friend iVec2& operator/(iVec2 left, int scalar);

		/// <summary>
		/// Addition operator override.
		/// </summary>
		/// <param name="other">The other iVec2 to add</param>
		/// <returns>The summation iVec2</returns>
		iVec2& operator+=(const iVec2& other);

		/// <summary>
		/// Subtraction operator override.
		/// </summary>
		/// <param name="other">The other iVec2 to subtract</param>
		/// <returns>The subtracted iVec2</returns>
		iVec2& operator-=(const iVec2& other);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="other">The other iVec2 to multiply</param>
		/// <returns>The multiplied iVec2</returns>
		iVec2 operator*=(const iVec2& other);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="scalar">The scalar to multiply</param>
		/// <returns>The multiplied iVec2</returns>
		iVec2& operator*=(int scalar);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="other">The other iVec2 to divide</param>
		/// <returns>The divided iVec2</returns>
		iVec2& operator/=(const iVec2& other);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="scalar">The scalar to divide</param>
		/// <returns>The divided iVec2</returns>
		iVec2& operator/=(int scalar);

		/// <summary>
		/// Equality operator override.
		/// </summary>
		/// <param name="comparison">The other object to compare to</param>
		/// <returns>Whether they are are equal</returns>
		bool operator==(const iVec2& other) const;

		/// <summary>
		/// Inequality operator override.
		/// </summary>
		/// <param name="comparison">The other object to compare to</param>
		/// <returns>Whether they are are inequal</returns>
		bool operator!=(const iVec2& other) const;

		/// <summary>
		/// Calculates the dot product with the passed vector.
		/// </summary>
		/// <param name="other">The vector to calculate the dot</param>
		/// <returns>The dot product of the two vectors</returns>
		int Dot(const iVec2& other) const;

		/// <summary>
		/// Calculates the euclidean distance to the passed vector.
		/// </summary>
		/// <param name="other">The other vector to calculate to</param>
		/// <returns>The euclidean distance to the passed vector</returns>
		float DistanceTo(const iVec2& other) const;

		/// <summary>
		/// Calculates the manhatten distance to the passed vector.
		/// </summary>
		/// <param name="other">The other vector to calculate to</param>
		/// <returns>The manhatten distance to the passed vector</returns>
		int ManhattenDistanceTo(const iVec2& other) const;

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
		float DirectionTo(const iVec2& other);

		/// <summary>
		/// Overloads the stream operator retrieving the iVec2's string.
		/// </summary>
		/// <param name="stream">The stream to append to</param>
		/// <param name="other">The iVec2 to convert</param>
		/// <returns>The modified stream</returns>
		friend std::ostream& operator<<(std::ostream& stream, const iVec2& other)
		{
			stream << "(" << other.x << ", " << other.y << ")";
			return stream;
		}
	};
}