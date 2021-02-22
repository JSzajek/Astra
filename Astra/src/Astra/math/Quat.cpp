#include "astra_pch.h"

#include "Quat.h"

namespace Astra::Math
{
	Quat::Quat()
		: x(0), y(0), z(0), w(1)
	{
	}

	Quat::Quat(const Vec3& axis, float angle)
	{
		ASTRA_CORE_ASSERT(axis.IsNormalized(), "Quat: The Axis Vector is Not Normalized.");

		float dim = axis.Length();
		if (dim == 0)
		{
			x = y = z = w = 0;
		}
		else
		{
			float s = std::sinf(angle * 0.5f) / dim;
			x = axis.x * s;
			y = axis.y * s;
			z = axis.z * s;
			w = std::cosf(angle * 0.5f);
		}
	}

	Quat::Quat(float xQuat, float yQuat, float zQuat, float wQuat)
		: x(xQuat), y(yQuat), z(zQuat), w(wQuat)
	{
	}

	Quat::Quat(const Vec3& first, const Vec3& second)
	{
		auto cross = first.Cross(second);
		auto dot = first.Dot(second);
		if (dot < -1.0 + CMP_EPSILON)
		{
			x = z = w = 0;
			y = 1;
		}
		else
		{
			auto sqrt = std::sqrtf((1.0f + dot) * 2.0f);
			auto d_sqrt = 1.0f / sqrt;
			x = cross.x * d_sqrt;
			y = cross.y * d_sqrt;
			z = cross.z * d_sqrt;
			w = sqrt * 0.5f;
		}
	}

	Quat::Quat(const Quat& other)
		: x(other.x), y(other.y), z(other.z), w(other.w)
	{
	}

	float& Quat::operator[](int index)
	{
		ASTRA_ASSERT(index < 4, "Quat: Out Of Index Axis Access.");
		return m_data[index];
	}

	const float Quat::operator[](int index) const
	{
		ASTRA_ASSERT(index < 4, "Quat: Out Of Index Axis Access.");
		return m_data[index];
	}

	Quat& Quat::operator=(const Quat& r_val)
	{
		x = r_val.x;
		y = r_val.y;
		z = r_val.z;
		w = r_val.w;
		return *this;
	}

	void Quat::operator+=(const Quat& r_val)
	{
		x += r_val.x;
		y += r_val.y;
		z += r_val.z;
		w += r_val.w;
	}

	Quat Quat::operator+(const Quat& r_val) const
	{
		return Quat(x + r_val.x, y + r_val.y, z + r_val.z, w + r_val.w);
	}

	void Quat::operator-=(const Quat& r_val)
	{
		x -= r_val.x;
		y -= r_val.y;
		z -= r_val.z;
		w -= r_val.w;
	}

	Quat Quat::operator-(const Quat& r_val) const
	{
		return Quat(x - r_val.x, y - r_val.y, z - r_val.z, w - r_val.w);
	}

	void Quat::operator*=(const Quat& r_val)
	{
		x *= r_val.x;
		y *= r_val.y;
		z *= r_val.z;
		w *= r_val.w;
	}

	Quat Quat::operator*(const Quat& r_val) const
	{
		return Quat(x * r_val.x, y * r_val.y, z * r_val.z, w * r_val.w);
	}

	Quat Quat::operator*(const Vec3& r_val) const
	{
		return Quat(x * r_val.x + y * r_val.z - z * r_val.y,
					w * r_val.y + z * r_val.x - x * r_val.z,
					w * r_val.z + x * r_val.y - y * r_val.x,
				   -x * r_val.x - y * r_val.y - z * r_val.z);
	}

	Quat Quat::operator*(float r_val) const
	{
		return Quat(x * r_val, y * r_val, z * r_val, w * r_val);
	}

	void Quat::operator/=(float r_val)
	{
		x /= r_val;
		y /= r_val;
		z /= r_val;
		w /= r_val;
	}

	Quat Quat::operator/(float r_val) const
	{
		return (*this) * (1.0f / r_val);
	}

	Quat Quat::operator-() const 
	{
		return Quat(-x, -y, -z, -w);
	}

	bool Quat::operator==(const Quat& r_val) const
	{
		return x == r_val.x && y == r_val.y && z == r_val.z && w == r_val.w;
	}

	bool Quat::operator!=(const Quat& r_val) const
	{
		return x != r_val.x || y != r_val.y || z != r_val.z || w != r_val.w;
	}

	float Quat::Dot(const Quat& r_val) const
	{
		return x * r_val.x + y * r_val.y + z * r_val.z + w * r_val.w;
	}

	void Quat::Normalize()
	{
		*this /= Length();
	}

	Quat Quat::Normalized() const
	{
		return *this / Length();
	}

	bool Quat::IsNormalized() const
	{
		return Math::IsEqualApprox(LengthSquared(), 1.0, UNIT_EPSILON);
	}

	float Quat::Length() const
	{
		return std::sqrtf(LengthSquared());
	}

	float Quat::LengthSquared() const
	{
		return x * x + y * y + z * z + w * w;
	}

	Quat Quat::Inverse() const
	{
		ASTRA_CORE_ASSERT(IsNormalized(), "Quat: Must Be Normalized");

		return Quat(-x, -y, -z, -w);
	}

	Quat Quat::Slerp(const Quat& r_val, const float weight) const
	{
		ASTRA_CORE_ASSERT(IsNormalized(), "Quat: The Beginning Quat Must Be Normalized");
		ASTRA_CORE_ASSERT(r_val.IsNormalized(), "Quat: The Ending Quat Must Be Normalized");
	
		Quat end;
		float omega, cosom, sinom, scale0, scale1;

		cosom = Dot(r_val);
		if (cosom < 0.0f)
		{
			cosom = -cosom;
			end.x = -r_val.x;
			end.y = -r_val.y;
			end.z = -r_val.z;
			end.w = -r_val.w;
		}
		else
		{
			end.x = r_val.x;
			end.y = r_val.y;
			end.z = r_val.z;
			end.w = r_val.w;
		}

		if ((1.0f - cosom) > CMP_EPSILON)
		{
			omega = std::acosf(cosom);
			sinom = std::sinf(omega);
			scale0 = std::sinf((1.0f - weight) * omega) / sinom;
			scale1 = std::sinf(weight * omega) / sinom;
		}
		else
		{
			scale0 = 1.0f - weight;
			scale1 = weight;
		}

		return Quat(scale0 * x + scale1 * end.x,
					scale0 * y + scale1 * end.y,
					scale0 * z + scale1 * end.z,
					scale0 * w + scale1 * end.w);
	}

	bool Quat::IsEqualApprox(const Quat& val) const
	{
		return Math::IsEqualApprox(x, val.x) && Math::IsEqualApprox(y, val.y) && Math::IsEqualApprox(z, val.z) && Math::IsEqualApprox(w, val.w);
	}

	Vec3 Quat::XForm(const Vec3& vec) const
	{
		ASTRA_CORE_ASSERT(IsNormalized(), "Quat: Must Be Normalized");

		Vec3 temp(x, y, z);
		Vec3 cross = temp.Cross(vec);
		return temp + ((cross * w) + temp.Cross(cross)) * 2.0f;
	}

	Vec3 Quat::XFormInverted(const Vec3& vec) const
	{
		return Inverse().XForm(vec);
	}
}