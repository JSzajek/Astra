#pragma once

#include "Astra/graphics/entities/Spatial.h"
#include "Astra/graphics/materials/WaterMaterial.h"

namespace Astra::Graphics
{
	class WaterTile : public Spatial
	{
	private:
		float m_size;
	public:
		WaterMaterial material;
	public:
		WaterTile();
		WaterTile(float xCenter, float zCenter, float height, float size);
		WaterTile(const WaterMaterial& material, float xCenter, float zCenter, float height, float size);
		WaterTile(const WaterTile& other);
	};
}