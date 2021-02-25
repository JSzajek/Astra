#pragma once

#include "FrameBuffer.h"

namespace Astra::Graphics
{
	class ShadowFrameBuffer
	{
	private:
		unsigned int m_width, m_height;
		unsigned int m_mapId;
		FrameBuffer* m_buffer;
	public:
		ShadowFrameBuffer(FrameBuffer* buffer, unsigned int width, unsigned int height);
		ShadowFrameBuffer(const ShadowFrameBuffer& other);
		~ShadowFrameBuffer();

		inline unsigned int GetWidth() const { return m_width; }
		inline unsigned int GetHeight() const { return m_height; }
		inline const FrameBuffer* GetBuffer() const { return m_buffer; }
		inline const unsigned int GetShadowMap() const { return m_mapId; }
	};
}