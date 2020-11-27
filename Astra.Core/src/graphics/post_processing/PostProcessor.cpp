#include "PostProcessor.h"

#include "../loaders/Loader.h"
#include "../Window.h"
#include "../../logger/Logger.h"
#include "../ResourceManager.h"

namespace Astra::Graphics
{
	#define BLOOM				1
	
	PostProcessor::PostProcessor()
	{
		m_defaultQuad = Loader::Load(GL_TRIANGLE_STRIP, { -1, 1, -1, -1, 1, 1, 1, -1 }, 2);
	#if MULTI_SAMPLE
		m_multisampledBuffer = Loader::LoadFrameBuffer(Window::GetWidth(), Window::GetHeight(), true, DepthBufferType::Render, HDR);
		m_screenBuffer = Loader::LoadFrameBuffer(Window::GetWidth(), Window::GetHeight(), false, DepthBufferType::Texture, HDR);
	#else
		m_multisampledBuffer = NULL;
		m_screenBuffer = Loader::LoadFrameBuffer(Window::width, Window::height, false, DepthBufferType::Render);
	#endif
	#if BLOOM
		effects.push_back(new BloomEffect(Window::GetWidth(), Window::GetHeight()));
	#endif
	#if HDR
		effects.push_back(new HDREffect(true, 1));
	#else	
		effects.push_back(new ContrastEffect());
	#endif
	}

	PostProcessor::~PostProcessor()
	{
		ResourceManager::Unload(m_defaultQuad);
		ResourceManager::Unload(m_screenBuffer);
	#if MULTI_SAMPLE
		ResourceManager::Unload(m_multisampledBuffer);
	#endif

		for (const auto* effect : effects)
		{
			delete effect;
		}
	}

	void PostProcessor::UpdateScreenRatio(unsigned int width, unsigned int height)
	{
	#if MULTI_SAMPLE
		Loader::UpdateFrameBuffer(m_multisampledBuffer, width, height, HDR, true);
	#endif
		Loader::UpdateFrameBuffer(m_screenBuffer, width, height, HDR, false);
		for (auto* effect : effects)
		{
			effect->UpdateAspectRatio(width, height);
		}
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
		glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
	}

	void PostProcessor::Detach()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
	}

	void PostProcessor::Draw()
	{
		glBindVertexArray(m_defaultQuad->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glDisable(GL_DEPTH_TEST);

	#if MULTI_SAMPLE
		ResolveFrameBuffer(m_multisampledBuffer->GetId(), m_screenBuffer->GetId(), Window::GetWidth(), Window::GetHeight(), Window::GetWidth(), Window::GetHeight());
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
			for (auto iter = effects.begin(); iter != effects.end();) 
			{
				(*iter)->Start(&attachment);
				glClear(GL_COLOR_BUFFER_BIT);
				glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);
				(*iter)->Stop();
				if ((*iter)->Finished())
				{
					(*iter)->Reset();
					iter++;
				}
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
		glBlitFramebuffer(0, 0, inputWidth, inputHeight, 0, 0, outputWidth, outputHeight, GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		Detach();
	}

	void PostProcessor::ResolveFrameBufferToScreen(GLuint bufferIn, unsigned int inputWidth, unsigned int inputHeight)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, bufferIn);
		glDrawBuffer(GL_BACK);
		glBlitFramebuffer(0, 0, inputWidth, inputHeight, 0, 0, Window::GetWidth(), Window::GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		Detach();
	}
}