#pragma once

#include "ImageEffect.h"
#include "Astra/graphics/shaders/HDRShader.h"

namespace Astra::Graphics
{
	class HDREffect : public ImageEffect
	{
	private:
		bool m_enabled;
	public:
		HDREffect(bool active, float exposure)
			: ImageEffect(new HDRShader())
		{
			m_type = EffectType::Hdr;
			m_enabled = active;

			m_shader->Start();
			m_shader->SetUniform1i(HDR_ACTIVE_TAG, (int)active);
			m_shader->SetUniform1f(EXPOSURE_TAG, exposure);
			m_shader->Stop();
		}

		void SetActive(bool enabled)
		{
			m_enabled = enabled;
			m_shader->Start();
			m_shader->SetUniform1i(HDR_ACTIVE_TAG, (int)enabled);
			m_shader->Stop();
		}

		inline bool GetActive() const { return m_enabled; }
	};
}