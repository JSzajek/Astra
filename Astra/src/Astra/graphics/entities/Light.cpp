#include "astra_pch.h"

#include "Light.h"

namespace Astra::Graphics
{
	Light::Light(const Math::Vec3& translation, const Math::Vec3& color)
		: Spatial(translation), m_color(color), m_ambient(Math::Vec3::Zero), m_diffuse(Math::Vec3::Zero), m_specular(Math::Vec3::Zero),
			m_attenuation(Math::Vec3::Zero), m_cutOff(0), m_outerCutOff(0), m_gizmo(NULL)
	{
	}

	Light::Light(const Light& other)
		: Spatial(other), m_color(other.m_color), m_ambient(other.m_ambient), m_diffuse(other.m_diffuse), 
			m_specular(other.m_specular), m_attenuation(other.m_attenuation), m_cutOff(other.m_cutOff), m_outerCutOff(other.m_outerCutOff)
	#if ASTRA_DEBUG
		, m_gizmo(other.m_gizmo)
	#endif
	{
		memcpy(m_data, other.m_data, 3 * 3 * sizeof(float));
	}

	Light::Light(const Math::Vec3& translation, const Math::Vec3& ambient,
					const Math::Vec3& diffuse, const Math::Vec3& specular)
		: Spatial(translation), m_color(Math::Vec3::Zero), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular),
			m_attenuation(Math::Vec3::Zero), m_cutOff(0), m_outerCutOff(0), m_gizmo(NULL)
	{
	}

	void Light::Free()
	{

	}
}