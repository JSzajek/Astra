#include "TerrainMaterial.h"

#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	TerrainMaterial::TerrainMaterial(const char* const filepath)
		: Texture(Loader::LoadTexture(filepath)), reflectivity(0), shineDampener(1)
	{
	}

	TerrainMaterialPack::TerrainMaterialPack(const TerrainMaterial* background, const TerrainMaterial* red,
											 const TerrainMaterial* green, const TerrainMaterial* blue)
		: backgroundTexture(background), rTexture(red), gTexture(green), bTexture(blue),
			specularTexture(Loader::LoadTexture(Texture::DefaultSpecular))
	{
	}
}
