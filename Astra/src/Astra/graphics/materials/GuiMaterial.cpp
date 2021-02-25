#include "astra_pch.h"

#include "GuiMaterial.h"
#include "Astra/graphics/Resource.h"

namespace Astra::Graphics
{
	GuiMaterial::GuiMaterial()
		: m_texture(NULL), m_rowCount(1)
	{
	}

	GuiMaterial::GuiMaterial(const char* const filepath, int rowCount)
		: m_texture(Resource::LoadTexture(TextureCreationSpec(filepath, false))), m_rowCount(rowCount)
	{
		m_size = Math::Vec2(static_cast<float>(m_texture->width / m_rowCount), static_cast<float>(m_texture->height / m_rowCount));
	}
}