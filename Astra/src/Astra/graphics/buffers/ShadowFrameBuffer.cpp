#include "astra_pch.h"

#include "ShadowFrameBuffer.h"

namespace Astra::Graphics
{
	ShadowFrameBuffer::ShadowFrameBuffer(FrameBuffer* buffer, unsigned int width, unsigned int height)
		: m_buffer(buffer), m_width(width), m_height(height)
	{
		m_mapId = m_buffer->GetDepthAttachment();
	}

	ShadowFrameBuffer::ShadowFrameBuffer(const ShadowFrameBuffer& other)
		: m_buffer(other.m_buffer), m_mapId(other.m_mapId), m_width(other.m_width), m_height(other.m_height)
	{
	}

	ShadowFrameBuffer::~ShadowFrameBuffer()
	{
		delete m_buffer;
	}
}