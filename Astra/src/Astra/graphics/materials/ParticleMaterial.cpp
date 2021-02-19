#include "astra_pch.h"

#include "ParticleMaterial.h"
#include "Astra/graphics/loaders/Loader.h"
#include "Astra/graphics/ResourceManager.h"

namespace Astra::Graphics
{
	ParticleMaterial::ParticleMaterial(const char* const filepath, int rowCount)
		: m_texture(Loader::LoadTexture(filepath, true, GL_REPEAT, false)), m_rowCount(rowCount)
	{
	}

	ParticleMaterial::~ParticleMaterial()
	{
		RESOURCE_UNLOAD(m_texture);
	}

	void ParticleMaterial::UpdateDiffuseMap(bool hdr)
	{
		Loader::UpdateDiffuseTexture(m_texture, hdr);
	}
}