#include "Light.h"

namespace Astra::Graphics
{
	Light::Light(const Math::Vec3& translation, const Math::Vec3& color)
		: Spatial(translation), m_color(color), m_ambient(0), m_diffuse(0), m_specular(0)
	{
	}

	Light::Light(const Light& other)
		: m_color(other.m_color), m_ambient(other.m_ambient), m_diffuse(other.m_diffuse), m_specular(other.m_specular)
	{
		memcpy(m_data, other.m_data, 3 * 3 * sizeof(float));
	}

	Light::Light(const Math::Vec3& translation, const Math::Vec3& ambient,
					const Math::Vec3& diffuse, const Math::Vec3& specular)
		: Spatial(translation), m_color(0), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular)
	{
	}
}