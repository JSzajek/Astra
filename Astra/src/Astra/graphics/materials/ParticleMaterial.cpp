#include "astra_pch.h"

#include "ParticleMaterial.h"
#include "../loaders/Loader.h"
#include "../ResourceManager.h"

namespace Astra::Graphics
{
	ParticleMaterial::ParticleMaterial(const char* const filepath, int rowCount)
		: m_texture(Loader::LoadTexture(filepath, true, GL_REPEAT, false)), m_rowCount(rowCount)
	{
	}

	ParticleMaterial::~ParticleMaterial()
	{
		ResourceManager::Unload(m_texture);
	}
}