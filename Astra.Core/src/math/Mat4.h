#pragma once

#include "MathsUtils.h"
#include "Vec3.h"
#include "Vec4.h"

namespace Astra::Math
{
	struct Mat4
	{
		union
		{
			float data[4 * 4];
			Vec4 columns[4];
		};

		Mat4();
		Mat4(float diagonal);
		Mat4(const Mat4&);

		static Mat4 Identity();

		Mat4& Multiply(const Mat4& other);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="other">The other Mat4 to multiply</param>
		/// <returns>The multiplied Mat4</returns>
		friend Mat4 operator*(Mat4 left, const Mat4& right);
		
		Vec3 Multiply(const Vec3& other) const;
		friend Vec3 operator*(const Mat4& left, const Vec3& right);

		Vec4 Multiply(const Vec4& other) const;
		friend Vec4 operator*(const Mat4& left, const Vec4& right);

		Mat4& Translate(const Vec3& other);
		Mat4& Rotate(float angle, const Vec3& other);
		Mat4& Scale(const Vec3& other);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="other">The other Mat4 to multiply</param>
		/// <returns>The multiplied Mat4</returns>
		Mat4& operator*=(const Mat4& other);

		//float& operator()(unsigned int row, unsigned int col);

		static Mat4 Orthographic(float left, float right, float bottom, float top, float near, float far);
		static Mat4 Perspective(float width, float height, float fov, float near, float far);

		static Mat4 TranslationMatrix(const Vec3& translation);
		static Mat4 RotationMatrix(float angle, const Vec3& axis);
		static Mat4 ScaleMatrix(const Vec3& scale);

		friend std::ostream& operator<<(std::ostream& stream, const Mat4& other)
		{
			stream << "mat4: (" << other.columns[0].x << ", " << other.columns[1].x << ", " << other.columns[2].x << ", " << other.columns[3].x << "), " << std::endl;
			stream << "(" << other.columns[0].y << ", " << other.columns[1].y << ", " << other.columns[2].y << ", " << other.columns[3].y << "), " << std::endl;
			stream << "(" << other.columns[0].z << ", " << other.columns[1].z << ", " << other.columns[2].z << ", " << other.columns[3].z << "), " << std::endl;
			stream << "(" << other.columns[0].w << ", " << other.columns[1].w << ", " << other.columns[2].w << ", " << other.columns[3].w << ")";
			return stream;
		}
	};
}
