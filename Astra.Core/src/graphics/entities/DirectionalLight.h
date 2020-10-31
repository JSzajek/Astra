#pragma once

#include "Light.h"

namespace Astra::Graphics
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(const Math::Vec3& position, const Math::Vec3& direction, const Math::Vec3& ambient,
						 const Math::Vec3& diffuse, const Math::Vec3& specular)
			: Light(position, ambient, diffuse, specular)
		{
			data[3] = direction.x;
			data[4] = direction.y;
			data[5] = direction.z;
			m_type = LightType::Directional;
		}
		
		inline bool IsDirectional() const override { return true; }
	};
}