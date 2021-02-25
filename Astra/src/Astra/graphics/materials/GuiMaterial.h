#pragma once

#include "Astra/math/Vec2.h"
#include "Astra/graphics/buffers/Texture.h"

namespace Astra::Graphics
{
	struct GuiMaterial
	{
	private:
		Asset<Texture> m_texture;
		size_t m_rowCount;
		Math::Vec2 m_size;
	public:
		GuiMaterial();
		GuiMaterial(const char* const filepath, int rowCount = 1);
		
		inline const Math::Vec2& GetSize() const { return m_size; }

		inline const Asset<Texture> GetTexture() const { return m_texture; }
		inline int GetRowCount() const { return m_rowCount; }
		inline unsigned int GetId() const { return m_texture->id; }
	};
}
