#pragma once

#include <vector>
#include <deque>

#include "ImageEffect.h"
#include <Astra/graphics/entities/utility/Mesh.h>

#include "HDREffect.h"
#include "HorizontalBlurEffect.h"
#include "VerticalBlurEffect.h"
#include "BloomEffect.h"

namespace Astra::Graphics
{
	class PostProcessor
	{
	private:
		Asset<Mesh> m_defaultQuad;
		FrameBuffer* m_screenBuffer;
		FrameBuffer* m_multisampledBuffer;

		std::deque<ImageEffect*> effects;
	public:
		PostProcessor();
		~PostProcessor();

		void Attach();
		void Detach();

		void Draw();

		void UpdateScreenRatio(unsigned int width, unsigned int height);
		void SetMultisampling(unsigned int sampleSize);
		void SetBloomEffect(bool enabled);
		void SetHDR(bool enabled);

		bool IsEmpty();
	private:
		void ResolveFrameBuffer(GLuint bufferIn, GLuint bufferOut, unsigned int inputWidth, unsigned int inputHeight,
			unsigned int outputWidth, unsigned int outputHeight);
		void ResolveFrameBufferToScreen(GLuint bufferIn, unsigned int inputWidth, unsigned int inputHeight);
	};
}