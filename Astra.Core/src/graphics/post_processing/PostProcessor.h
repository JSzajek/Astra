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

		std::vector<ImageEffect*> effects;
	public:
		PostProcessor();
		~PostProcessor();

		void Attach();
		void Detach();

		void Draw();
	};
}