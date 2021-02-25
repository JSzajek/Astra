#include "astra_pch.h"

#include "ParticleMaterial.h"
#include "Astra/graphics/Resource.h"

namespace Astra::Graphics
{
	ParticleMaterial::ParticleMaterial()
		: m_texture(NULL), m_rowCount(1)
	{
	}

	ParticleMaterial::ParticleMaterial(const char* const filepath, unsigned int rowCount)
		: m_texture(Resource::LoadTexture(TextureCreationSpec(filepath, true, false))), m_rowCount(rowCount)
	{
	}

	ParticleMaterial::ParticleMaterial(const ParticleMaterial& other)
		: m_texture(other.m_texture), m_rowCount(other.m_rowCount)
	{
	}

	void ParticleMaterial::operator=(const ParticleMaterial& other)
	{
		m_texture = other.m_texture;
		m_rowCount = other.m_rowCount;
	}
}