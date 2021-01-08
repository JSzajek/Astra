#pragma once

#include "../buffers/Texture.h"

namespace Astra::Graphics
{
	struct GuiMaterial
	{
	private:
		const Texture* m_texture;
		int m_rowCount;
	public:
		GuiMaterial(const char* const filepath, int rowCount);
		~GuiMaterial();
		
		inline const Texture* GetTexture() const { return m_texture; }
		inline int GetRowCount() const { return m_rowCount; }
		inline unsigned int GetId() const { return m_texture->id; }
	};
}
