#include "Mat4.h"

namespace Astra::Math
{
	Mat4::Mat4()
	{
		memset(data, 0, 4 * 4 * sizeof(float));
	}

	Mat4::Mat4(float diagonal)
	{
		memset(data, 0, 4 * 4 * sizeof(float));
		SetDiagonal(diagonal);
	}

	Mat4::Mat4(const Mat4& other)
	{
		memcpy(data, other.data, 16 * sizeof(float));
	}

	void Mat4::operator=(const Mat4& other)
	{
		memcpy(data, other.data, 16 * sizeof(float));
	}

	void Mat4::SetIdentity()
	{
		memset(data, 0, 4 * 4 * sizeof(float));
		SetDiagonal(1);
	}

	Mat4 Mat4::Identity()
	{
		return Mat4(1.0f);
	}

	Mat4& Mat4::Multiply(const Mat4& other)
	{
		float result[4 * 4];
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				float sum = 0;
				for (int d = 0; d < 4; d++)
				{
					sum += data[x + d * 4] * other.data[d + y * 4];
				}
				result[x + y * 4] = sum;
			}
		}
		memcpy(data, result, 4 * 4 * sizeof(float));
		return *this;
	}

	Vec4 Mat4::Multiply(const Vec4& other) const
	{
		return Vec4(
			columns[0].x * other.x + columns[1].x * other.y + columns[2].x * other.z + columns[3].x * other.w,
			columns[0].y * other.x + columns[1].y * other.y + columns[2].y * other.z + columns[3].y * other.w,
			columns[0].z * other.x + columns[1].z * other.y + columns[2].z * other.z + columns[3].z * other.w,
			columns[0].w * other.x + columns[1].w * other.y + columns[2].w * other.z + columns[3].w * other.w
		);
	}

	Vec3 Mat4::Multiply(const Vec3& other) const
	{
		return Vec3(
			columns[0].x * other.x + columns[1].x * other.y + columns[2].x * other.z + columns[3].x,
			columns[0].y * other.x + columns[1].y * other.y + columns[2].y * other.z + columns[3].y,
			columns[0].z * other.x + columns[1].z * other.y + columns[2].z * other.z + columns[3].z
		);
	}

	Vec4 operator*(const Mat4& left, const Vec4& right)
	{
		return left.Multiply(right);
	}

	Vec3 operator*(const Mat4& left, const Vec3& right)
	{
		return left.Multiply(right);
	}

	/// <summary>
	/// Multiplication operator override.
	/// </summary>
	/// <param name="other">The other Mat4 to multiply</param>
	/// <returns>The multiplied Mat4</returns>
	Mat4 operator*(Mat4 left, const Mat4& right)
	{
		return left.Multiply(right);
	}

	/// <summary>
	/// Multiplication operator override.
	/// </summary>
	/// <param name="other">The other Mat4 to multiply</param>
	/// <returns>The multiplied Mat4</returns>
	Mat4& Mat4::operator*=(const Mat4& other)
	{
		return Multiply(other);
	}

	Mat4& Mat4::Translate(const Vec3& other)
	{
		Mat4 translationMatrix = TranslationMatrix(other);
		return Multiply(translationMatrix);
	}

	Mat4& Mat4::Rotate(float angle, const Vec3& other)
	{
		Multiply(RotationMatrix(angle, other));
		return *this;
	}

	Mat4& Mat4::Scale(const Vec3& other)
	{
		Multiply(ScaleMatrix(other));
		return *this;
	}

	Mat4& Mat4::Scale(float scale)
	{
		Multiply(ScaleMatrix(scale));
		return *this;
	}

	const Mat4& Mat4::Inverse() const
	{
		Mat4 inverse(0);

		inverse.data[0] = data[5] * data[10] * data[15] -
						  data[5] * data[11] * data[14] -
						  data[9] * data[6] * data[15] +
						  data[9] * data[7] * data[14] +
						  data[13] * data[6] * data[11] -
						  data[13] * data[7] * data[10];

		inverse.data[4] = -data[4] * data[10] * data[15] +
						   data[4] * data[11] * data[14] +
						   data[8] * data[6] * data[15] -
						   data[8] * data[7] * data[14] -
						   data[12] * data[6] * data[11] +
						   data[12] * data[7] * data[10];

		inverse.data[8] = data[4] * data[9] * data[15] -
						  data[4] * data[11] * data[13] -
						  data[8] * data[5] * data[15] +
						  data[8] * data[7] * data[13] +
						  data[12] * data[5] * data[11] -
						  data[12] * data[7] * data[9];

		inverse.data[12] = -data[4] * data[9] * data[14] +
							data[4] * data[10] * data[13] +
							data[8] * data[5] * data[14] -
							data[8] * data[6] * data[13] -
							data[12] * data[5] * data[10] +
							data[12] * data[6] * data[9];

		inverse.data[1] = -data[1] * data[10] * data[15] +
						   data[1] * data[11] * data[14] +
						   data[9] * data[2] * data[15] -
						   data[9] * data[3] * data[14] -
						   data[13] * data[2] * data[11] +
						   data[13] * data[3] * data[10];

		inverse.data[5] = data[0] * data[10] * data[15] -
						  data[0] * data[11] * data[14] -
						  data[8] * data[2] * data[15] +
						  data[8] * data[3] * data[14] +
						  data[12] * data[2] * data[11] -
						  data[12] * data[3] * data[10];

		inverse.data[9] = -data[0] * data[9] * data[15] +
						   data[0] * data[11] * data[13] +
						   data[8] * data[1] * data[15] -
						   data[8] * data[3] * data[13] -
						   data[12] * data[1] * data[11] +
						   data[12] * data[3] * data[9];

		inverse.data[13] = data[0] * data[9] * data[14] -
						   data[0] * data[10] * data[13] -
						   data[8] * data[1] * data[14] +
						   data[8] * data[2] * data[13] +
						   data[12] * data[1] * data[10] -
						   data[12] * data[2] * data[9];

		inverse.data[2] = data[1] * data[6] * data[15] -
						  data[1] * data[7] * data[14] -
						  data[5] * data[2] * data[15] +
						  data[5] * data[3] * data[14] +
						  data[13] * data[2] * data[7] -
						  data[13] * data[3] * data[6];

		inverse.data[6] = -data[0] * data[6] * data[15] +
						   data[0] * data[7] * data[14] +
						   data[4] * data[2] * data[15] -
						   data[4] * data[3] * data[14] -
						   data[12] * data[2] * data[7] +
						   data[12] * data[3] * data[6];

		inverse.data[10] = data[0] * data[5] * data[15] -
						   data[0] * data[7] * data[13] -
						   data[4] * data[1] * data[15] +
						   data[4] * data[3] * data[13] +
						   data[12] * data[1] * data[7] -
						   data[12] * data[3] * data[5];

		inverse.data[14] = -data[0] * data[5] * data[14] +
							data[0] * data[6] * data[13] +
							data[4] * data[1] * data[14] -
							data[4] * data[2] * data[13] -
							data[12] * data[1] * data[6] +
							data[12] * data[2] * data[5];

		inverse.data[3] = -data[1] * data[6] * data[11] +
						   data[1] * data[7] * data[10] +
						   data[5] * data[2] * data[11] -
						   data[5] * data[3] * data[10] -
						   data[9] * data[2] * data[7] +
						   data[9] * data[3] * data[6];

		inverse.data[7] = data[0] * data[6] * data[11] -
						  data[0] * data[7] * data[10] -
						  data[4] * data[2] * data[11] +
						  data[4] * data[3] * data[10] +
						  data[8] * data[2] * data[7] -
						  data[8] * data[3] * data[6];

		inverse.data[11] = -data[0] * data[5] * data[11] +
							data[0] * data[7] * data[9] +
							data[4] * data[1] * data[11] -
							data[4] * data[3] * data[9] -
							data[8] * data[1] * data[7] +
							data[8] * data[3] * data[5];

		inverse.data[15] = data[0] * data[5] * data[10] -
						   data[0] * data[6] * data[9] -
						   data[4] * data[1] * data[10] +
						   data[4] * data[2] * data[9] +
						   data[8] * data[1] * data[6] -
						   data[8] * data[2] * data[5];

		float det = data[0] * inverse.data[0] + data[1] * inverse.data[4] + data[2] * inverse.data[8] + data[3] * inverse.data[12];
		det = 1.0f / det;
		for (int i = 0; i < 16; i++)
		{
			inverse.data[i] *= det;
		}
		return inverse;
	}

	void Mat4::Transpose()
	{
		float result[4 * 4];
		result[0]  = data[0];
		result[1]  = data[4];
		result[2]  = data[8];
		result[3]  = data[12];
		result[4]  = data[1];
		result[5]  = data[5];
		result[6]  = data[9];
		result[7]  = data[13];
		result[8]  = data[2];
		result[9]  = data[6];
		result[10] = data[10];
		result[11] = data[14];
		result[12] = data[3];
		result[13] = data[7];
		result[14] = data[11];
		result[15] = data[15];
		memcpy(data, result, 4 * 4 * sizeof(float));
	}

	Mat4 Mat4::Orthographic(float left, float right, float bottom, float top, float near, float far)
	{
		Mat4 result;
		result.data[0 + 0 * 4] = 2.0f / (right - left);
		result.data[1 + 1 * 4] = 2.0f / (top - bottom);
		result.data[2 + 2 * 4] = 2.0f / (near - far);

		result.data[0 + 3 * 4] = (left + right) / (left - right);
		result.data[1 + 3 * 4] = (bottom + top) / (bottom - top);
		result.data[2 + 3 * 4] = (far + near) / (far - near);
		return result;
	}

	Mat4 Mat4::Perspective(float width, float height, float fov, float near, float far)
	{
		Mat4 result;
		float aspectRatio = width / height;
		float y_scale = 1.0f / tan(Math::ToRadians(fov / 2.0f));
		float x_scale = y_scale / aspectRatio;
		float fulstrumLength = far - near;
		
		result.columns[0][0] = x_scale;
		result.columns[1][1] = y_scale;
		result.columns[2][2] = -((far + near) / fulstrumLength);
		result.columns[2][3] = -1;
		result.columns[3][2] = -((2 * near * far) / fulstrumLength);
		result.columns[3][3] = 0;
		return result;
	}

	Mat4 Mat4::TranslationMatrix(const Vec3& translation)
	{
		Mat4 result(1.0f);

		result.data[0 + 3 * 4] = translation.x;
		result.data[1 + 3 * 4] = translation.y;
		result.data[2 + 3 * 4] = translation.z;
		
		return result;
	}

	Mat4 Mat4::RotationMatrix(float angle, const Vec3& axis)
	{
		Mat4 result(1);
		float rad = ToRadians(angle);
		const float c = cos(rad);
		const float s = sin(rad);

		Vec3 normAxis = Vec3(axis);
		normAxis.Normalize();

		Vec3 temp(axis);
		temp *= (1 - c);

		result.columns[0][0] = c + temp[0] * normAxis[0];
		result.columns[0][1] = temp[0] * normAxis[1] + s * normAxis[2];
		result.columns[0][2] = temp[0] * normAxis[2] - s * normAxis[1];
		
		result.columns[1][0] = temp[1] * normAxis[0] - s * normAxis[2];
		result.columns[1][1] = c + temp[1] * normAxis[1];
		result.columns[1][2] = temp[1] * normAxis[2] + s * normAxis[0];
		
		result.columns[2][0] = temp[2] * normAxis[0] + s * normAxis[1];
		result.columns[2][1] = temp[2] * normAxis[1] - s * normAxis[0];
		result.columns[2][2] = c + temp[2] * normAxis[2];
		return result;
	}

	Mat4 Mat4::ScaleMatrix(float scale)
	{
		Mat4 result(1.0f);

		result.data[0 + 0 * 4] = scale;
		result.data[1 + 1 * 4] = scale;
		result.data[2 + 2 * 4] = scale;
		
		return result;
	}

	Mat4 Mat4::ScaleMatrix(const Vec3& scale)
	{
		Mat4 result(1.0f);

		result.data[0 + 0 * 4] = scale.x;
		result.data[1 + 1 * 4] = scale.y;
		result.data[2 + 2 * 4] = scale.z;

		return result;
	}

	void Mat4::SetDiagonal(float diagonal)
	{
		data[0] = diagonal;
		data[1 + 1 * 4] = diagonal;
		data[2 + 2 * 4] = diagonal;
		data[3 + 3 * 4] = diagonal;
	}
}