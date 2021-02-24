#pragma once

#include "Gui.h"

namespace Astra::Graphics
{
	// TODO: Refactor and rethink the difference between a label and a panel
	class Label : public Gui
	{
	private:
		unsigned int m_textureIndex;
		//FontType* m_font;
		//GuiText* m_text;
	public:
		Label(const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale, unsigned int textureIndex = 1)
			: Gui(material, position, rotation, scale), m_textureIndex(textureIndex)/*, m_text(NULL), m_font(NULL)*/
		{
		}

		virtual inline GuiType GetType() const override { return GuiType::Label; }

		inline float GetMaterialXOffset() const { return (float)(m_textureIndex % Material->GetRowCount()) / (float)Material->GetRowCount(); }
		inline float GetMaterialYOffset() const { return (float)(m_textureIndex / Material->GetRowCount()) / (float)Material->GetRowCount(); }
		
		inline unsigned int GetTextureIndex() const { return m_textureIndex; }
	};
}