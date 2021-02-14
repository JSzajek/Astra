#include "astra_pch.h"

#include "TerrainMaterial.h"
#include "../loaders/Loader.h"
#include "../ResourceManager.h"

namespace Astra::Graphics
{
	TerrainMaterial::TerrainMaterial(const char* const filepath)
		: m_texture(Loader::LoadTexture(filepath)), reflectivity(0), shineDampener(1)
	{
	}

	TerrainMaterialPack::TerrainMaterialPack(const TerrainMaterial* background, const TerrainMaterial* red,
											 const TerrainMaterial* green, const TerrainMaterial* blue)
		: backgroundTexture(ResourceManager::Track(background)),
		  rTexture(ResourceManager::Track(red)), 
		  gTexture(ResourceManager::Track(green)),
		  bTexture(ResourceManager::Track(blue)),
			specularTexture(Loader::LoadTexture(Texture::DefaultSpecular, false))
	{
	}

	TerrainMaterial::~TerrainMaterial()
	{
		ResourceManager::Unload(m_texture);
	}

	TerrainMaterialPack::~TerrainMaterialPack()
	{
		ResourceManager::Unload(backgroundTexture);
		ResourceManager::Unload(rTexture);
		ResourceManager::Unload(gTexture);
		ResourceManager::Unload(bTexture);
		ResourceManager::Unload(specularTexture);
	}
}
