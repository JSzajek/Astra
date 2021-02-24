#pragma once

#include "Astra/graphics/buffers/Texture.h"

namespace Astra::Graphics
{
	class TerrainMaterial
	{
	private:
		Texture* m_blendMapTexture;
		Texture* m_backgroundTexture;
		Texture* m_rTexture;
		Texture* m_gTexture;
		Texture* m_bTexture;
		Texture* specularTexture;
	public:
		float shineDampener, reflectivity;
	public:
		TerrainMaterial();
		TerrainMaterial(const char* const blendMap, const char* const background, 
						const char* const red, const char* const green, const char* const blue);
		TerrainMaterial(const TerrainMaterial& other);
		void operator=(const TerrainMaterial& other);
		~TerrainMaterial();

		inline unsigned int GetBackgroundId() const { return m_backgroundTexture->id; }
		inline unsigned int GetBlendMapId() const { return m_blendMapTexture->id; }
		inline unsigned int GetRedId() const { return m_rTexture->id; }
		inline unsigned int GetGreenId() const { return m_gTexture->id; }
		inline unsigned int GetBlueId() const { return m_bTexture->id; }
		inline unsigned int GetSpecularId() const { return specularTexture->id; }
	};
}