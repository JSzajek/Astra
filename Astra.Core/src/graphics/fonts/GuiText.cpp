#include "GuiText.h"

#include "FontController.h"

namespace Astra::Graphics
{
	GuiText::GuiText(const std::string& text, float fontSize, FontType* font, const Math::Vec2& position, float maxLineLength, bool centered)
		: m_textString(text), m_fontSize(fontSize), m_font(font), m_position(position), m_lineMaxSize(maxLineLength),
			m_centerText(centered), m_color(Math::One), m_numberOfLines(0), m_vertexCount(0), m_textMeshVao(0)
	{
		FontController::LoadText(*this);
	}

	GuiText::GuiText(const GuiText& other)
		: m_textString(other.m_textString), m_fontSize(other.m_fontSize), m_font(other.m_font), m_position(other.m_position), m_lineMaxSize(other.m_lineMaxSize),
			m_centerText(other.m_centerText), m_color(other.m_color), m_numberOfLines(other.m_numberOfLines), m_vertexCount(other.m_vertexCount), m_textMeshVao(other.m_textMeshVao)
	{
	}

	GuiText::~GuiText()
	{
		// TODO: Determine the best time to delete font pointers
		//delete m_font;
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

	void GuiText::SetColor(float r, float g, float b)
	{
		m_color.x = r;
		m_color.y = g;
		m_color.z = b;
	}

	void GuiText::SetMeshInfo(unsigned int vao, int vertexCount)
	{
		m_textMeshVao = vao;
		m_vertexCount = vertexCount;
	}
}