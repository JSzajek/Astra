#pragma once

#include "FrameBuffer.h"

namespace Astra::Graphics
{
	class WaterFrameBuffer
	{
	private:
		unsigned int m_reflectionWidth, m_reflectionHeight;
		unsigned int m_refractionWidth, m_refractionHeight;
	public:
		FrameBuffer* m_reflectionBuffer;
		FrameBuffer* m_refractionBuffer;
	public:
		WaterFrameBuffer(FrameBuffer* reflection, FrameBuffer* refraction,
						 unsigned int reflectionWidth, unsigned int reflectionHeight,
					     unsigned int refractionWidth, unsigned int refractionHeight);
		WaterFrameBuffer(const WaterFrameBuffer& other);
		~WaterFrameBuffer();

		const FrameBuffer* GetReflectionBuffer() const { return m_reflectionBuffer; }
		const FrameBuffer* GetRefractionBuffer() const { return m_refractionBuffer; }
	};
}