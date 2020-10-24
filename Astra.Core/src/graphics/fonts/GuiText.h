#pragma once

#include <string>
#include "../../math/Maths.h"

namespace Astra::Graphics
{
	struct FontType;

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
		FontType* m_font;
		bool m_centerText;
	public:
		GuiText(const std::string& text, float fontSize, FontType* font, const Math::Vec2& position, float maxLineLength, bool centered);
		GuiText(const GuiText& other);
		~GuiText();

		inline FontType* GetFontType() const { return m_font; }
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

		bool operator==(const GuiText& other);
		bool operator!=(const GuiText& other);

		void RemoveText();
		void SetColor(float r, float g, float b);
		void SetMeshInfo(unsigned int vao, int vertexCount);
	};
}