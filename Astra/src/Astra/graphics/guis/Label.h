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
		Label()
			: Gui(), m_textureIndex(0)
		{
		}

		Label(const GuiMaterial& material, const Math::Vec2& position, float rotation, const Math::Vec2& scale, unsigned int textureIndex = 1)
			: Gui(material, position, rotation, scale), m_textureIndex(textureIndex)/*, m_text(NULL), m_font(NULL)*/
		{
		}

		Label(const char* const name, const GuiMaterial& material, const Math::Vec2& position, float rotation, const Math::Vec2& scale, unsigned int textureIndex = 1)
			: Gui(name, material, position, rotation, scale), m_textureIndex(textureIndex)/*, m_text(NULL), m_font(NULL)*/
		{
		}

		virtual inline GuiType GetType() const override { return GuiType::Label; }
		inline virtual std::string ToString() const override { return !Name.length() ? ("Label_&" + std::to_string(m_uid)) : Name; }

		inline float GetMaterialXOffset() const { return (float)(m_textureIndex % Material.GetRowCount()) / (float)Material.GetRowCount(); }
		inline float GetMaterialYOffset() const { return (float)(m_textureIndex / Material.GetRowCount()) / (float)Material.GetRowCount(); }
		
		inline unsigned int GetTextureIndex() const { return m_textureIndex; }
	};
}