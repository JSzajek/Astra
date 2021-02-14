#pragma once

#include <vector>

#include "../buffers/VertexArray.h"
#include "ImageEffect.h"

#include "ContrastEffect.h"
#include "HDREffect.h"
#include "HorizontalBlurEffect.h"
#include "VerticalBlurEffect.h"
#include "BloomEffect.h"

namespace Astra::Graphics
{
	class PostProcessor
	{
	private:
		const VertexArray* m_defaultQuad;
		FrameBuffer* m_screenBuffer;
		FrameBuffer* m_multisampledBuffer;

		std::vector<ImageEffect*> effects;
	public:
		PostProcessor();
		~PostProcessor();

		void Attach();
		void Detach();

		void Draw();

		void UpdateScreenRatio(unsigned int width, unsigned int height);
	private:
		void ResolveFrameBuffer(GLuint bufferIn, GLuint bufferOut, unsigned int inputWidth, unsigned int inputHeight,
			unsigned int outputWidth, unsigned int outputHeight);
		void ResolveFrameBufferToScreen(GLuint bufferIn, unsigned int inputWidth, unsigned int inputHeight);
	};
}