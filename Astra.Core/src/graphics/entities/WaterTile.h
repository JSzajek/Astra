#pragma once

#include "Spatial.h"

namespace Astra::Graphics
{
	class WaterTile : public Spatial
	{
	private:
		static const constexpr float DefaultSize = 60;
	private:
		float m_size;
	public:
		WaterTile(float xCenter, float zCenter, float height);
		WaterTile(float xCenter, float zCenter, float height, float size);
		WaterTile(const WaterTile& other);
		inline Math::Vec3& Rotation() override;
	};
}