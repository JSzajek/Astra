#pragma once

#include <vector>
#include <string>

#include "MetaFile.h"
#include "GuiText.h"
#include "Line.h"

namespace Astra::Graphics
{
	#define SPACE_ASCII		32
	#define LINE_HEIGHT		0.03

	struct TextMeshData
	{
	public:
		std::vector<float> VertexPositions;
		std::vector<float> TextureCoords;
	public:
		TextMeshData(const std::vector<float>& vertexPositions, const std::vector<float>& textureCoords);
		inline const int GetVertexCount() const { return VertexPositions.size() / 2; }
	};
	
	class TextMeshCreator
	{
	private:
		MetaFile metaData;
	public:
		TextMeshCreator(const char* filepath);
		const TextMeshData* CreateTextMesh(GuiText& text);
	private:
		std::vector<Line> CreateStructure(GuiText& text);
		void CompleteStructure(std::vector<Line>& lines, Line& currentLine, const Word& currentWord, const GuiText& text);
		const TextMeshData* CreateQuadVertices(GuiText& text, const std::vector<Line>& lines);
		void AddVerticesForCharacter(double xCursor, double yCursor, const Character* character, double fontSize, std::vector<float>& vertices);
		void AddVertices(std::vector<float>& vertices, double x, double y, double maxX, double maxY);
		void AddTexCoords(std::vector<float>& texCoords, double x, double y, double maxX, double maxY);
	};

	struct FontType
	{
	private:
		int m_textureAtlas;
		TextMeshCreator* m_loader;
	public:
		FontType(int textureAtlas, const char* filepath);
		FontType(const FontType& other);
		~FontType();

		inline int GetTextureAtlas() const { return m_textureAtlas; }
		
		const TextMeshData* LoadText(GuiText& text);
	};
}