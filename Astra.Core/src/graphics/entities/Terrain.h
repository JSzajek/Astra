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
		static constexpr float Size = 200;
		static constexpr float VertexCount = 80;
	public:
		Terrain(unsigned int width, unsigned int height, Texture* texture);
	private:
		const VertexArray* GeneratePlaneTerrain();
	};
}