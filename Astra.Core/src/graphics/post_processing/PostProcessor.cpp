#include "PostProcessor.h"

#include "../loaders/Loader.h"
#include "../Window.h"
#include "../../logger/Logger.h"

namespace Astra::Graphics
{
	#define GAUSS_BLUR				1
	#define BASE_BLUR_DOWNSCALE		4
	#define BLUR_STEPS				2

	PostProcessor::PostProcessor()
	{
		m_defaultQuad = Loader::Load(GL_TRIANGLE_STRIP, { -1, 1, -1, -1, 1, 1, 1, -1 }, 2);
		m_screenBuffer = Loader::LoadFrameBuffer(Window::width, Window::height);

	#if GAUSS_BLUR
		for (int i = 1; i <= BLUR_STEPS; i++)
		{
			size_t blurWidth = Window::width / (BASE_BLUR_DOWNSCALE * i);
			size_t blurHeight = Window::height / (BASE_BLUR_DOWNSCALE * i);
			effects.push_back(new HorizontalBlurEffect(blurWidth, blurHeight));
			effects.push_back(new VerticalBlurEffect(blurWidth, blurHeight));
		}
	#endif
		effects.push_back(new ContrastEffect());
	}

	PostProcessor::~PostProcessor()
	{
		delete m_defaultQuad;
		delete m_screenBuffer;
	}

	void PostProcessor::Attach()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_screenBuffer->GetId());
		if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::LogError("Frame Buffer Did Not Attach Correctly.");
		}
		glViewport(0, 0, Window::width, Window::height);
	}

	void PostProcessor::Detach()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glViewport(0, 0, Window::width, Window::height);
	}

	void PostProcessor::Draw()
	{
		glBindVertexArray(m_defaultQuad->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glDisable(GL_DEPTH_TEST);

		unsigned int attachment = m_screenBuffer->GetColorAttachment();
		for (auto* effect : effects)
		{
			effect->Start(&attachment);

			glClear(GL_COLOR_BUFFER_BIT);
			glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);

			effect->Stop();
		}

		glEnable(GL_DEPTH_TEST);
		glDisableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glBindVertexArray(0);
	}
}