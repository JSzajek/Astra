#pragma once

#include "Spatial.h"
#include "Astra/graphics/gizmos/Gizmo.h"
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
		std::function<void(unsigned int, const Light*)> m_lightChanged;
	protected:
		Math::Vec3 m_attenuation;
		float m_cutOff;
		float m_outerCutOff;
	#if ASTRA_DEBUG
		Gizmo m_gizmo;
	#endif
	protected:
		Light(const Math::Vec3& translation, const Math::Vec3& color);
		Light(const Math::Vec3& translation, const Math::Vec3& ambient, 
			  const Math::Vec3& diffuse, const Math::Vec3& specular);
	public:
		Light(const Light& other);
	#if ASTRA_DEBUG
		~Light()
		{
			//delete m_gizmo;
		}
	#endif
		inline Math::Vec3 GetColor() const { return m_color; }
		inline Math::Vec3 GetAmbient() const { return m_ambient; }
		inline Math::Vec3 GetDiffuse() const { return m_diffuse; }
		inline Math::Vec3 GetSpecular () const { return m_specular; }
		inline Math::Vec3 GetAttenuation() const { return m_attenuation; }
		inline const float GetCutOff() const { return m_cutOff; }
		inline const float GetOuterCutOff() const { return m_outerCutOff; }
	#if ASTRA_DEBUG
		inline const Gizmo& GetGizmo() const { return m_gizmo; }
	#endif
		inline virtual LightType GetType() const = 0;
		inline virtual bool IsDirectional() const = 0;
		inline void SetCallback(std::function<void(unsigned int, const Light*)> callback) { m_lightChanged = callback; }
	public:
		virtual void Free() override;
		/*inline void SetColor(const Math::Vec3& color) 
		{ 
			m_color = color;
			if (m_lightChanged != NULL)
			{
				m_lightChanged(this);
			}
		}*/

		//inline void SetAttenuation(const Math::Vec3& attenuation)
		//{
		//	m_attenuation = attenuation;
		//	if (m_lightChanged != NULL)
		//	{
		//		m_lightChanged();
		//	}
		//}

		/*inline void SetTranslation(const Math::Vec3& translation) override 
		{
			Spatial::SetTranslation(translation);
			if (m_lightChanged != NULL)
			{
				m_lightChanged(this);
			}
		}*/
	};
}