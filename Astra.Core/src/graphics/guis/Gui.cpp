#include "Gui.h"

#include "../ResourceManager.h"

namespace Astra::Graphics
{
	Gui::Gui(const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
		: Material(material), Position(position), Rotation(rotation), Scale(scale)
	{
	}

	Gui::Gui(const GuiMaterial* material, const char* const name, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
		: Gui(material, position, rotation, scale)
	{
		Name = name;
	}

	Gui::~Gui()
	{
		ResourceManager::Unload(Material);
	}
}