#pragma once

#include "../Spatial.h"
#include "../../materials/WaterMaterial.h"

namespace Astra::Graphics
{
	class WaterTile : public Spatial
	{
	private:
		static const constexpr float DefaultSize = 60;
	private:
		float m_size;
	public:
		WaterMaterial* const material;
	public:
		WaterTile(float xCenter, float zCenter, float height, WaterMaterial* const material = new WaterMaterial());
		WaterTile(float xCenter, float zCenter, float height, float size, WaterMaterial* const material = new WaterMaterial());
		WaterTile(const WaterTile& other);
		inline Math::Vec3* Rotation() override;
	};
}