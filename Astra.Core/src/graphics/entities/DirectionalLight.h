#pragma once

#include "Light.h"
#include "../gizmos/Gizmo.h"

namespace Astra::Graphics
{
	class DirectionalLight : public Light
	{
	private:
	#if _DEBUG
		const Gizmo* m_gizmo;
	#endif
	public:
		DirectionalLight(const Math::Vec3& position, const Math::Vec3& direction, const Math::Vec3& ambient,
						 const Math::Vec3& diffuse, const Math::Vec3& specular)
			: Light(position, ambient, diffuse, specular)
		{
			data[3] = direction.x;
			data[4] = direction.y;
			data[5] = direction.z;
			m_type = LightType::Directional;
		#if _DEBUG
			m_gizmo = new Gizmo("../Astra.Core/src/resources/textures/DirectionalLight.png", position, 3);
		#endif
		}
		
		inline bool IsDirectional() const override { return true; }
	#if _DEBUG
		inline const Gizmo* const GetGizmo() const { return m_gizmo; }
	#endif
	};
}