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
		~TerrainMaterial();
	};
	
	struct TerrainMaterialPack
	{
		const TerrainMaterial* backgroundTexture;
		const TerrainMaterial* rTexture;
		const TerrainMaterial* gTexture;
		const TerrainMaterial* bTexture;
	
		TerrainMaterialPack(const TerrainMaterial* background, const TerrainMaterial* red,
							const TerrainMaterial* green, const TerrainMaterial* blue);
	};
}