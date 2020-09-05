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
		static constexpr float Size = 40;
		static constexpr float VertexCount = 6;
	public:
		const VertexArray* vertexArray;
		const TerrainMaterial* material;
	public:
		Terrain(int xGrid, int zGrid, TerrainMaterial* material);
	private:
		const VertexArray* GeneratePlaneTerrain();
	};
}