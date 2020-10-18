#pragma once

#include <vector>
#include <string>

#include "MetaFile.h"
#include "Line.h"

namespace Astra::Graphics
{
	#define SPACE_ASCII		32
	#define LINE_HEIGHT		0.03

	struct GuiText;
	struct TextMeshData;

	class TextMeshCreator
	{
	private:
		MetaFile metaData;
	public:
		TextMeshCreator(const char* filepath);
		const TextMeshData& CreateTextMesh(GuiText& text);
	private:
		std::vector<Line> CreateStructure(GuiText& text);
		void CompleteStructure(std::vector<Line>& lines, Line& currentLine, const Word& currentWord, const GuiText& text);
		const TextMeshData& CreateQuadVertices(GuiText& text, const std::vector<Line>& lines);
		void AddVerticesForCharacter(double xCursor, double yCursor, const Character* character, double fontSize, std::vector<float>& vertices);
		void AddVertices(std::vector<float>& vertices, double x, double y, double maxX, double maxY);
		void AddTexCoords(std::vector<float>& texCoords, double x, double y, double maxX, double maxY);
	};

	struct TextMeshData
	{
	public:
		std::vector<float> VertexPositions;
		std::vector<float> TextureCoords;
	public:
		TextMeshData(const std::vector<float>& vertexPositions, const std::vector<float>& textureCoords);
		TextMeshData(const TextMeshData& other);
		inline const int GetVertexCount() const { return VertexPositions.size() / 2; }
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
		const TextMeshData& LoadText(GuiText& text) { return m_loader->CreateTextMesh(text); }
	};

	struct GuiText
	{
	private:
		std::string m_textString;
		float m_fontSize;
		unsigned int m_textMeshVao;
		int m_vertexCount;

		Math::Vec3 m_color;
		Math::Vec2 m_position;
		float m_lineMaxSize;
		int m_numberOfLines;
		FontType m_font;
		bool m_centerText;
	public:
		GuiText(const std::string& text, float fontSize, const FontType& font, const Math::Vec2& position, float maxLineLength, bool centered);
		GuiText(const GuiText& other);

		inline const FontType& GetFontType() { return m_font; }
		inline const Math::Vec3& GetColor() const { return m_color; }
		inline int GetNumberOfLines() const { return m_numberOfLines; }
		inline const Math::Vec2& GetPosition() const { return m_position; }
		inline unsigned int GetMesh() const { return m_textMeshVao; }
		inline int GetVertexCount() const { return m_vertexCount; }
		inline float GetFontSize() const { return m_fontSize; }
		inline float GetMaxLineSize() const { return m_lineMaxSize; }
		inline const std::string& GetTextString() const { return m_textString; }

		inline bool IsCentered() const { return m_centerText; }
		inline void SetNumberOfLines(int numberOfLines) { m_numberOfLines = numberOfLines; }

		void RemoveText();
		void SetColor(float r, float g, float b);
		void SetMeshInfo(unsigned int vao, int vertexCount);
	};
}