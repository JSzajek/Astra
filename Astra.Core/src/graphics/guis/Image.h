#pragma once

#include "Gui.h"

namespace Astra::Graphics
{
	class Image : public Gui
	{
	private:
		unsigned int m_textureIndex;
	public:
		Image(const GuiMaterial* material, const Math::Vec2& position, float rotation, const Math::Vec2& scale, unsigned int textureIndex = 0)
			: Gui(material, position, rotation, scale), m_textureIndex(textureIndex)
		{
			SetType(GuiType::Image);
			UpdateMatrices();
		}

		Image(const GuiMaterial* material, const Math::Vec2& position, const Math::Vec2& scale, unsigned int textureIndex = 0)
			: Image(material, position, 0, scale, textureIndex)
		{
		}

		inline float GetMaterialXOffset() const { return (float)(m_textureIndex % Material->GetRowCount()) / (float)Material->GetRowCount(); }
		inline float GetMaterialYOffset() const { return (float)(m_textureIndex / Material->GetRowCount()) / (float)Material->GetRowCount(); }
		inline unsigned int GetTextureIndex() const { return m_textureIndex; }
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