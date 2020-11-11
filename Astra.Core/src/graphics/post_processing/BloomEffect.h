#pragma once

#include "ImageEffect.h"
#include "HorizontalBlurEffect.h"
#include "VerticalBlurEffect.h"
#include "../shaders/BloomShader.h"


namespace Astra::Graphics
{
	#define BASE_BLUR_DOWNSCALE		4
	#define BLUR_STEPS				2

	class BloomEffect : public ImageEffect
	{
	private:
		const FrameBuffer* m_secondBuffer;
		Shader* m_secondShader;
		std::vector<ImageEffect*> m_blurs;
		unsigned int m_brightTexture;
	public:
		BloomEffect(int width, int height);
		void Start(unsigned int* attachment) override;
		void Stop() override;
	};
}