#pragma once

#include "Spatial.h"

namespace Astra::Graphics
{
	class Light : public Spatial
	{
	private:
		Math::Vec3 m_color;
	public:
		Light(const Math::Vec3& translation, const Math::Vec3 color)
			: Spatial(translation), m_color(color)
		{
		}

		Light(const Light& other)
			: Spatial(other.GetTranslation(), other.GetRotation(), other.GetScale()), m_color(other.GetColor())
		{
		}

		inline const Math::Vec3& GetColor() const { return m_color; }
	};
}