#pragma once

#include "Spatial.h"
#include "../buffers/Texture.h"
#include "../buffers/VertexArray.h"
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
		const Texture* texture;
	public:
		Terrain(int xGrid, int zGrid, Texture* texture);
	private:
		const VertexArray* GeneratePlaneTerrain();
	};
}