#pragma once

#include <vector>

#include "../buffers/VertexArray.h"
#include "ImageEffect.h"

#include "ContrastEffect.h"
#include "HorizontalBlurEffect.h"
#include "VerticalBlurEffect.h"

namespace Astra::Graphics
{
	class PostProcessor
	{
	private:
		const VertexArray* m_defaultQuad;
		const FrameBuffer* m_screenBuffer;
		const FrameBuffer* m_multisampledBuffer;

		std::vector<ImageEffect*> effects;
	public:
		PostProcessor();
		~PostProcessor();

		void Attach();
		void Detach();

		void Draw();
	private:
		void ResolveFrameBuffer(GLuint bufferIn, GLuint bufferOut, unsigned int inputWidth, unsigned int inputHeight,
			unsigned int outputWidth, unsigned int outputHeight);
		void ResolveFrameBufferToScreen(GLuint bufferIn, unsigned int inputWidth, unsigned int inputHeight);
	};
}