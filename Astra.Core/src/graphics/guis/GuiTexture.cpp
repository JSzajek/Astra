#include "GuiTexture.h"

#include "../ResourceManager.h"

namespace Astra::Graphics
{
	GuiTexture::GuiTexture(const GuiMaterial* material, const Math::Vec2& position, const Math::Vec2& scale)
		: Gui(material, position, 0, scale)
	{
	}

	GuiTexture::GuiTexture(const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
		: Gui(material, position, rotation, scale)
	{
	}
	
	GuiTexture::~GuiTexture()
	{
	}
}