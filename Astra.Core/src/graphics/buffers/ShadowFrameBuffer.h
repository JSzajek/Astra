#pragma once

#include "FrameBuffer.h"

namespace Astra::Graphics
{
	class ShadowFrameBuffer
	{
	private:
		float m_width, m_height;
		unsigned int m_mapId;
	public:
		FrameBuffer m_buffer;
	public:
		ShadowFrameBuffer(const FrameBuffer& buffer, unsigned int mapId, unsigned int width, unsigned int height);
		ShadowFrameBuffer(const ShadowFrameBuffer& other);

		inline const float GetWidth() const { return m_width; }
		inline const float GetHeight() const { return m_height; }
		inline const FrameBuffer& GetBuffer() const { return m_buffer; }
		inline const unsigned int GetShadowMap() const { return m_mapId; }
	};
}