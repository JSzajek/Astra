#pragma once

#include "Astra/graphics/buffers/Texture.h"

namespace Astra::Graphics
{
	class ParticleMaterial
	{
	private:
		Asset<Texture> m_texture;
		unsigned int m_rowCount;
	public:
		ParticleMaterial();
		ParticleMaterial(const char* const filepath, unsigned int rowCount);
		ParticleMaterial(const ParticleMaterial& other);
		void operator=(const ParticleMaterial& other);

		inline const Asset<Texture> GetTexture() const { return m_texture; }
		inline int GetRowCount() const { return m_rowCount; }
		inline unsigned int GetId() const { return m_texture->id; }
	};
}