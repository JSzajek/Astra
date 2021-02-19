#pragma once

#include "Gui.h"
#include "TextBox.h"

namespace Astra::Graphics
{
	class ToggleButton : public Gui
	{
	private:
		TextBox m_text;
		Color m_outputColor;
		Color m_hoverColor;
		Color m_toggledColor;
		bool m_hovering;
		bool m_pressing;
		bool m_toggled;
	public:
		ToggleButton(const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
			: Gui(material, position, rotation, scale), m_hoverColor(), m_toggledColor(), m_hovering(0), m_pressing(0), m_text("", position, rotation, scale)
		{
			m_rect.SetSize(Math::iVec2(static_cast<int>(m_rows[1].x * Material->GetSize().x), static_cast<int>(m_rows[1].y * Material->GetSize().y)));
			SetType(GuiType::Toggle);
			UpdateMatrices();
		}

		ToggleButton(const GuiMaterial* material, const Math::Vec2& position, const Math::Vec2& scale)
			: ToggleButton(material, position, 0, scale)
		{
		}

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
			SetToggled(!m_toggled);
		}

		inline void OnReleased(bool hovering) override
		{
			Gui::OnReleased(hovering);
			m_pressing = false;
			if (!hovering)
			{
				m_outputColor = m_toggled ? m_toggledColor : m_modulate;
			}
			else
			{
				if (m_toggled)
				{
					m_outputColor = m_modulate * m_toggledColor * m_hoverColor;
				}
				else
				{
					m_outputColor = m_modulate * m_hoverColor;
				}
			}
		}

		void SetToggled(bool enabled) 
		{ 
			m_toggled = enabled;
			if (m_toggled)
			{
				m_outputColor = m_modulate * m_toggledColor;
			}
			else
			{
				m_outputColor = m_modulate;
			}
		}
		inline bool IsToggled() const { return m_toggled; }

		inline const Color& GetModulate() const override { return m_outputColor; }

		inline void SetHoverColor(const Color& color) { m_hoverColor = color; }
		inline void SetToggledColor(const Color& color) { m_toggledColor = color; }

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