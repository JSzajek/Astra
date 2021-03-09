#pragma once

#include <string>

#include "MathsUtils.h"

#include "Vec3.h"
#include "Vec4.h"
#include "Quat.h"

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
		Mat4(const Mat4& other);
		Mat4(const Vec3& translation, const Quat& rotation, const Vec3& scale);
		Mat4(float v00, float v01, float v02, float v03,
			 float v10, float v11, float v12, float v13,
			 float v20, float v21, float v22, float v23,
			 float v30, float v31, float v32, float v33);

		Mat4(const Vec4& vals);

		void operator=(const Mat4&);

		static Mat4 Identity();

		void SetIdentity();

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

		Mat4 operator+(const Mat4& r_val) const;
		void operator+=(const Mat4& r_val);

		/// <summary>
		/// Multiplication operator override.
		/// </summary>
		/// <param name="other">The other Mat4 to multiply</param>
		/// <returns>The multiplied Mat4</returns>
		Mat4& operator*=(const Mat4& other);

		Mat4& Translate(const Vec3& other);
		Mat4& Rotate(float angle, const Vec3& other);
		Mat4& Scale(const Vec3& other);
		Mat4& Scale(float scale);

		const Mat4 Inverse() const;
		void Transpose();

		static Mat4 Orthographic(float left, float right, float bottom, float top, float _near, float _far);
		static Mat4 Perspective(float width, float height, float fov, float _near, float _far);

		static Mat4 TranslationMatrix(const Vec3& translation);
		static Mat4 RotationMatrix(float angle, const Vec3& axis);
		static Mat4 RotationMatrix(const Quat& quat);
		static Mat4 ScaleMatrix(const Vec3& scale);
		static Mat4 ScaleMatrix(float scale);

		#define TOSTRING_MAT4(x_0, y_0, z_0, w_0, x_1, y_1, z_1, w_1, x_2, y_2, z_2, w_2, x_3, y_3, z_3, w_3) "[(" + std::to_string(x_0) + ", " + std::to_string(y_0) + ", " + std::to_string(z_0) + ", " + std::to_string(w_0) + ")\n(" \
																												   + std::to_string(x_1) + ", " + std::to_string(y_1) + ", " + std::to_string(z_1) + ", " + std::to_string(w_1) + ")\n(" \
																												   + std::to_string(x_2) + ", " + std::to_string(y_2) + ", " + std::to_string(z_2) + ", " + std::to_string(w_2) + ")\n(" \
																												   + std::to_string(x_3) + ", " + std::to_string(y_3) + ", " + std::to_string(z_3) + ", " + std::to_string(w_3) + ")]"
		
		
		
		inline std::string ToString() const  { return TOSTRING_MAT4(columns[0].x, columns[0].y, columns[0].z, columns[0].w,
									                                columns[1].x, columns[1].y, columns[1].z, columns[1].w, 
									                                columns[2].x, columns[2].y, columns[2].z, columns[2].w, 
									                                columns[3].x, columns[3].y, columns[3].z, columns[3].w); 
		}

		friend std::ostream& operator<<(std::ostream& stream, const Mat4& other)
		{
			stream << other.ToString();
			return stream;
		}
	private:
		void SetDiagonal(float diagonal);
	};
}
