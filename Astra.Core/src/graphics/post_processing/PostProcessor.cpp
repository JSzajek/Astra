#include "PostProcessor.h"

#include "../loaders/Loader.h"
#include "../Window.h"
#include "../../logger/Logger.h"

namespace Astra::Graphics
{
	#define GAUSS_BLUR				0
	#define BASE_BLUR_DOWNSCALE		4
	#define BLUR_STEPS				2

	PostProcessor::PostProcessor()
	{
		m_defaultQuad = Loader::Load(GL_TRIANGLE_STRIP, { -1, 1, -1, -1, 1, 1, 1, -1 }, 2);
	#if MULTI_SAMPLE
		m_multisampledBuffer = Loader::LoadFrameBuffer(Window::width, Window::height, true, DepthBufferType::Render, true);
		m_screenBuffer = Loader::LoadFrameBuffer(Window::width, Window::height, false, DepthBufferType::Texture, true);
	#else
		m_multisampledBuffer = NULL;
		m_screenBuffer = Loader::LoadFrameBuffer(Window::width, Window::height, false, DepthBufferType::Render);
	#endif

	#if GAUSS_BLUR
		for (int i = 1; i <= BLUR_STEPS; i++)
		{
			size_t blurWidth = Window::width / (BASE_BLUR_DOWNSCALE * i);
			size_t blurHeight = Window::height / (BASE_BLUR_DOWNSCALE * i);
			effects.push_back(new HorizontalBlurEffect(blurWidth, blurHeight));
			effects.push_back(new VerticalBlurEffect(blurWidth, blurHeight));
		}
	#endif
		effects.push_back(new HDREffect(true, 1));
		//effects.push_back(new ContrastEffect());
	}

	PostProcessor::~PostProcessor()
	{
		delete m_defaultQuad;
		delete m_screenBuffer;
	#if MULTI_SAMPLE
		delete m_multisampledBuffer;
	#endif
	}

	void PostProcessor::Attach()
	{
	#if MULTI_SAMPLE
		glBindFramebuffer(GL_FRAMEBUFFER, m_multisampledBuffer->GetId());
	#else
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_screenBuffer->GetId());
	#endif
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::LogError("Frame Buffer Did Not Attach Correctly.");
		}
		glViewport(0, 0, Window::width, Window::height);
	}

	void PostProcessor::Detach()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Window::width, Window::height);
	}

	void PostProcessor::Draw()
	{
		glBindVertexArray(m_defaultQuad->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glDisable(GL_DEPTH_TEST);

	#if MULTI_SAMPLE
		ResolveFrameBuffer(m_multisampledBuffer->GetId(), m_screenBuffer->GetId(), Window::width, Window::height, Window::width, Window::height);
		unsigned int attachment = m_screenBuffer->GetColorAttachment();
	#else
		unsigned int attachment = m_screenBuffer->GetColorAttachment();
	#endif
		if (effects.size() == 0)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);
		}
		else
		{
			for (auto* effect : effects)
			{
				effect->Start(&attachment);

				glClear(GL_COLOR_BUFFER_BIT);
				glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);

				effect->Stop();
			}
		}

		glEnable(GL_DEPTH_TEST);
		glDisableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glBindVertexArray(0);
	}

	void PostProcessor::ResolveFrameBuffer(GLuint bufferIn, GLuint bufferOut,
										   unsigned int inputWidth, unsigned int inputHeight,
										   unsigned int outputWidth, unsigned int outputHeight)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, bufferIn);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, bufferOut);
		glBlitFramebuffer(0, 0, inputWidth, inputHeight, 0, 0, outputWidth, outputHeight,
							GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT, GL_NEAREST);
		Detach();
	}

	void PostProcessor::ResolveFrameBufferToScreen(GLuint bufferIn, unsigned int inputWidth, unsigned int inputHeight)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, bufferIn);
		glDrawBuffer(GL_BACK);
		glBlitFramebuffer(0, 0, inputWidth, inputHeight, 0, 0, Window::width, Window::height,
			GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		Detach();
	}

}