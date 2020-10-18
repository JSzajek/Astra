#include "TextMesh.h"
#include "../../math/Maths.h"

namespace Astra::Graphics
{
	GuiText::GuiText(const std::string& text, float fontSize, const FontType& font, const Math::Vec2& position, float maxLineLength, bool centered)
		: m_textString(text), m_fontSize(fontSize), m_font(font), m_position(position), m_lineMaxSize(maxLineLength),
			m_centerText(centered), m_color(Math::One), m_numberOfLines(0), m_vertexCount(0), m_textMeshVao(0)
	{
	}

	GuiText::GuiText(const GuiText& other)
		: m_textString(other.m_textString), m_fontSize(other.m_fontSize), m_font(other.m_font), m_position(other.m_position), m_lineMaxSize(other.m_lineMaxSize),
		m_centerText(other.m_centerText), m_color(other.m_color), m_numberOfLines(other.m_numberOfLines), m_vertexCount(other.m_vertexCount), m_textMeshVao(other.m_textMeshVao)
	{
	}

	void GuiText::RemoveText()
	{
		// Remove Text
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

	FontType::FontType(int textureAtlas, const char* filepath)
		: m_textureAtlas(textureAtlas), m_loader(new TextMeshCreator(filepath))
	{
	}

	FontType::FontType(const FontType& other)
		: m_textureAtlas(other.m_textureAtlas), m_loader(other.m_loader)
	{
	}

	FontType::~FontType()
	{
		delete m_loader;
	}

	TextMeshData::TextMeshData(const std::vector<float>& vertexPositions, const std::vector<float>& textureCoords)
		: VertexPositions(vertexPositions), TextureCoords(textureCoords)
	{
	}

	TextMeshData::TextMeshData(const TextMeshData& other)
		: VertexPositions(other.VertexPositions), TextureCoords(other.TextureCoords)
	{
	}

	TextMeshCreator::TextMeshCreator(const char* filepath)
		: metaData(filepath)
	{
	}

	const TextMeshData& TextMeshCreator::CreateTextMesh(GuiText& text)
	{
		std::vector<Line> lines = CreateStructure(text);
		TextMeshData data = CreateQuadVertices(text, lines);
		return data;
	}

	std::vector<Line> TextMeshCreator::CreateStructure(GuiText& text)
	{
		std::vector<Line> lines;
		Line currentLine(metaData.GetSpaceWidth(), text.GetFontSize(), text.GetMaxLineSize());
		Word currentWord(text.GetFontSize());

		for (auto c : text.GetTextString())
		{
			int ascii = static_cast<int>(c);
			if (ascii == SPACE_ASCII)
			{
				if (!currentLine.AddWord(currentWord))
				{
					lines.push_back(currentLine);
					currentLine = Line(metaData.GetSpaceWidth(), text.GetFontSize(), text.GetMaxLineSize());
					currentLine.AddWord(currentWord);
				}
				currentWord = Word(text.GetFontSize());
				continue;
			}
			const Character* character = metaData.GetCharacter(ascii);
			currentWord.AddCharacter(character);
		}
		CompleteStructure(lines, currentLine, currentWord, text);
		return lines;
	}

	void TextMeshCreator::CompleteStructure(std::vector<Line>& lines, Line& currentLine, const Word& currentWord, const GuiText& text)
	{
		if (!currentLine.AddWord(currentWord))
		{
			lines.push_back(currentLine);
			currentLine = Line(metaData.GetSpaceWidth(), text.GetFontSize(), text.GetMaxLineSize());
			currentLine.AddWord(currentWord);
		}
		lines.push_back(currentLine);
	}

	const TextMeshData& TextMeshCreator::CreateQuadVertices(GuiText& text, const std::vector<Line>& lines)
	{
		text.SetNumberOfLines(lines.size());
		double xCursor = 0;
		double yCursor = 0;
		std::vector<float> vertices;
		std::vector<float> textureCoords;
		for (auto line : lines)
		{
			if (text.IsCentered())
			{
				xCursor = (line.GetMaxLength() - line.GetLineLength()) / 2;
			}
			for (auto word : line.GetWords())
			{
				for (auto character : word.GetCharacters())
				{
					AddVerticesForCharacter(xCursor, yCursor, character, text.GetFontSize(), vertices);
					AddTexCoords(textureCoords, character->GetXTextureCoord(), character->GetYTextureCoord(), character->GetXMaxTextureCoord(), character->GetYMaxTextureCoord());
					xCursor += character->GetXAdvance() * text.GetFontSize();
				}
				xCursor += metaData.GetSpaceWidth() * text.GetFontSize();
			}
			xCursor = 0;
			yCursor += LINE_HEIGHT * text.GetFontSize();
		}
		return TextMeshData(vertices, textureCoords);
	}

	void TextMeshCreator::AddVerticesForCharacter(double xCursor, double yCursor, const Character* character, double fontSize, std::vector<float>& vertices)
	{
		double x = xCursor + (character->GetXOffset() * fontSize);
		double y = yCursor + (character->GetYOffset() * fontSize);
		double xMax = x + (character->GetXSize() * fontSize);
		double yMax = y + (character->GetYSize() * fontSize);
		double xProper = (2 * x) - 1;
		double yProper = (2 * y) + 1;
		double xMaxProper = (2 * xMax) - 1;
		double yMaxProper = (-2 * yMax) + 1;
		AddVertices(vertices, xProper, yProper, xMaxProper, yMaxProper);
	}

	void TextMeshCreator::AddVertices(std::vector<float>& vertices, double x, double y, double maxX, double maxY)
	{
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(x);
		vertices.push_back(maxY);
		vertices.push_back(maxX);
		vertices.push_back(maxY);
		vertices.push_back(maxX);
		vertices.push_back(maxY);
		vertices.push_back(maxX);
		vertices.push_back(y);
		vertices.push_back(x);
		vertices.push_back(y);
	}

	void TextMeshCreator::AddTexCoords(std::vector<float>& texCoords, double x, double y, double maxX, double maxY)
	{
		texCoords.push_back(x);
		texCoords.push_back(y);
		texCoords.push_back(x);
		texCoords.push_back(maxY);
		texCoords.push_back(maxX);
		texCoords.push_back(maxY);
		texCoords.push_back(maxX);
		texCoords.push_back(maxY);
		texCoords.push_back(maxX);
		texCoords.push_back(y);
		texCoords.push_back(x);
		texCoords.push_back(y);
	}
}