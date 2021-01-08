#include "GuiMaterial.h"

#include "../loaders/Loader.h"
#include "../ResourceManager.h"

namespace Astra::Graphics
{
	GuiMaterial::GuiMaterial(const char* const filepath, int rowCount)
		: m_texture(Loader::LoadTexture(filepath, false, GL_REPEAT, false)), m_rowCount(rowCount)
	{
	}

	GuiMaterial::~GuiMaterial()
	{
		ResourceManager::Unload(m_texture);
	}
}