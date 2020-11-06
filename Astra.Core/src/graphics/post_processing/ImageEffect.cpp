#include "ImageEffect.h"
#include "../loaders/Loader.h"

#include "../Window.h"
#include "../../logger/Logger.h"

namespace Astra::Graphics
{
	ImageEffect::ImageEffect(Shader* shader)
		: m_shader(shader), m_buffer(NULL), m_width(0), m_height(0)
	{
	}

	ImageEffect::ImageEffect(Shader* shader, int width, int height)
		: m_shader(shader), m_buffer(Loader::LoadFrameBuffer(width, height)), m_width(width), m_height(height)
	{
	}
	
	ImageEffect::~ImageEffect()
	{
		delete m_buffer;
	}

	void ImageEffect::Start(unsigned int* attachment) const
	{
		m_shader->Start();
		if (m_buffer)
		{
			BindBuffer();
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *attachment);

		if (m_buffer)
		{
			*attachment = m_buffer->GetColorAttachment();
		}
	}
	
	void ImageEffect::Stop() const
	{
		if (m_buffer)
		{
			UnbindBuffer();
		}
		m_shader->Stop();
	}

	void ImageEffect::BindBuffer() const
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_buffer->GetId());
		if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::LogError("Frame Buffer Did Not Attach Correctly.");
		}
		glViewport(0, 0, m_width, m_height);
	}

	void ImageEffect::UnbindBuffer() const
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glViewport(0, 0, Window::width, Window::height);
	}
}