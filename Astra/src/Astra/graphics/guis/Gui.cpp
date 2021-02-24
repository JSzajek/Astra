#include "astra_pch.h"

#include "Gui.h"
#include "Astra/graphics/ResourceManager.h"

namespace Astra::Graphics
{
	Gui::Gui()
		: Spatial2D(), Material(NULL), m_rect()
	{
	}

	Gui::Gui(const char* const name, const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
		: Spatial2D(name, position, rotation, scale), Material(material), m_rect(m_rows[0].x, m_rows[0].y, m_rows[1].x, m_rows[1].y)
	{
	}

	Gui::Gui(const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
		: Spatial2D(position, rotation, scale), Material(material), m_rect(m_rows[0].x, m_rows[0].y, m_rows[1].x, m_rows[1].y)
	{
	}

	Gui::Gui(const Gui& other)
		: Spatial2D(other), m_modulate(other.m_modulate), Material(other.Material), m_rect(other.m_rect)
	{
	}

	Gui::~Gui()
	{
		//RESOURCE_UNLOAD(Material);
	}
}