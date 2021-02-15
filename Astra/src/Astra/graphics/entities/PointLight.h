#pragma once

#include "Light.h"
#include "../gizmos/Gizmo.h"

namespace Astra::Graphics
{
	class PointLight : public Light
	{
	private:
	#if ASTRA_DEBUG
		const Gizmo* m_gizmo;
	#endif
		Math::Vec3 m_attenuation;
	public:
		PointLight(const Math::Vec3& translation, const Math::Vec3& ambient,
				   const Math::Vec3& diffuse, const Math::Vec3& specular,
				   float constant = 1.0f, float linear = 0.09f, float quadratic = 0.035f)
			: Light(translation, ambient, diffuse, specular), m_attenuation(constant, linear, quadratic)
		{
			m_type = LightType::Point;
		#if ASTRA_DEBUG
			m_gizmo = new Gizmo(RESOURCE("res/textures/PointLight.png"), translation, 3);
		#endif
		}
	#if ASTRA_DEBUG
		~PointLight()
		{
			delete m_gizmo;
		}
		inline const Gizmo* const GetGizmo() const { return m_gizmo; }
	#endif

		inline const Math::Vec3& GetAttenuation() const { return m_attenuation; }
	};
}