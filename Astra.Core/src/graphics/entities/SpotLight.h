#pragma once

#include "Light.h"

namespace Astra::Graphics
{
	class SpotLight : public Light
	{
	private:
		float m_cutOff;
		float m_outerCutOff;
	public:
		SpotLight(const Math::Vec3& translation, const Math::Vec3& direction, const Math::Vec3& ambient,
				  const Math::Vec3& diffuse, const Math::Vec3& specular, float cutOff, float outerCutOff)
			: Light(translation, ambient, diffuse, specular), m_cutOff(cutOff), m_outerCutOff(outerCutOff)
		{
			data[3] = direction.x;
			data[4] = direction.y;
			data[5] = direction.z;
			m_type = LightType::Spotlight;
		}

		inline float GetCutOff() const { return m_cutOff; }
		inline float GetOuterCutOff() const { return m_outerCutOff; }
	};
}