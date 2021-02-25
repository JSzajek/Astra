#include "astra_pch.h"

#include <GL/glew.h>

#include "FrameBuffer.h"

namespace Astra::Graphics
{
	FrameBuffer::FrameBuffer(DepthBufferType type, bool multisampled, size_t numOfColorAttachments, size_t numOfDepthAttachments)
		: m_type(type), m_multisampled(multisampled)
	{
		m_colorAttachmentOffset = numOfColorAttachments;
		m_depthAttachements = numOfDepthAttachments;
		m_data.resize(1 + numOfColorAttachments + numOfDepthAttachments);
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffersEXT(1, &m_data[0]);
		for (size_t i = 0; i < m_colorAttachmentOffset; i++)
		{
			glDeleteTextures(m_colorAttachmentOffset, &GetColorAttachment(i));
		}

		for (size_t i = 0; i < m_depthAttachements; i++)
		{
			if (m_type == DepthBufferType::Texture)
			{
				glDeleteTextures(1, &GetDepthAttachment(i));
			}
			else
			{
				glDeleteRenderbuffersEXT(1, &GetDepthAttachment(i));
			}
		}
	}
}