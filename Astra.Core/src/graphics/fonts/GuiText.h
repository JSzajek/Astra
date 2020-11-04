#pragma once

#include <string>
#include "../../math/Maths.h"

namespace Astra::Graphics
{
	struct FontType;

	struct GuiText
	{
	public:
		Math::Vec2 Position;
		Math::Vec3 Color;
		Math::Vec3 OutlineColor;
		Math::Vec2 Offset;
	private:
		std::string m_textString;
		unsigned int m_textMeshVao;
		int m_vertexCount;

		float m_fontSize;
		float m_fontWidth;
		float m_fontEdge;
		float m_thickness;
		
		float m_outlineSize;
		float m_outlineWidth;
		float m_outlineEdge;

		float m_lineMaxSize;
		int m_numberOfLines;
		FontType* m_font;
		bool m_centerText;
	public:
		GuiText(const std::string& text, FontType* font, float fontSize, const Math::Vec2& position, float maxLineLength, bool centered);
		GuiText(const std::string& text, FontType* font, float fontSize = 1, const Math::Vec2& position = Math::Vec2::Zero, const Math::Vec3& color = Math::Vec3::Zero, float thickness = 0, float outlineSize = 0, const Math::Vec3& outlineColor = Math::Vec3::Zero,
					const Math::Vec2& offset = Math::Vec2::Zero, float maxLineLength = 1, bool centered = false);
		GuiText(const GuiText& other);

		inline const float FontWidth() const { return m_fontWidth; }
		inline const float FontEdge() const { return m_fontEdge; }

		inline const float OutlineWidth() const { return m_outlineWidth; }
		inline const float OutlineEdge() const { return m_outlineEdge; }

		inline FontType* GetFontType() const { return m_font; }
		inline int GetNumberOfLines() const { return m_numberOfLines; }
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
		void SetMeshInfo(unsigned int vao, int vertexCount);
	};
}