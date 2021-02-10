#include "Gui.h"

#include "../ResourceManager.h"

namespace Astra::Graphics
{
	Gui::Gui(const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
		: Spatial2D(position, rotation, scale), Material(material)
	{
	}

	Gui::Gui(const GuiMaterial* material, const char* const name, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
		: Spatial2D(position, rotation, scale), Material(material), Name(name)
	{
	}

	Gui::Gui(const Gui& other)
		: Spatial2D(other), Name(other.Name), m_modulate(other.m_modulate),
			m_type(other.m_type), Material(other.Material)
	{
	}

	Gui::~Gui()
	{
		RESOURCE_UNLOAD(Material);
	}
}