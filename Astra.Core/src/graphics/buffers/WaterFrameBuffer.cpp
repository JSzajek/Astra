#include "WaterFrameBuffer.h"

namespace Astra::Graphics
{
	WaterFrameBuffer::WaterFrameBuffer(const FrameBuffer& reflection, const FrameBuffer& refraction)
		: m_reflectionBuffer(reflection), m_refractionBuffer(refraction), id (0),
			m_reflectionHeight(DefaultReflectionHeight), m_reflectionWidth(DefaultReflectionWidth),
			m_refractionHeight(DefaultRefractionHeight), m_refractionWidth(DefaultRefractionWidth)
	{
	}

	WaterFrameBuffer::WaterFrameBuffer(const WaterFrameBuffer& other)
		: m_reflectionBuffer(other.m_reflectionBuffer), m_refractionBuffer(other.m_refractionBuffer),
			m_reflectionHeight(other.m_reflectionHeight), m_reflectionWidth(other.m_reflectionWidth),
			m_refractionHeight(other.m_refractionHeight), m_refractionWidth(other.m_refractionWidth),
			id(other.id)
	{
	}

	bool WaterFrameBuffer::operator==(const WaterFrameBuffer& other) const
	{
		return id == other.id;
	}
}