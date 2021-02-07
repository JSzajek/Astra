#pragma once

#include "Gui.h"
#include "Color.h"

namespace Astra::Graphics
{
	class Panel : public Gui
	{
	private:
		unsigned int m_vao;
		unsigned int m_vbo;
		Math::Vec2 m_size;
	public:
		Panel(const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
			: Gui(material, position, rotation, scale), m_vao(0), m_vbo(0), m_size(1)
		{
		}

		inline const bool HasCustomVao() const override { return true; }
		inline const int GetCustomVao() const override { return m_vao; }
		inline const int GetCustomVbo() const override { return m_vbo; }

		inline const Math::Vec2& GetSize() const override { return m_size; }
	};
}