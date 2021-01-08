#pragma once

#include "Gui.h"

namespace Astra::Graphics
{
	class GuiTexture : public Gui
	{
	public:
		GuiTexture(const GuiMaterial* material, const Math::Vec2& position, const Math::Vec2& scale);
		GuiTexture(const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale);
		~GuiTexture();
	};
}