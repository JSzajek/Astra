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
		FrameBuffer m_refractionBuffer;
		FrameBuffer m_reflectionBuffer;
	public:
		WaterFrameBuffer(const FrameBuffer& reflection, const FrameBuffer& refraction);
		WaterFrameBuffer(const WaterFrameBuffer& other);
	};
}