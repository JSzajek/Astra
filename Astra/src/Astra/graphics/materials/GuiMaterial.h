#pragma once

#include "Astra/math/Vec2.h"
#include "Astra/graphics/buffers/Texture.h"

namespace Astra::Graphics
{
	struct GuiMaterial
	{
	private:
		Texture* m_texture;
		size_t m_rowCount;
		Math::Vec2 m_size;
	public:
		GuiMaterial();
		GuiMaterial(const char* const filepath, int rowCount = 1);
		GuiMaterial(const GuiMaterial& other);
		void operator=(const GuiMaterial& other);
		~GuiMaterial();
		
		inline const Math::Vec2& GetSize() const { return m_size; }

		inline const Texture* GetTexture() const { return m_texture; }
		inline int GetRowCount() const { return m_rowCount; }
		inline unsigned int GetId() const { return m_texture->id; }
	};
}
