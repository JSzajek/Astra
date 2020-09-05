#pragma once

#include "../buffers/Texture.h"

namespace Astra::Graphics
{
	class TerrainMaterial : public Texture
	{
	public:
		float shineDampener, reflectivity;
	public:
		TerrainMaterial(const char* const filepath);
	};
}