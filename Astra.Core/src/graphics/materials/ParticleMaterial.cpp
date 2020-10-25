#include "ParticleMaterial.h"

#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	ParticleMaterial::ParticleMaterial(const char* const filepath, int rowCount)
		: Texture(Loader::LoadTexture(filepath)), m_rowCount(1)
	{
	}
}