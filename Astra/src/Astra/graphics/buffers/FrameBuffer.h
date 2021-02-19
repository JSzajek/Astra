#pragma once

#include <vector>
#include <GL/glew.h>

namespace Astra::Graphics
{
	enum class DepthBufferType
	{
		Render,
		Texture,
		None
	};

	struct FrameBuffer
	{
	private:
		std::vector<unsigned int> m_data;
		size_t m_colorAttachmentOffset;
		size_t m_depthAttachements;
		DepthBufferType m_type;
		bool m_multisampled;
	public:
		FrameBuffer(DepthBufferType type, bool multisampled = false, size_t numOfColorAttachments = 1, size_t numOfDepthAttachments = 1)
			: m_type(type), m_multisampled(multisampled)
		{
			m_colorAttachmentOffset = numOfColorAttachments;
			m_depthAttachements = numOfDepthAttachments;
			m_data.resize(1 + numOfColorAttachments + numOfDepthAttachments);
		}

		~FrameBuffer()
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

		const unsigned int& GetId() const { return m_data[0]; }
		const unsigned int& GetColorAttachment(size_t index = 0) const { return m_data[1 + index]; }
		const unsigned int& GetDepthAttachment(size_t index = 0) const { return m_data[1 + m_colorAttachmentOffset + index]; }

		inline DepthBufferType GetType() const { return m_type; }
		inline bool GetMultisampled() const { return m_multisampled; }
		inline size_t GetColorAttachmentCount() const { return m_colorAttachmentOffset; }

		unsigned int& Id() { return m_data[0]; }
		unsigned int& ColorAttachment(size_t index = 0) { return m_data[1 + index]; }
		unsigned int& DepthAttachment(size_t index = 0) { return m_data[1 + m_colorAttachmentOffset + index]; }
	};
}