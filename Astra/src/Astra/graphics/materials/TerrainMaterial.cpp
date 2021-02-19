#include "astra_pch.h"

#include "TerrainMaterial.h"
#include "Astra/graphics/loaders/Loader.h"
#include "Astra/graphics/ResourceManager.h"

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
		RESOURCE_UNLOAD(m_texture);
	}

	TerrainMaterialPack::~TerrainMaterialPack()
	{
		RESOURCE_UNLOAD(backgroundTexture);
		RESOURCE_UNLOAD(rTexture);
		RESOURCE_UNLOAD(gTexture);
		RESOURCE_UNLOAD(bTexture);
		RESOURCE_UNLOAD(specularTexture);
	}

	void TerrainMaterial::UpdateDiffuseMap(bool hdr)
	{
		Loader::UpdateDiffuseTexture(m_texture, hdr);
	}
}
