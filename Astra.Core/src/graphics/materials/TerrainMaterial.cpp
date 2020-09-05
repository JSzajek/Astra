#include "TerrainMaterial.h"

#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	TerrainMaterial::TerrainMaterial(const char* const filepath)
		: Texture(filepath), reflectivity(0), shineDampener(1)
	{
		Loader::BufferTexture(this);
	}


	TerrainMaterialPack::TerrainMaterialPack(const TerrainMaterial* background, const TerrainMaterial* red,
												const TerrainMaterial* green, const TerrainMaterial* blue)
		: backgroundTexture(background), rTexture(red), gTexture(green), bTexture(blue)
	{
	}
}
