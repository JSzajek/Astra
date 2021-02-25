#pragma once

#include "Gui.h"

namespace Astra::Graphics
{
	class Image : public Gui
	{
	private:
		unsigned int m_textureIndex;
	public:
		Image()
			: Gui(), m_textureIndex(0)
		{
		}

		Image(const char* const name, const GuiMaterial& material, const Math::Vec2& position, float rotation, const Math::Vec2& scale, unsigned int textureIndex = 0)
			: Gui(name, material, position, rotation, scale), m_textureIndex(textureIndex)
		{
			m_rect.SetSize(Math::iVec2(static_cast<int>(m_rows[1].x * Material.GetSize().x), static_cast<int>(m_rows[1].y * Material.GetSize().y)));
			UpdateMatrices();
		}

		Image(const GuiMaterial& material, const Math::Vec2& position, float rotation, const Math::Vec2& scale, unsigned int textureIndex = 0)
			: Gui(material, position, rotation, scale), m_textureIndex(textureIndex)
		{
			m_rect.SetSize(Math::iVec2(static_cast<int>(m_rows[1].x * Material.GetSize().x), static_cast<int>(m_rows[1].y * Material.GetSize().y)));
			UpdateMatrices();
		}

		Image(const GuiMaterial& material, const Math::Vec2& position, const Math::Vec2& scale, unsigned int textureIndex = 0)
			: Image(material, position, 0, scale, textureIndex)
		{
		}

		Image(const char* const name, const GuiMaterial& material, const Math::Vec2& position, const Math::Vec2& scale, unsigned int textureIndex = 0)
			: Image(name, material, position, 0, scale, textureIndex)
		{
		}

		virtual inline GuiType GetType() const override { return GuiType::Image; }
		inline virtual std::string ToString() const override { return !Name.length() ? ("Image_&" + std::to_string(m_uid)) : Name; }

		inline float GetMaterialXOffset() const { return (float)(m_textureIndex % Material.GetRowCount()) / (float)Material.GetRowCount(); }
		inline float GetMaterialYOffset() const { return (float)(m_textureIndex / Material.GetRowCount()) / (float)Material.GetRowCount(); }
		inline unsigned int GetTextureIndex() const { return m_textureIndex; }
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