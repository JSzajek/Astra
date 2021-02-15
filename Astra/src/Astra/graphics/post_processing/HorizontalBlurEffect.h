#pragma once

#include "ImageEffect.h"
#include "../shaders/HorizontalBlurShader.h"

namespace Astra::Graphics
{
	class HorizontalBlurEffect : public ImageEffect
	{
	private:
		unsigned int m_squeezeRatio;
	public:
		HorizontalBlurEffect(int width, int height, unsigned int squeezeRatio)
			: ImageEffect(new HorizontalBlurShader(), width, height, 1, true, GL_CLAMP_TO_EDGE), m_squeezeRatio(squeezeRatio)
		{
			m_shader->Start();
			m_shader->SetUniform1f(TARGET_WIDTH_TAG, static_cast<float>(width));
			m_shader->Stop();
		}
		
		void UpdateAspectRatio(unsigned int width, unsigned int height) override
		{
			ImageEffect::UpdateAspectRatio(width / m_squeezeRatio, height / m_squeezeRatio);
		}
	};
}