#pragma once

#include "FrameBuffer.h"

namespace Astra::Graphics
{
	class WaterFrameBuffer
	{
	private:
		static const constexpr float DefaultReflectionWidth = 320;
		static const constexpr float DefaultReflectionHeight = 180;

		static const constexpr float DefaultRefractionWidth = 1280;
		static const constexpr float DefaultRefractionHeight = 720;

	private:
		float m_reflectionWidth, m_reflectionHeight;
		float m_refractionWidth, m_refractionHeight;
	public:
		FrameBuffer m_reflectionBuffer;
		FrameBuffer m_refractionBuffer;
		unsigned int id;
	public:
		WaterFrameBuffer(const FrameBuffer& reflection, const FrameBuffer& refraction);
		WaterFrameBuffer(const WaterFrameBuffer& other);

		const FrameBuffer& GetReflectionBuffer() const { return m_reflectionBuffer; }
		const FrameBuffer& GetRefractionBuffer() const { return m_refractionBuffer; }

		bool operator==(const WaterFrameBuffer& other) const;
	};
}