#include "GuiTexture.h"

namespace Astra::Graphics
{
	GuiTexture::GuiTexture(const char* name, const Math::Vec2& position, const Math::Vec2& scale)
		: Name(name), m_position(position), m_scale(scale), m_id(0), m_offset(-1)
	{
	}

	GuiTexture::GuiTexture(const char* name, const unsigned int& id, const Math::Vec2& position, const Math::Vec2& scale)
		: Name(name), m_position(position), m_scale(scale), m_id(id), m_offset(-1)
	{
	}
}