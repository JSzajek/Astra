#pragma once

#include "Spatial.h"
#include <functional>

namespace Astra::Graphics
{
	class Light : public Spatial
	{
	private:
		Math::Vec3 m_color;
		Math::Vec3 m_attenuation;
		std::function<void()> m_lightChanged;
	public:
		Light(const Math::Vec3& translation, const Math::Vec3& color, const Math::Vec3& attenuation = Math::Vec3(1, 0, 0));
		Light(const Light& other);

		inline const Math::Vec3& GetColor() const { return m_color; }
		inline const Math::Vec3& GetAttenuation() const { return m_attenuation; }

		inline void SetCallback(std::function<void()> callback) { m_lightChanged = callback; }
		
		inline void SetColor(const Math::Vec3& color) 
		{ 
			m_color = color;
			if (m_lightChanged != NULL)
			{
				m_lightChanged();
			}
		}

		inline void SetAttenuation(const Math::Vec3& attenuation)
		{
			m_attenuation = attenuation;
			if (m_lightChanged != NULL)
			{
				m_lightChanged();
			}
		}

		inline void SetTranslation(const Math::Vec3& translation) override 
		{
			rows[0] = translation;
			if (m_lightChanged != NULL)
			{
				m_lightChanged();
			}
		}
	};
}