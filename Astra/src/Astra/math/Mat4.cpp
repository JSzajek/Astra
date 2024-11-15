#include "astra_pch.h"

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

	Mat4::Mat4(const Vec3& translation, const Quat& rotation, const Vec3& scale)
	{
		// Calc Coefficients
		auto xx = rotation.x * rotation.x;	auto yy = rotation.y * rotation.y;	auto zz = rotation.z * rotation.z;
		auto xy = rotation.x * rotation.y;	auto yz = rotation.y * rotation.z;	auto zw = rotation.z * rotation.w;
		auto xz = rotation.x * rotation.z;	auto yw = rotation.y * rotation.w;
		auto xw = rotation.x * rotation.w;

		columns[0][0] = (1.0f) - (2.0f) * (yy + zz) * scale.x;	//rot00 * xScale
		columns[1][0] = (2.0f) * (xy - zw) * scale.x;			//rot10 * xScale
		columns[2][0] = (2.0f) * (xz + yw) * scale.x;			//rot20 * xScale
		columns[3][0] = translation.x;

		columns[0][1] = (2.0f) * (xy + zw) * scale.y;			//rot01 * yScale
		columns[1][1] = (1.0f) - (2.0f) * (xx + zz) * scale.y;	//rot11 * yScale
		columns[2][1] = (2.0f) * (yz - xw) * scale.y;			//rot21 * yScale
		columns[3][1] = translation.y;

		columns[0][2] = (2.0f) * (xz - yw) * scale.z;			//rot02 * zScale
		columns[1][2] = (2.0f) * (yz + xw) * scale.z;			//rot12 * zScale
		columns[2][2] = (1.0f) - (2.0f) * (xx + yy) * scale.z;	//rot22 * zScale
		columns[3][2] = translation.z;

		columns[0][3] = columns[1][3] = columns[2][3] = 0.0f;
		columns[3][3] = 1.0f;
	}

	Mat4::Mat4(float v00, float v01, float v02, float v03,
		       float v10, float v11, float v12, float v13,
		       float v20, float v21, float v22, float v23,
		       float v30, float v31, float v32, float v33)
	{
		columns[0][0] = v00;
		columns[0][1] = v01;
		columns[0][2] = v02;
		columns[0][3] = v03;

		columns[1][0] = v10;
		columns[1][1] = v11;
		columns[1][2] = v12;
		columns[1][3] = v13;

		columns[2][0] = v20;
		columns[2][1] = v21;
		columns[2][2] = v22;
		columns[2][3] = v23;

		columns[3][0] = v30;
		columns[3][1] = v31;
		columns[3][2] = v32;
		columns[3][3] = v33;
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

	const Mat4 Mat4::Inverse() const
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

	Mat4 Mat4::Orthographic(float left, float right, float bottom, float top, float _near, float _far)
	{
		Mat4 result;
		result.data[0 + 0 * 4] = 2.0f / (right - left);
		result.data[1 + 1 * 4] = 2.0f / (top - bottom);
		result.data[2 + 2 * 4] = -2.0f / (_far - _near);

		result.data[0 + 3 * 4] = -((right + left) / (right - left));
		result.data[1 + 3 * 4] = -((top + bottom) / (top - bottom));
		result.data[2 + 3 * 4] = -((_far + _near) / (_far - _near));
		result.data[3 + 3 * 4] = 1;
		return result;
	}

	Mat4 Mat4::Perspective(float width, float height, float fov, float _near, float _far)
	{
		Mat4 result;
		float aspectRatio = width / height;
		float y_scale = 1.0f / tan(Math::ToRadians(fov / 2.0f));
		float x_scale = y_scale / aspectRatio;
		float fulstrumLength = _far - _near;
		
		result.columns[0][0] = x_scale;
		result.columns[1][1] = y_scale;
		result.columns[2][2] = -((_far + _near) / fulstrumLength);
		result.columns[2][3] = -1;
		result.columns[3][2] = -((2 * _near * _far) / fulstrumLength);
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

	Mat4 Mat4::RotationMatrix(const Quat& quat)
	{
		// Equations based on https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
		Mat4 result(1);

		// Calc Coefficients
		auto xx = quat.x * quat.x;	auto yy = quat.y * quat.y;	auto zz = quat.z * quat.z;
		auto xy = quat.x * quat.y;	auto yz = quat.y * quat.z;	auto zw = quat.z * quat.w;
		auto xz = quat.x * quat.z;	auto yw = quat.y * quat.w;
		auto xw = quat.x * quat.w;

		result.columns[0][0] = 1.0f - 2.0f * (yy + zz);
		result.columns[0][1] = 2.0f * (xy - zw);
		result.columns[0][2] = 2.0f * (xz + yw);

		result.columns[1][0] = 2.0f * (xy + zw);
		result.columns[1][1] = 1.0f - 2.0f * (xx + zz);
		result.columns[1][2] = 2.0f * (yz - xw);

		result.columns[2][0] = 2.0f * (xz - yw);
		result.columns[2][1] = 2.0f * (yz + xw);
		result.columns[2][2] = 1.0f - 2.0f * (xx + yy);

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