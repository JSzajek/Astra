#pragma once

#include "ImageEffect.h"
#include "../shaders/HDRShader.h"

namespace Astra::Graphics
{
	class HDREffect : public ImageEffect
	{
	public:
		HDREffect(bool active, float exposure)
			: ImageEffect(new HDRShader())
		{
			m_shader->Start();
			m_shader->SetUniform1i(HDR_ACTIVE_TAG, (int)active);
			m_shader->SetUniform1f(EXPOSURE_TAG, exposure);
			m_shader->Stop();
		}
	};
}