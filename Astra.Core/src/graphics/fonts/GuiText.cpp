#include "GuiText.h"

#include "FontController.h"

namespace Astra::Graphics
{
	GuiText::GuiText(const std::string& text, FontType* font, float fontSize, const Math::Vec2& position, float maxLineLength, bool centered)
		: m_textString(text), m_fontSize(fontSize), m_font(font), Position(position), m_lineMaxSize(maxLineLength),
			m_centerText(centered), Color(Math::Zero), m_numberOfLines(0), m_vertexCount(0), m_textMeshVao(0), 
				m_thickness(0), m_outlineSize(0), m_outlineWidth(0), m_outlineEdge(0)
	{
		m_fontWidth = (0.0176 * log(fontSize) + 0.4677) + m_thickness;
		m_fontEdge = -0.062 * log(fontSize) + 0.1818;

		FontController::LoadText(*this);
	}

	GuiText::GuiText(const std::string& text, FontType* font, float fontSize, const Math::Vec2& position, const Math::Vec3& color, float thickness, 
						float outlineSize, const Math::Vec3& outlineColor, const Math::Vec2& offset, float maxLineLength, bool centered)
		: m_textString(text), Color(color), m_fontSize(fontSize), m_thickness(thickness), OutlineColor(outlineColor), m_outlineSize(outlineSize),
			Offset(offset), m_font(font), Position(position), m_lineMaxSize(maxLineLength), m_centerText(centered), m_numberOfLines(0), m_vertexCount(0), m_textMeshVao(0)
	{
		m_fontWidth = (0.0176 * log(fontSize) + 0.4677) + m_thickness;
		m_fontEdge = -0.062 * log(fontSize) + 0.1818;

		m_outlineWidth = (0.0176 * log(m_outlineSize) + m_fontWidth);
		m_outlineEdge = -0.062 * log(m_outlineSize) + 0.4;

		FontController::LoadText(*this);
	}

	GuiText::GuiText(const GuiText& other)
		: m_textString(other.m_textString), m_fontSize(other.m_fontSize), m_font(other.m_font), Position(other.Position), m_lineMaxSize(other.m_lineMaxSize), m_thickness(other.m_thickness),
			m_centerText(other.m_centerText), Color(other.Color), m_numberOfLines(other.m_numberOfLines), m_vertexCount(other.m_vertexCount), m_textMeshVao(other.m_textMeshVao),
				m_fontWidth(other.m_fontWidth), m_fontEdge(other.m_fontEdge), m_outlineSize(other.m_outlineSize), m_outlineWidth(other.m_outlineWidth), m_outlineEdge(other.m_outlineEdge), OutlineColor(other.OutlineColor)
	{
	}

	bool GuiText::operator==(const GuiText& other)
	{
		return m_textMeshVao == other.m_textMeshVao && m_textString == other.m_textString;
	}

	bool GuiText::operator!=(const GuiText& other)
	{
		return !(*this == other);
	}

	void GuiText::RemoveText()
	{
		FontController::RemoveText(*this);
	}

	void GuiText::SetMeshInfo(unsigned int vao, int vertexCount)
	{
		m_textMeshVao = vao;
		m_vertexCount = vertexCount;
	}
}