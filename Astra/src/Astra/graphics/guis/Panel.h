#pragma once

#include "Gui.h"
#include "TextBox.h"

namespace Astra::Graphics
{
	// Panel is a combination of a image (panel) and potentially textbox text
	// unlike a button it doesn't have hover/pressed detection
	class Panel : public Gui
	{
	private:
		TextBox m_text;
	public:
		Panel()
			: Gui(), m_text()
		{
		}

		Panel(const char* const name, const GuiMaterial& material, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
			: Gui(name, material, position, rotation, scale), m_text("", position, rotation, scale)
		{
			m_rect.SetSize(Math::iVec2(static_cast<int>(m_rows[1].x * Material.GetSize().x), static_cast<int>(m_rows[1].y * Material.GetSize().y)));
			UpdateMatrices();
		}


		Panel(const GuiMaterial& material, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
			: Gui(material, position, rotation, scale), m_text("", position, rotation, scale)
		{
			m_rect.SetSize(Math::iVec2(static_cast<int>(m_rows[1].x * Material.GetSize().x), static_cast<int>(m_rows[1].y * Material.GetSize().y)));
			UpdateMatrices();
		}

		Panel(const GuiMaterial& material, const Math::Vec2& position, const Math::Vec2& scale)
			: Panel(material, position, 0, scale)
		{
		}

		Panel(const char* const name, const GuiMaterial& material, const Math::Vec2& position, const Math::Vec2& scale)
			: Panel(name, material, position, 0, scale)
		{
		}

		virtual inline GuiType GetType() const override { return GuiType::Panel; }
		virtual void Free() override { }
		inline virtual std::string ToString() const override { return !Name.length() ? ("Panel_&" + std::to_string(m_uid)) : Name; }

		inline void SetText(std::string text) { m_text.SetText(text); }
		inline TextBox* GetTextBox() { return &m_text; }

		void SetTranslation(const Math::Vec2& translation) override
		{
			Spatial2D::SetTranslation(translation);
			m_text.SetTranslation(translation);
		}

		void operator()(unsigned int _type, unsigned int _op, unsigned int _index, float _val) override
		{
			Spatial2D::operator()(_type, _op, _index, _val);
			m_text.operator()(_type, _op, _index, _val);
		}

		void operator()(unsigned int _type, unsigned int _op, float _val) override
		{
			Spatial2D::operator()(_type, _op, _val);
			m_text.operator()(_type, _op, _val);
		}
	protected:
		void UpdateMatrices() override
		{
			Math::Vec2 copy = m_rows[1];
			m_rows[1] = copy * Material.GetSize();
			Spatial2D::UpdateMatrices();
			m_rows[1] = copy;
		}
	};
}