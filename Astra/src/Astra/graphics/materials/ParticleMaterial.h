#pragma once

#include "Astra/graphics/buffers/Texture.h"

namespace Astra::Graphics
{
	class ParticleMaterial
	{
	private:
		Texture* m_texture;
		int m_rowCount;
	public:
		ParticleMaterial(const char* const filepath, int rowCount);
		~ParticleMaterial();

		inline const Texture* GetTexture() const { return m_texture; }
		inline int GetRowCount() const { return m_rowCount; }
		inline unsigned int GetId() const { return m_texture->id; }
	public:
		void UpdateDiffuseMap(bool hdr);
	};
}