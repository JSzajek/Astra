#include "astra_pch.h"

#include "BloomEffect.h"
#include "Astra/Application.h"
#include "../loaders/Loader.h"
#include "../ResourceManager.h"

namespace Astra::Graphics
{
	BloomEffect::BloomEffect(int width, int height)
		: ImageEffect(new SplitBloomShader(), 2 + (BLUR_STEPS * 2)), m_brightTexture(0)
	{
		// Multiple render target buffer
		m_buffer = Loader::LoadMultiTargetFrameBuffer(width, height, 2, 0, true); 
		m_width = width;
		m_height = height;

		auto [winWidth, winHeight] = Application::Get().GetWindow().GetSize();
		for (int i = 1; i <= BLUR_STEPS; i++)
		{
			unsigned int squeezRatio = (BASE_BLUR_DOWNSCALE * i);
			size_t blurWidth = winWidth / squeezRatio;
			size_t blurHeight = winHeight / squeezRatio;
			m_blurs.push_back(new HorizontalBlurEffect(blurWidth, blurHeight, squeezRatio));
			m_blurs.push_back(new VerticalBlurEffect(blurWidth, blurHeight, squeezRatio));
		}

		m_secondBuffer = Loader::LoadFrameBuffer(width, height, false, DepthBufferType::None, true, GL_CLAMP_TO_EDGE);
		m_secondShader = new CombineBloomShader();
		m_secondShader->Start();
		m_secondShader->SetUniform1i(REGULAR_COLOR_MAP, 0);
		m_secondShader->SetUniform1i(BRIGHT_BLUR_MAP, 1);
		m_secondShader->Stop();
	}

	BloomEffect::~BloomEffect()
	{
		ResourceManager::Unload(m_secondBuffer);
		delete m_secondShader;
		for (const auto* effect : m_blurs)
		{
			delete effect;
		}
	}

	void BloomEffect::Start(unsigned int* attachment)
	{
		if (m_step == 0)
		{
			// Splitting Step
			m_shader->Start();
			BindBuffer(m_buffer->GetId());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *attachment);
		}
		if (m_step > 0 && m_step < 5)
		{
			// Blurring Steps
			if (m_step == 1)
			{
				m_brightTexture = m_buffer->GetColorAttachment(1);
			}
			m_blurs[m_step - 1]->Start(&m_brightTexture);
			return;
		}
		else if (m_step == 5) 
		{ 
			// Combining Step
			m_secondShader->Start();
			BindBuffer(m_secondBuffer->GetId());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_buffer->GetColorAttachment(0));
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_brightTexture);
			
			*attachment = m_secondBuffer->GetColorAttachment();
		}
	}

	void BloomEffect::Stop()
	{
		if (m_step > 0 && m_step < 5)
		{
			m_blurs[m_step - 1]->Stop();
		}
		else if (m_step == 5)
		{
			UnbindBuffer();
			m_secondShader->Stop();
		}
		else
		{
			UnbindBuffer();
			m_shader->Stop();
		}
		m_step++;
	}

	void BloomEffect::UpdateAspectRatio(unsigned int width, unsigned int height)
	{
		ImageEffect::UpdateAspectRatio(width, height);
		Loader::UpdateFrameBuffer(m_secondBuffer, width, height, HDR, false);

		for (auto* blur : m_blurs)
		{
			blur->UpdateAspectRatio(width, height);
		}
	}
}