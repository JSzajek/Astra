#pragma once

#include "Spatial.h"
#include "../buffers/VertexArray.h"
#include "../materials/TerrainMaterial.h"
#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	class Terrain : public Spatial
	{
	private:
		static constexpr float Size = 800;
		//static constexpr float VertexCount = 128;
		static constexpr float MaxHeight = 40;
	private:
		float* m_heights;
		int m_vertexCount;
	public:
		const VertexArray* vertexArray;
		const TerrainMaterialPack* texturePack;
		const TerrainMaterial* blendMap;
	public:
		Terrain(int xGrid, int zGrid, const TerrainMaterialPack* pack, const TerrainMaterial* map);
		~Terrain();

		float GetHeightOfTerrain(int xWorldCoord, int zWorldCoord);
	private:
		const VertexArray* GeneratePlaneTerrain(const char* const heightmap);
		float GetHeight(int x, int z, const unsigned char* buffer, const int& imageHeight);
		Math::Vec3 CalculateNormal(int x, int z, const unsigned char* buffer, const int& imageHeight);
	};
}