#pragma once

#include <vector>
#include <cstring>

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
		//unsigned int m_data[3];
	public:
		FrameBuffer(size_t numOfColorAttachments = 1, size_t numOfDepthAttachments = 1)
		{
			m_colorAttachmentOffset = numOfColorAttachments;
			m_data.resize(1 + numOfColorAttachments + numOfDepthAttachments);
		}

		const unsigned int& GetId() const { return m_data[0]; }
		const unsigned int& GetColorAttachment(size_t index = 0) const { return m_data[1 + index]; }
		const unsigned int& GetDepthAttachment(size_t index = 0) const { return m_data[1 + m_colorAttachmentOffset + index]; }

		unsigned int& Id() { return m_data[0]; }
		unsigned int& ColorAttachment(size_t index = 0) { return m_data[1 + index]; }
		unsigned int& DepthAttachment(size_t index = 0) { return m_data[1 + m_colorAttachmentOffset + index]; }
	};
}