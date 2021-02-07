#pragma once

#include "../../math/Vec2.h"
#include "../buffers/Texture.h"

namespace Astra::Graphics
{
	struct GuiMaterial
	{
	private:
		const Texture* m_texture;
		size_t m_rowCount;
		Math::Vec2 m_size;
	public:
		GuiMaterial(const char* const filepath, int rowCount);
		~GuiMaterial();
		
		inline const Math::Vec2& GetSize() const { return m_size; }

		inline const Texture* GetTexture() const { return m_texture; }
		inline int GetRowCount() const { return m_rowCount; }
		inline unsigned int GetId() const { return m_texture->id; }
	};
}
