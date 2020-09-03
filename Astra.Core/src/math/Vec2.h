#pragma once

#include <iostream>

namespace Astra::Math
{
	/// <summary>
	/// Struct representing a 2-dimensional vector.
	/// </summary>
	struct Vec2
	{
	public:
		float x, y;
	public:

		/// <summary>
		/// Initializes a empty instance of the <see cref="Vec2"/> class.
		/// </summary>
		Vec2();

		/// <summary>
		/// Initializes a new instance of the <see cref="Vec2"/> class.
		/// </summary>
		/// <param name="_x">The x value</param>
		/// <param name="_y">The y value</param>
		Vec2(const float& _x, const float& _y);

		/// <summary>
		/// Copy constructor of the <see cref="Vec2"/> class.
		/// </summary>
		/// <param name="other"></param>
		Vec2(const Vec2& other);

		/// <summary>
		/// Element access operator override.
		/// </summary>
		/// <param name="index">The index of the element to access</param>
		/// <returns></returns>
		float& operator[](int index);

		/// <summary>
		/// Adds the passed vector.
		/// </summary>
		/// <param name="other">The other Vec2 to add</param>
		/// <returns>The summation Vec2</returns>
		Vec2& Add(const Vec2& other);
		
		/// <summary>
		/// Subtracts the passed vector.
		/// </summary>
		/// <param name="other">The other Vec2 to subtract</param>
		/// <returns>The subtracted Vec2</returns>
		Vec2& Subtract(const Vec2& other);

		/// <summary>
		/// Multiplies the passed vector.
		/// </summary>
		/// <param name="other">The other Vec2 to multiply</param>
		/// <returns>The multiplied Vec2</returns>
		Vec2& Multiply(const Vec2& other);

		/// <summary>
		/// Multiplies the passed vector.
		/// </summary>
		/// <param name="other">The scalar to multiply</param>
		/// <returns>The multiplied Vec2</returns>
		Vec2& Multiply(const float& scalar);
		
		/// <summary>
		/// Divides the passed vector.
		/// </summary>
		/// <param name="other">The other Vec2 to divide</param>
		/// <returns>The divided Vec2</returns>
		Vec2& Divide(const Vec2& other);

		/// <summary>
		/// Divides the passed vector.
		/// </summary>
		/// <param name="other">The scalar to divide</param>
		/// <returns>The divided Vec2</returns>
		Vec2& Divide(const float& scalar);

		/// <summary>
		/// Addition operator override.
		/// </summary>
		/// <param name="other">The other Vec2 to add</param>
		/// <returns>The summation Vec2</returns>
		friend Vec2& operator+(Vec2 left, const Vec2& right);

		/// <summary>
		/// Subtraction operator override.
		/// </summary>
		/// <param name="other">The other Vec2 to subtract</param>
		/// <returns>The subtracted Vec2</returns>
		friend Vec2& operator-(Vec2 left, const Vec2& right);
		
		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="other">The other Vec2 to multiply</param>
		/// <returns>The multiplied Vec2</returns>
		friend Vec2& operator*(Vec2 left, const Vec2& right);
		
		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="scalar">The scalar to multiply</param>
		/// <returns>The multiplied Vec2</returns>
		friend Vec2& operator*(Vec2 left, const float& scalar);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="other">The other Vec2 to divide</param>
		/// <returns>The divided Vec2</returns>
		friend Vec2& operator/(Vec2 left, const Vec2& right);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="scalar">The scalar to divide</param>
		/// <returns>The divided Vec2</returns>
		friend Vec2& operator/(Vec2 left, const float& scalar);

		/// <summary>
		/// Addition operator override.
		/// </summary>
		/// <param name="other">The other Vec2 to add</param>
		/// <returns>The summation Vec2</returns>
		Vec2& operator+=(const Vec2& other);

		/// <summary>
		/// Subtraction operator override.
		/// </summary>
		/// <param name="other">The other Vec2 to subtract</param>
		/// <returns>The subtracted Vec2</returns>
		Vec2& operator-=(const Vec2& other);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="other">The other Vec2 to multiply</param>
		/// <returns>The multiplied Vec2</returns>
		Vec2& operator*=(const Vec2& other);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="scalar">The scalar to multiply</param>
		/// <returns>The multiplied Vec2</returns>
		Vec2& operator*=(const float& scalar);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="other">The other Vec2 to divide</param>
		/// <returns>The divided Vec2</returns>
		Vec2& operator/=(const Vec2& other);

		/// <summary>
		/// Division operator override.
		/// </summary>
		/// <param name="scalar">The scalar to divide</param>
		/// <returns>The divided Vec2</returns>
		Vec2& operator/=(const float& scalar);

		/// <summary>
		/// Equality operator override.
		/// </summary>
		/// <param name="comparison">The other object to compare to</param>
		/// <returns>Whether they are are equal</returns>
		bool operator==(const Vec2& other) const;

		/// <summary>
		/// Inequality operator override.
		/// </summary>
		/// <param name="comparison">The other object to compare to</param>
		/// <returns>Whether they are are inequal</returns>
		bool operator!=(const Vec2& other) const;

		/// <summary>
		/// Calculates the dot product with the passed vector.
		/// </summary>
		/// <param name="other">The vector to calculate the dot</param>
		/// <returns>The dot product of the two vectors</returns>
		float Dot(const Vec2& other) const;

		/// <summary>
		/// Calculates the euclidean distance to the passed vector.
		/// </summary>
		/// <param name="other">The other vector to calculate to</param>
		/// <returns>The euclidean distance to the passed vector</returns>
		float DistanceTo(const Vec2& other) const;

		/// <summary>
		/// Calculates the manhatten distance to the passed vector.
		/// </summary>
		/// <param name="other">The other vector to calculate to</param>
		/// <returns>The manhatten distance to the passed vector</returns>
		float ManhattenDistanceTo(const Vec2& other) const;

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
		float DirectionTo(const Vec2& other);

		/// <summary>
		/// Overloads the stream operator retrieving the vec2's string.
		/// </summary>
		/// <param name="stream">The stream to append to</param>
		/// <param name="other">The vec2 to convert</param>
		/// <returns>The modified stream</returns>
		friend std::ostream& operator<<(std::ostream& stream, const Vec2& other)
		{
			stream << "(" << other.x << ", " << other.y << ")";
			return stream;
		}
	};
}