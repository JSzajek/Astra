#pragma once

#include "HeightGenerator.h"
#include "../Spatial.h"
#include "../../buffers/VertexArray.h"
#include "../../materials/TerrainMaterial.h"
#include "../../loaders/Loader.h"

namespace Astra::Graphics
{
	#define MAX_VERTICES	128

	class Terrain : public Spatial
	{
	private:
		static constexpr float Size = 256;
		static constexpr float MaxHeight = 40;
	private:
		float* m_heights;
		int m_vertexCount;
	public:
		const VertexArray* vertexArray;
		const TerrainMaterialPack* texturePack;
		const TerrainMaterial* blendMap;
	public:
		Terrain();
		Terrain(int xGrid, int zGrid, const char* const heightmap, const TerrainMaterialPack* pack, const TerrainMaterial* map);
		Terrain(int xGrid, int zGrid, float amplitude, int octaves, float roughness, const TerrainMaterialPack* pack, const TerrainMaterial* map);
		Terrain(int xGrid, int zGrid, float amplitude, int octaves, float roughness, int seed, const TerrainMaterialPack* pack, const TerrainMaterial* map);
		~Terrain();

		float GetHeightOfTerrain(int xWorldCoord, int zWorldCoord);
	private:
		const VertexArray* GeneratePlaneTerrain(const char* const heightmap);
		const VertexArray* GeneratePlaneTerrain(HeightGenerator* const generator);
		float GetHeight(int x, int z, const unsigned char* buffer, const int& imageHeight);
		float GetHeight(int x, int z, HeightGenerator* const generator);
		Math::Vec3 CalculateNormal(int x, int z, const unsigned char* buffer, const int& imageHeight);
		Math::Vec3 CalculateNormal(int x, int z, HeightGenerator* const generator);
	};
}