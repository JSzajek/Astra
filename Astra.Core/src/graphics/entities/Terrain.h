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
		static constexpr float VertexCount = 128;
	public:
		const VertexArray* vertexArray;
		const TerrainMaterialPack* texturePack;
		const TerrainMaterial* blendMap;
	public:
		Terrain(int xGrid, int zGrid, const TerrainMaterialPack* pack, const TerrainMaterial* map);
		~Terrain();
	private:
		const VertexArray* GeneratePlaneTerrain();
	};
}