#pragma once

#include "ImageEffect.h"
#include "../shaders/VerticalBlurShader.h"

namespace Astra::Graphics
{
	class VerticalBlurEffect : public ImageEffect
	{
	private:
		unsigned int m_squeezeRatio;
	public:
		VerticalBlurEffect(int width, int height, unsigned int squeezeRatio)
			: ImageEffect(new VerticalBlurShader(), width, height, 1, true, GL_CLAMP_TO_EDGE), m_squeezeRatio(squeezeRatio)
		{
			m_shader->Start();
			m_shader->SetUniform1f(TARGET_HEIGHT_TAG, height);
			m_shader->Stop();
		}
		
		void UpdateAspectRatio(unsigned int width, unsigned int height) override
		{
			ImageEffect::UpdateAspectRatio(width / m_squeezeRatio, height / m_squeezeRatio);
		}
	};
}