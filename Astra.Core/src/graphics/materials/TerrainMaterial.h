#pragma once

#include "../buffers/Texture.h"

namespace Astra::Graphics
{
	class TerrainMaterial
	{
	private:
		const Texture* m_texture;
	public:
		float shineDampener, reflectivity;
	public:
		TerrainMaterial(const char* const filepath);
		~TerrainMaterial();

		inline unsigned int GetId() const { return m_texture->id; }
	};
	
	struct TerrainMaterialPack
	{
		const TerrainMaterial* backgroundTexture;
		const TerrainMaterial* rTexture;
		const TerrainMaterial* gTexture;
		const TerrainMaterial* bTexture;

		const Texture* specularTexture;
	
		TerrainMaterialPack(const TerrainMaterial* background, const TerrainMaterial* red,
							const TerrainMaterial* green, const TerrainMaterial* blue);
		~TerrainMaterialPack();
	};
}