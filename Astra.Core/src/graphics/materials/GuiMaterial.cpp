#include "GuiMaterial.h"

#include "../loaders/Loader.h"
#include "../ResourceManager.h"

namespace Astra::Graphics
{
	GuiMaterial::GuiMaterial(const char* const filepath, int rowCount)
		: m_texture(Loader::LoadTexture(filepath, false, GL_REPEAT, false)), m_rowCount(rowCount)
	{
		m_size = Math::Vec2(static_cast<float>(m_texture->width / m_rowCount), static_cast<float>(m_texture->height / m_rowCount));
	}

	GuiMaterial::~GuiMaterial()
	{
		ResourceManager::Unload(m_texture);
	}
}