#pragma once

#include "Spatial.h"
#include <functional>

namespace Astra::Graphics
{
	enum class LightType : unsigned short
	{
		Directional,
		Point,
		Spotlight
	};

	class Light : public Spatial
	{
	private:
		Math::Vec3 m_color;
		Math::Vec3 m_ambient;
		Math::Vec3 m_diffuse;
		Math::Vec3 m_specular;
		std::function<void(const Light*)> m_lightChanged;
	protected:
		LightType m_type;
	protected:
		Light(const Math::Vec3& translation, const Math::Vec3& color);
		Light(const Math::Vec3& translation, const Math::Vec3& ambient, 
			  const Math::Vec3& diffuse, const Math::Vec3& specular);
	public:
		Light(const Light& other);

		inline const Math::Vec3& GetColor() const { return m_color; }
		inline const Math::Vec3& GetAmbient() const { return m_ambient; }
		inline const Math::Vec3& GetDiffuse() const { return m_diffuse; }
		inline const Math::Vec3& GetSpecular () const { return m_specular; }

		inline LightType GetType() const { return m_type; }

		inline virtual bool IsDirectional() const { return false; }

		inline void SetCallback(std::function<void(const Light*)> callback) { m_lightChanged = callback; }
		
		inline void SetColor(const Math::Vec3& color) 
		{ 
			m_color = color;
			if (m_lightChanged != NULL)
			{
				m_lightChanged(this);
			}
		}

		//inline void SetAttenuation(const Math::Vec3& attenuation)
		//{
		//	m_attenuation = attenuation;
		//	if (m_lightChanged != NULL)
		//	{
		//		m_lightChanged();
		//	}
		//}

		inline void SetTranslation(const Math::Vec3& translation) override 
		{
			Spatial::SetTranslation(translation);
			if (m_lightChanged != NULL)
			{
				m_lightChanged(this);
			}
		}
	};
}