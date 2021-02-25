#include "astra_pch.h"

#include "Astra/Application.h"
#include "ImageEffect.h"
#include "Astra/graphics/loaders/Loader.h"

namespace Astra::Graphics
{
	ImageEffect::ImageEffect(Shader* shader, size_t totalSteps)
		: m_shader(shader), m_buffer(NULL), m_width(0), m_height(0), m_totalSteps(totalSteps), m_step(0), m_floating(false), m_type(EffectType::DefaultEffect)
	{
	}

	ImageEffect::ImageEffect(Shader* shader, int width, int height, size_t totalSteps, bool floating, unsigned int component)
		: m_shader(shader), m_buffer(Loader::LoadFrameBuffer(FrameBufferCreationSpec(width, height, false, DepthBufferType::None, floating, component))), 
			m_width(width), m_height(height), m_totalSteps(totalSteps), m_step(0), m_floating(floating), m_type(EffectType::DefaultEffect)
	{
	}
	
	ImageEffect::~ImageEffect()
	{
		delete m_buffer;
		delete m_shader;
	}

	void ImageEffect::Start(unsigned int* attachment)
	{
		m_shader->Start();
		if (m_buffer)
		{
			BindBuffer(m_buffer->GetId());
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *attachment);

		if (m_buffer)
		{
			*attachment = m_buffer->GetColorAttachment();
		}
	}
	
	void ImageEffect::Stop()
	{
		if (m_buffer)
		{
			UnbindBuffer();
		}
		m_shader->Stop();
		m_step++;
	}

	void ImageEffect::UpdateAspectRatio(unsigned int width, unsigned int height)
	{
		if (m_buffer)
		{
			m_width = width;
			m_height = height;
			Loader::UpdateFrameBuffer(m_buffer, width, height, Application::Get().GetWindow().IsHDR(), false);
		}
	}

	void ImageEffect::BindBuffer(unsigned int id) const
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
		if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			ASTRA_CORE_ERROR("Image Effect Frame Buffer Did Not Attach Correctly.");
		}
		glViewport(0, 0, m_width, m_height);
	}

	void ImageEffect::UnbindBuffer() const
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		auto [width, height] = Application::Get().GetWindow().GetSize();
		glViewport(0, 0, width, height);
	}
}