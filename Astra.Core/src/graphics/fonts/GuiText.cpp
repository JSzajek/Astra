#include "GuiText.h"

#include "FontController.h"
#include "../ResourceManager.h"

namespace Astra::Graphics
{
	GuiText::GuiText(const std::string& text, FontType* font, float fontSize, const Math::Vec2& position, float maxLineLength, bool centered)
		: m_textString(text), m_fontSize(fontSize), m_font(font), Position(position), m_lineMaxSize(maxLineLength),
			m_centerText(centered), Color(Math::Vec3::Zero), m_numberOfLines(0), m_vertexCount(0), m_textMeshVao(0), 
				m_thickness(0), m_outlineSize(0), m_outlineWidth(0), m_outlineEdge(0)
	{
		m_fontWidth = (0.0176f * logf(fontSize) + 0.4677f) + m_thickness;
		m_fontEdge = -0.062f * logf(fontSize) + 0.1818f;

		FontController::LoadText(this);
	}

	GuiText::GuiText(const std::string& text, FontType* font, float fontSize, const Math::Vec2& position, const Math::Vec3& color, float thickness, 
						float outlineSize, const Math::Vec3& outlineColor, const Math::Vec2& offset, float maxLineLength, bool centered)
		: m_textString(text), Color(color), m_fontSize(fontSize), m_thickness(thickness), OutlineColor(outlineColor), m_outlineSize(outlineSize),
			Offset(offset), m_font(font), Position(position), m_lineMaxSize(maxLineLength), m_centerText(centered), m_numberOfLines(0), m_vertexCount(0), m_textMeshVao(0)
	{
		m_fontWidth = (0.0176f * logf(fontSize) + 0.4677f) + m_thickness;
		m_fontEdge = -0.062f * logf(fontSize) + 0.1818f;

		m_outlineWidth = (0.0176f * logf(m_outlineSize) + m_fontWidth);
		m_outlineEdge = -0.062f * logf(m_outlineSize) + 0.4f;

		FontController::LoadText(this);
	}

	GuiText::~GuiText()
	{
		//FontController::RemoveText(this);
	}

	bool GuiText::operator==(const GuiText& other)
	{
		return m_textMeshVao == other.m_textMeshVao && m_textString == other.m_textString;
	}

	bool GuiText::operator!=(const GuiText& other)
	{
		return !(*this == other);
	}

	void GuiText::SetMeshInfo(unsigned int vao, int vertexCount)
	{
		m_textMeshVao = vao;
		m_vertexCount = vertexCount;
	}
}