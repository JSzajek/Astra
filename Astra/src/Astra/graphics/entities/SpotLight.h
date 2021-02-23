#pragma once

#include "Light.h"
#include "Astra/Core.h"

namespace Astra::Graphics
{
	class SpotLight : public Light
	{
	public:
		SpotLight(const Math::Vec3& translation, const Math::Vec3& direction, const Math::Vec3& ambient,
				  const Math::Vec3& diffuse, const Math::Vec3& specular, float cutOff, float outerCutOff)
			: Light(translation, ambient, diffuse, specular)
		{
			m_cutOff = cutOff;
			m_outerCutOff = outerCutOff;
			m_data[3] = direction.x;
			m_data[4] = direction.y;
			m_data[5] = direction.z;
		#if ASTRA_DEBUG
			m_gizmo = new Gizmo(RESOURCE("res/textures/SpotLight.png"), translation, 3);
		#endif
		}
		inline virtual LightType GetType() const override { return LightType::Spotlight; }
		inline virtual bool IsDirectional() const override { return true; }
	};
}