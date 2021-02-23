#pragma once

#include "Light.h"
#include "Astra/Core.h"

namespace Astra::Graphics
{
	class PointLight : public Light
	{
	public:
		PointLight(const Math::Vec3& translation, const Math::Vec3& ambient, 
				   const Math::Vec3& diffuse, const Math::Vec3& specular,
				   float constant = 1.0f, float linear = 0.09f, float quadratic = 0.035f)
			: Light(translation, ambient, diffuse, specular)
		{
			m_attenuation = Math::Vec3(constant, linear, quadratic);
		#if ASTRA_DEBUG
			m_gizmo = new Gizmo(RESOURCE("res/textures/PointLight.png"), translation, 3);
		#endif
		}

		PointLight(const char* const name, const Math::Vec3& translation, const Math::Vec3& ambient,
				   const Math::Vec3& diffuse, const Math::Vec3& specular,
				   float constant = 1.0f, float linear = 0.09f, float quadratic = 0.035f)
			: PointLight(translation, ambient, diffuse, specular, constant, linear, quadratic)
		{
			Name = name;
		}

		inline virtual LightType GetType() const override { return LightType::Point; }
		inline virtual bool IsDirectional() const override { return false; }

		inline virtual std::string ToString() const override { return !Name.length() ? ("Point_Light_&" + std::to_string(m_uid)) : Name; }
	};
}