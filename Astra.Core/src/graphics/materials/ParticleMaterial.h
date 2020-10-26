#pragma once

#include "../buffers/Texture.h"

namespace Astra::Graphics
{
	class ParticleMaterial : public Texture
	{
	private:
		int m_rowCount;
	public:
		ParticleMaterial(const char* const filepath, int rowCount);
		
		inline int GetRowCount() const { return m_rowCount; }
	};
}