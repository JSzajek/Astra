#pragma once

#include "Gui.h"
#include "TextBox.h"

namespace Astra::Graphics
{
	class Button : public Gui
	{
	private:
		TextBox m_text;
		Color m_outputColor;
		Color m_hoverColor;
		Color m_pressedColor;
		bool m_hovering;
		bool m_pressing;
	public:
		Button()
			: Gui(), m_hoverColor(), m_pressedColor(), m_hovering(0), m_pressing(0), m_text()
		{
		}

		Button(const char* const name, const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
			: Gui(name, material, position, rotation, scale), m_hoverColor(), m_pressedColor(), m_hovering(0), m_pressing(0), m_text("", position, rotation, scale)
		{
			m_rect.SetSize(Math::iVec2(static_cast<int>(m_rows[1].x * Material->GetSize().x), static_cast<int>(m_rows[1].y * Material->GetSize().y)));
			UpdateMatrices();
		}

		Button(const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
			: Gui(material, position, rotation, scale), m_hoverColor(), m_pressedColor(), m_hovering(0), m_pressing(0), m_text("", position, rotation, scale)
		{
			m_rect.SetSize(Math::iVec2(static_cast<int>(m_rows[1].x * Material->GetSize().x), static_cast<int>(m_rows[1].y * Material->GetSize().y)));
			UpdateMatrices();
		}

		Button(const GuiMaterial* material, const Math::Vec2& position, const Math::Vec2& scale)
			: Button(material, position, 0, scale)
		{
		}

		Button(const char* const name, const GuiMaterial* material, const Math::Vec2& position, const Math::Vec2& scale)
			: Button(name, material, position, 0, scale)
		{
		}
		
		virtual inline GuiType GetType() const override { return GuiType::Button; }
		virtual void Free() override { }
		inline virtual std::string ToString() const override { return !Name.length() ? ("Button_&" + std::to_string(m_uid)) : Name; }

		inline void OnHover() override
		{
			Gui::OnHover();
			if (!m_pressing)
			{
				m_outputColor = m_modulate * m_hoverColor;
			}
		}

		inline void OnExit() override
		{
			Gui::OnExit();
			if (!m_pressing)
			{
				m_outputColor = m_modulate;
			}
		}

		inline void OnPressed() override
		{
			Gui::OnPressed();
			m_pressing = true;
			m_outputColor = m_modulate * m_pressedColor;
		}

		inline void OnReleased(bool hovering) override
		{
			Gui::OnReleased(hovering);
			m_pressing = false;
			if (!hovering)
				m_outputColor = m_modulate;
			else
				m_outputColor = m_modulate * m_hoverColor;
		}

		inline const Color& GetModulate() const override { return m_outputColor; }

		inline void SetHoverColor(const Color& color) { m_hoverColor = color; }
		inline void SetPressedColor(const Color& color) { m_pressedColor = color; }

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
			m_rows[1] = copy * Material->GetSize();
			Spatial2D::UpdateMatrices();
			m_rows[1] = copy;
		}
	};
}