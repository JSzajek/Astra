#include "astra_pch.h"

#include "PostProcessor.h"

#include "Astra/Application.h"
#include "Astra/graphics/loaders/Loader.h"
#include "Astra/graphics/ResourceManager.h"

namespace Astra::Graphics
{
	PostProcessor::PostProcessor()
	{
		m_defaultQuad = Loader::Load(GL_TRIANGLE_STRIP, { -1, 1, -1, -1, 1, 1, 1, -1 }, 2);
		auto [width, height] = Application::Get().GetWindow().GetSize();
		auto hdr = Application::Get().GetWindow().IsHDR();

		SetMultisampling(Application::Get().GetWindow().IsMultisampling());
		
		if (Application::Get().GetWindow().IsBloom())
		{
			effects.push_front(new BloomEffect(width, height));
		}

		effects.push_back(new HDREffect(hdr, 0.8f));
		ResourceManager::ToggleHDRTextures(hdr);
	}

	PostProcessor::~PostProcessor()
	{
		RESOURCE_UNLOAD(m_defaultQuad);
		RESOURCE_UNLOAD(m_screenBuffer);
		RESOURCE_UNLOAD(m_multisampledBuffer);

		for (const auto* effect : effects)
		{
			delete effect;
		}
	}

	void PostProcessor::UpdateScreenRatio(unsigned int width, unsigned int height)
	{
		bool hdr = Application::Get().GetWindow().IsHDR();

		if (m_multisampledBuffer)
		{
			Loader::UpdateFrameBuffer(m_multisampledBuffer, width, height, hdr, Application::Get().GetWindow().IsMultisampling());
		}
		Loader::UpdateFrameBuffer(m_screenBuffer, width, height, hdr, false);
		for (auto* effect : effects)
		{
			effect->UpdateAspectRatio(width, height);
		}
	}

	void PostProcessor::SetMultisampling(unsigned int sampleSize)
	{
		auto [width, height] = Application::Get().GetWindow().GetSize();
		bool hdr = Application::Get().GetWindow().IsHDR();

		RESOURCE_UNLOAD(m_screenBuffer);
		RESOURCE_UNLOAD(m_multisampledBuffer);

		if (sampleSize == 0)
		{
			// Turn off multisampling
			m_screenBuffer = Loader::LoadFrameBuffer(width, height, false, DepthBufferType::Render, hdr);
		}
		else 
		{
			if (!m_multisampledBuffer)
			{
				// Turn on multisample framebuffer
				m_multisampledBuffer = Loader::LoadFrameBuffer(width, height, sampleSize, DepthBufferType::Render, hdr);
				m_screenBuffer = Loader::LoadFrameBuffer(width, height, false, DepthBufferType::Texture, hdr);
			}
			else
			{
				// Resize multisample framebuffer
				Loader::UpdateFrameBuffer(m_multisampledBuffer, width, height, hdr, sampleSize);
			}
		}
	}

	void PostProcessor::SetBloomEffect(bool enabled)
	{
		if (!enabled)
		{
			for (auto iter = effects.begin(); iter != effects.end(); iter++)
			{
				if ((*iter)->GetType() == EffectType::Bloom)
				{
					effects.erase(iter);
					break;
				}
			}
		}
		else
		{
			auto [width, height] = Application::Get().GetWindow().GetSize();
			effects.push_front(new BloomEffect(width, height));
		}
	}

	void PostProcessor::SetHDR(bool enabled)
	{
		((HDREffect*)*(--effects.end()))->SetActive(enabled);

		// Update loaded diffuse textures
		ResourceManager::ToggleHDRTextures(enabled);

		// Update framebuffer for hdr - Including ones inside effects
		auto [width, height] = Application::Get().GetWindow().GetSize();
		UpdateScreenRatio(width, height);
	}

	bool PostProcessor::IsEmpty()
	{
		if (m_multisampledBuffer)
		{
			return false;
		}
		if (effects.size() == 1)
		{
			return !((HDREffect*)*(--effects.end()))->GetActive();
		}
		return false;
	}

	void PostProcessor::Attach()
	{
		if (m_multisampledBuffer)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_multisampledBuffer->GetId());
		}
		else
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_screenBuffer->GetId());
		}
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			ASTRA_CORE_ERROR("Post Processing Frame Buffer Did Not Attach Correctly.");
		}
		auto [width, height] = Application::Get().GetWindow().GetSize();
		glViewport(0, 0, width, height);
	}

	void PostProcessor::Detach()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		auto [width, height] = Application::Get().GetWindow().GetSize();
		glViewport(0, 0, width, height);
	}

	void PostProcessor::Draw()
	{
		glBindVertexArray(m_defaultQuad->vaoId);
		glDisable(GL_DEPTH_TEST);

		unsigned int attachment = 0;
		if (m_multisampledBuffer)
		{
			auto [width, height] = Application::Get().GetWindow().GetSize();
			ResolveFrameBuffer(m_multisampledBuffer->GetId(), m_screenBuffer->GetId(), width, height, width, height);
			attachment = m_screenBuffer->GetColorAttachment();
		}
		else
		{
			attachment = m_screenBuffer->GetColorAttachment();
		}

		for (auto iter = effects.begin(); iter != effects.end();) 
		{
			(*iter)->Start(&attachment);
			glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);
			(*iter)->Stop();
			if ((*iter)->Finished())
			{
				(*iter)->Reset();
				iter++;
			}
		}
		glEnable(GL_DEPTH_TEST);
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
		auto [width, height] = Application::Get().GetWindow().GetSize();
		glBlitFramebuffer(0, 0, inputWidth, inputHeight, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		Detach();
	}
}