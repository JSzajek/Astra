#include "Mat4.h"

namespace Astra::Math
{
	Mat4::Mat4()
	{
		memset(data, 0, 4 * 4 * sizeof(float));
	}

	Mat4::Mat4(float diagonal)
	{
		for (int i = 0; i < 4 * 4; i++)
		{
			data[i] = 0;
		}

		data[0] = diagonal;
		data[1 + 1 * 4] = diagonal;
		data[2 + 2 * 4] = diagonal;
		data[3 + 3 * 4] = diagonal;
	}

	Mat4::Mat4(const Mat4& other)
	{
		for (int i = 0; i < 4 * 4; i++)
		{
			data[i] = other.data[i];
		}
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

	Mat4& Mat4::Translate(const Vec3& other)
	{
		Mat4 translationMatrix = TranslationMatrix(other);
		return Multiply(translationMatrix);

		/*columns[3].x = columns[0].x * other.x + columns[1].x * other.y + columns[2].x * other.z + columns[3].x;
		columns[3].y = columns[0].y * other.x + columns[1].y * other.y + columns[2].y * other.z + columns[3].y;
		columns[3].z = columns[0].z * other.x + columns[1].z * other.y + columns[2].z * other.z + columns[3].z;
		return *this;*/
	}

	Mat4& Mat4::Rotate(float angle, const Vec3& other)
	{
		/*Mat4 rotationMat = RotationMatrix(angle, other);
		return Multiply(rotationMat);*/

		Multiply(RotationMatrix(angle, other));
		/*Mat4 tempMat(*this);
		columns[0] = tempMat.columns[0] * rotate.columns[0][0] + tempMat.columns[1] * rotate.columns[0][1] + tempMat.columns[2] * rotate.columns[0][2];
		columns[1] = tempMat.columns[0] * rotate.columns[1][0] + tempMat.columns[1] * rotate.columns[1][1] + tempMat.columns[2] * rotate.columns[1][2];
		columns[2] = tempMat.columns[0] * rotate.columns[2][0] + tempMat.columns[1] * rotate.columns[2][1] + tempMat.columns[2] * rotate.columns[2][2];
		columns[3] = tempMat.columns[3];*/

		return *this;
	}

	Mat4& Mat4::Scale(const Vec3& other)
	{
		Multiply(ScaleMatrix(other));
		/*columns[0] = columns[0] * other.x;
		columns[1] = columns[1] * other.y;
		columns[2] = columns[2] * other.z;*/
		return *this;
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

	Mat4 Mat4::Orthographic(float left, float right, float bottom, float top, float near, float far)
	{
		Mat4 result(1.0f);
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
		Mat4 result(1.0f);

		float aspectRatio = width / height;
		float y_scale = (1.0f / tan(Math::ToRadians(fov / 2.0f)) * aspectRatio);
		float x_scale = y_scale / aspectRatio;
		float fulstrumLength = far - near;
		
		result.columns[0].x = x_scale;
		result.columns[1].y = y_scale;
		result.columns[2].z = -((far + near) / fulstrumLength);
		result.columns[2].w = -1;
		result.columns[3].z = -((2 * near * far) / fulstrumLength);
		return result;
	}

	/*Mat4 Mat4::Perspective(float fov, float aspectRatio, float near, float far)
	{
		Mat4 result(1.0f);
		float q = 1.0f / tan(ToRadians(0.5f * fov));
		float a = q / aspectRatio;
		float b = (near + far) / (near - far);
		float c = (2.0f * near * far) / (near - far);

		result.data[0 + 0 * 4] = a;
		result.data[1 + 1 * 4] = q;
		result.data[2 + 2 * 4] = b;
		result.data[3 + 2 * 4] = -1.0f;
		result.data[2 + 3 * 4] = c;
		
		return result;
	}*/

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
		const float c = cos(angle);
		const float s = sin(angle);

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

		/*Mat4 result(1.0f);
		float rad = ToRadians(angle);
		float c = cos(rad);
		float c_inv = 1.0f - c;
		float s = sin(rad);

		float x = axis.x;
		float y = axis.y;
		float z = axis.z;

		result.data[0 + 0 * 4] = x * x * c_inv + c;
		result.data[0 + 1 * 4] = y * x * c_inv + z * s;
		result.data[0 + 2 * 4] = x * z * c_inv - y * s;

		result.data[1 + 0 * 4] = x * y * c_inv - z * s;
		result.data[1 + 1 * 4] = y * y * c_inv + c;
		result.data[1 + 2 * 4] = y * z * c_inv + x * s;

		result.data[2 + 0 * 4] = x * z * c_inv + y * s;
		result.data[2 + 1 * 4] = y * z * c_inv - x * s;
		result.data[2 + 2 * 4] = z * z * c_inv + c;
		*/
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
}