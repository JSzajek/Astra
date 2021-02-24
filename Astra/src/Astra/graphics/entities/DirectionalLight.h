#pragma once

#include "Light.h"
#include "Astra/Core.h"

namespace Astra::Graphics
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight()
			: Light(Math::Vec3::Zero, Math::Vec3(1), Math::Vec3(1), Math::Vec3(1))
		{
		#if ASTRA_DEBUG
			m_gizmo = Gizmo(RESOURCE("res/textures/DirectionalLight.png"), Math::Vec3::Zero, 3);
		#endif
		}

		DirectionalLight(const Math::Vec3& position, const Math::Vec3& direction, 
						 const Math::Vec3& ambient, const Math::Vec3& diffuse, 
						 const Math::Vec3& specular)
			: Light(position, ambient, diffuse, specular)
		{
			m_data[3] = direction.x;
			m_data[4] = direction.y;
			m_data[5] = direction.z;
		#if ASTRA_DEBUG
			m_gizmo = Gizmo(RESOURCE("res/textures/DirectionalLight.png"), position, 3);
		#endif
		}

		DirectionalLight(const char* const name, const Math::Vec3& position, const Math::Vec3& direction,
						 const Math::Vec3& ambient, const Math::Vec3& diffuse,
						 const Math::Vec3& specular)
			: DirectionalLight(position, direction, ambient, diffuse, specular)
		{
			Name = name;
		}

		inline virtual LightType GetType() const override { return LightType::Directional; }
		inline virtual bool IsDirectional() const override { return true; }

		inline virtual std::string ToString() const override { return !Name.length() ? ("Directional_Light_&" + std::to_string(m_uid)) : Name; }
	};
}