#include "GuiTexture.h"

namespace Astra::Graphics
{
	GuiTexture::GuiTexture(const Math::Vec2& position, const Math::Vec2& scale)
		: m_position(position), m_scale(scale), m_id(0)
	{
	}

	GuiTexture::GuiTexture(const GLuint& id, const Math::Vec2& position, const Math::Vec2& scale)
		: m_position(position), m_scale(scale), m_id(id)
	{
	}
}