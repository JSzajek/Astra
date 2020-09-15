#pragma once

#include "FrameBuffer.h"

namespace Astra::Graphics
{
	class WaterFrameBuffer
	{
	private:
		float m_reflectionWidth, m_reflectionHeight;
		float m_refractionWidth, m_refractionHeight;
	public:
		FrameBuffer m_reflectionBuffer;
		FrameBuffer m_refractionBuffer;
	public:
		WaterFrameBuffer(const FrameBuffer& reflection, const FrameBuffer& refraction,
						 unsigned int reflectionWidth, unsigned int reflectionHeight,
					     unsigned int refractionWidth, unsigned int refractionHeight);
		WaterFrameBuffer(const WaterFrameBuffer& other);

		const FrameBuffer& GetReflectionBuffer() const { return m_reflectionBuffer; }
		const FrameBuffer& GetRefractionBuffer() const { return m_refractionBuffer; }
	};
}