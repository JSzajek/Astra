#pragma once

#include "Gui.h"
#include "../fonts/GuiText.h"

namespace Astra::Graphics
{
	class Label : public Gui
	{
	private:
		unsigned int m_textureIndex;
		FontType* m_font;
		GuiText* m_text;
	public:
		Label(const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale, unsigned int textureIndex = 1)
			: Gui(material, position, rotation, scale), m_textureIndex(textureIndex), m_text(NULL), m_font(NULL)
		{
			SetType(GuiType::Label);
			/*Position = position;
			Position += Math::Vec2::One;
			Position /= 2.0f;*/
		}

		void SetFont(FontType* font)
		{
			m_font = font;
		}

		void SetText(const char* const text) 
		{
			if (m_text == NULL && m_font)
			{
				m_text = new GuiText(text, m_font, 3, Position, 0.5f, true);
			}
		}
		
		inline GuiText* GetText() const { return m_text; }

		inline float GetMaterialXOffset() const { return (float)(m_textureIndex % Material->GetRowCount()) / (float)Material->GetRowCount(); }
		inline float GetMaterialYOffset() const { return (float)(m_textureIndex / Material->GetRowCount()) / (float)Material->GetRowCount(); }
		
		inline unsigned int GetTextureIndex() const { return m_textureIndex; }
	};
}