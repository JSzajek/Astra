#pragma once

#include "Spatial.h"

namespace Astra::Graphics
{
	class Light : public Spatial
	{
	private:
		Math::Vec3 m_color;
		Math::Vec3 m_attenuation;
	public:
		Light(const Math::Vec3& translation, const Math::Vec3& color, const Math::Vec3& attenuation = Math::Vec3(1,0,0))
			: Spatial(translation), m_color(color), m_attenuation(attenuation)
		{
		}

		Light(const Light& other)
			: m_color(other.m_color), m_attenuation(other.m_attenuation)
		{
			memcpy(data, other.data, 3 * 3 * sizeof(float));
		}

		inline const Math::Vec3& GetColor() const { return m_color; }
		inline const Math::Vec3& GetAttenuation() const { return m_attenuation; }
	};
}