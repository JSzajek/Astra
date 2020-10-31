#pragma once

#include "Light.h"

namespace Astra::Graphics
{
	class PointLight : public Light
	{
	private:
		Math::Vec3 m_attenuation;
	public:
		PointLight(const Math::Vec3& translation, const Math::Vec3& ambient,
				   const Math::Vec3& diffuse, const Math::Vec3& specular,
				   float constant = 1.0f, float linear = 0.09f, float quadratic = 0.035f)
			: Light(translation, ambient, diffuse, specular), m_attenuation(constant, linear, quadratic)
		{
			m_type = LightType::Point;
		}

		inline const Math::Vec3& GetAttenuation() const { return m_attenuation; }
	};
}