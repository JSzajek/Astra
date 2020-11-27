#include "GuiTexture.h"

#include "../ResourceManager.h"

namespace Astra::Graphics
{
	GuiTexture::GuiTexture(const char* name, const Math::Vec2& position, const Math::Vec2& scale)
		: Name(name), m_position(position), m_scale(scale), m_id(0), m_offset(-1), m_texture(NULL)
	{
	}

	GuiTexture::GuiTexture(const char* name, const Texture* texture, const Math::Vec2& position, const Math::Vec2& scale)
		: Name(name), m_position(position), m_scale(scale), m_id(texture->id), m_offset(-1), m_texture(texture)
	{
	}

	GuiTexture::~GuiTexture()
	{
		ResourceManager::Unload(m_texture);
	}
}