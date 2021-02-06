#include "TextMesh.h"
#include "../../math/Maths.h"

#include "../ResourceManager.h"

namespace Astra::Graphics
{
	FontType::FontType(const Texture* textureAtlas, const char* filepath)
		: m_textureAtlas(textureAtlas), m_loader(new TextMeshCreator(filepath))
	{
	}

	FontType::~FontType()
	{
		//delete m_loader;
		ResourceManager::Unload(m_textureAtlas);
	}

	const TextMeshData* FontType::LoadText(const GuiText* text) 
	{ 
		return m_loader->CreateTextMesh(text); 
	}

	TextMeshData::TextMeshData(const std::vector<float>& vertexPositions, const std::vector<float>& textureCoords)
		: VertexPositions(vertexPositions), TextureCoords(textureCoords)
	{
	}

	TextMeshCreator::TextMeshCreator(const char* filepath)
		: metaData(filepath)
	{
	}

	const TextMeshData* TextMeshCreator::CreateTextMesh(const GuiText* text)
	{
		std::vector<Line> lines = CreateStructure(text);
		const TextMeshData* data = CreateQuadVertices(text, lines);
		return data;
	}

	std::vector<Line> TextMeshCreator::CreateStructure(const GuiText* text)
	{
		std::vector<Line> lines;
		Line currentLine(metaData.GetSpaceWidth(), text->GetFontSize(), text->GetMaxLineSize());
		Word currentWord(text->GetFontSize());

		for (auto c : text->GetTextString())
		{
			int ascii = static_cast<int>(c);
			if (ascii == SPACE_ASCII)
			{
				if (!currentLine.AddWord(currentWord))
				{
					lines.push_back(currentLine);
					currentLine = Line(metaData.GetSpaceWidth(), text->GetFontSize(), text->GetMaxLineSize());
					currentLine.AddWord(currentWord);
				}
				currentWord = Word(text->GetFontSize());
				continue;
			}
			const Character* character = metaData.GetCharacter(ascii);
			currentWord.AddCharacter(character);
		}
		CompleteStructure(lines, currentLine, currentWord, text);
		return lines;
	}

	void TextMeshCreator::CompleteStructure(std::vector<Line>& lines, Line& currentLine, const Word& currentWord, const GuiText* text)
	{
		if (!currentLine.AddWord(currentWord))
		{
			lines.push_back(currentLine);
			currentLine = Line(metaData.GetSpaceWidth(), text->GetFontSize(), text->GetMaxLineSize());
			currentLine.AddWord(currentWord);
		}
		lines.push_back(currentLine);
	}

	const TextMeshData* TextMeshCreator::CreateQuadVertices(const GuiText* text, const std::vector<Line>& lines)
	{
		float xCursor = 0;
		float yCursor = 0;
		std::vector<float> vertices;
		std::vector<float> textureCoords;
		for (auto line : lines)
		{
			if (text->IsCentered())
			{
				xCursor = (line.GetMaxLength() - line.GetLineLength()) / 2.0f;
			}
			for (auto word : line.GetWords())
			{
				for (auto character : word.GetCharacters())
				{
					AddVerticesForCharacter(xCursor, yCursor, character, text->GetFontSize(), vertices);
					AddTexCoords(textureCoords, character->GetXTextureCoord(), character->GetYTextureCoord(), 
													character->GetXMaxTextureCoord(), character->GetYMaxTextureCoord());
					xCursor += character->GetXAdvance() * text->GetFontSize();
				}
				xCursor += metaData.GetSpaceWidth() * text->GetFontSize();
			}
			xCursor = 0;
			yCursor += LINE_HEIGHT * text->GetFontSize();
		}
		return new TextMeshData(vertices, textureCoords);
	}

	void TextMeshCreator::AddVerticesForCharacter(float xCursor, float yCursor, const Character* character, float fontSize, std::vector<float>& vertices)
	{
		float x = xCursor + (character->GetXOffset() * fontSize);
		float y = yCursor + (character->GetYOffset() * fontSize);
		float xMax = x + (character->GetXSize() * fontSize);
		float yMax = y + (character->GetYSize() * fontSize);
		float xProper = (2 * x) - 1;
		float yProper = (-2 * y) + 1;
		float xMaxProper = (2 * xMax) - 1;
		float yMaxProper = (-2 * yMax) + 1;
		AddVertices(vertices, xProper, yProper, xMaxProper, yMaxProper);
	}

	void TextMeshCreator::AddVertices(std::vector<float>& vertices, float x, float y, float maxX, float maxY)
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

	void TextMeshCreator::AddTexCoords(std::vector<float>& texCoords, float x, float y, float maxX, float maxY)
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