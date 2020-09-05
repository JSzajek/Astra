#include "TerrainMaterial.h"

#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	TerrainMaterial::TerrainMaterial(const char* const filepath)
		: Texture(filepath)
	{
		Loader::BufferTexture(this);
	}
}
