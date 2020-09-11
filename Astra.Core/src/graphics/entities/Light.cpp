#include "Light.h"

namespace Astra::Graphics
{
	Light::Light(const Math::Vec3& translation, const Math::Vec3& color, const Math::Vec3& attenuation)
		: Spatial(translation), m_color(color), m_attenuation(attenuation)
	{
	}

	Light::Light(const Light& other)
		: m_color(other.m_color), m_attenuation(other.m_attenuation)
	{
		memcpy(data, other.data, 3 * 3 * sizeof(float));
	}


}