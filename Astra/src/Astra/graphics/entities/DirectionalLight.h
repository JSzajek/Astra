#pragma once

#include "Light.h"
#include "../gizmos/Gizmo.h"

namespace Astra::Graphics
{
	class DirectionalLight : public Light
	{
	private:
	#if ASTRA_DEBUG
		const Gizmo* m_gizmo;
	#endif
	public:
		DirectionalLight(const Math::Vec3& position, const Math::Vec3& direction, const Math::Vec3& ambient,
						 const Math::Vec3& diffuse, const Math::Vec3& specular)
			: Light(position, ambient, diffuse, specular)
		{
			m_data[3] = direction.x;
			m_data[4] = direction.y;
			m_data[5] = direction.z;
			m_type = LightType::Directional;
		#if ASTRA_DEBUG
			m_gizmo = new Gizmo("../Astra/res/textures/DirectionalLight.png", position, 3);
		#endif
		}
	#if ASTRA_DEBUG
		~DirectionalLight()
		{
			delete m_gizmo;
		}
		inline const Gizmo* const GetGizmo() const { return m_gizmo; }
	#endif
		
		inline bool IsDirectional() const override { return true; }
	};
}