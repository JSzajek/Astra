#pragma once

#include "ImageEffect.h"
#include "../shaders/HorizontalBlurShader.h"

namespace Astra::Graphics
{
	class HorizontalBlurEffect : public ImageEffect
	{
	public:
		HorizontalBlurEffect(int width, int height)
			: ImageEffect(new HorizontalBlurShader(), width, height, true, GL_CLAMP_TO_EDGE)
		{
			m_shader->Start();
			m_shader->SetUniform1f(TARGET_WIDTH_TAG, width);
			m_shader->Stop();
		}
	};
}