#include "ShadowFrameBuffer.h"

#include "../ResourceManager.h"

namespace Astra::Graphics
{
	ShadowFrameBuffer::ShadowFrameBuffer(FrameBuffer* buffer, unsigned int mapId, unsigned int width, unsigned int height)
		: m_buffer(buffer), m_mapId(mapId), m_width(width), m_height(height)
	{
	}

	ShadowFrameBuffer::ShadowFrameBuffer(const ShadowFrameBuffer& other)
		: m_buffer(other.m_buffer), m_mapId(other.m_mapId), m_width(other.m_width), m_height(other.m_height)
	{
	}

	ShadowFrameBuffer::~ShadowFrameBuffer()
	{
		ResourceManager::Unload(m_buffer);
	}
}