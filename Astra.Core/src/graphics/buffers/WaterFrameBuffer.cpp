#include "WaterFrameBuffer.h"

namespace Astra::Graphics
{
	WaterFrameBuffer::WaterFrameBuffer(const FrameBuffer& reflection, const FrameBuffer& refraction,
										unsigned int reflectionWidth, unsigned int reflectionHeight,
										unsigned int refractionWidth, unsigned int refractionHeight)
		: m_reflectionBuffer(reflection), m_refractionBuffer(refraction),
			m_reflectionHeight(reflectionHeight), m_reflectionWidth(reflectionWidth),
			m_refractionHeight(refractionHeight), m_refractionWidth(refractionWidth)
	{
	}

	WaterFrameBuffer::WaterFrameBuffer(const WaterFrameBuffer& other)
		: m_reflectionBuffer(other.m_reflectionBuffer), m_refractionBuffer(other.m_refractionBuffer),
			m_reflectionHeight(other.m_reflectionHeight), m_reflectionWidth(other.m_reflectionWidth),
			m_refractionHeight(other.m_refractionHeight), m_refractionWidth(other.m_refractionWidth)
	{
	}
}