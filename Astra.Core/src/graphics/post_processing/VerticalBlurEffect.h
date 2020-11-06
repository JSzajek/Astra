#pragma once

#include "ImageEffect.h"
#include "../shaders/VerticalBlurShader.h"

namespace Astra::Graphics
{
	class VerticalBlurEffect : public ImageEffect
	{
	public:
		VerticalBlurEffect(int width, int height)
			: ImageEffect(new VerticalBlurShader(), width, height)
		{
			m_shader->Start();
			m_shader->SetUniform1f(TARGET_HEIGHT_TAG, height);
			m_shader->Stop();
		}
	};
}