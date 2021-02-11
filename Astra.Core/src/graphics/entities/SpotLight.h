#pragma once

#include "Light.h"
#include "../gizmos/Gizmo.h"

namespace Astra::Graphics
{
	class SpotLight : public Light
	{
	private:
	#if _DEBUG
		const Gizmo* m_gizmo;
	#endif
		float m_cutOff;
		float m_outerCutOff;
	public:
		SpotLight(const Math::Vec3& translation, const Math::Vec3& direction, const Math::Vec3& ambient,
				  const Math::Vec3& diffuse, const Math::Vec3& specular, float cutOff, float outerCutOff)
			: Light(translation, ambient, diffuse, specular), m_cutOff(cutOff), m_outerCutOff(outerCutOff)
		{
			m_data[3] = direction.x;
			m_data[4] = direction.y;
			m_data[5] = direction.z;
			m_type = LightType::Spotlight;
		#if _DEBUG
			m_gizmo = new Gizmo("../Astra.Core/src/resources/textures/SpotLight.png", translation, 3);
		#endif
		}
	#if _DEBUG
		~SpotLight()
		{
			delete m_gizmo;
		}
		inline const Gizmo* const GetGizmo() const { return m_gizmo; }
	#endif

		inline float GetCutOff() const { return m_cutOff; }
		inline float GetOuterCutOff() const { return m_outerCutOff; }
	};
}