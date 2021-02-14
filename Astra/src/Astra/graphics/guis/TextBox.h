#pragma once

#include <string>

#include "Gui.h"
#include "utility/FontAtlas.h"

namespace Astra::Graphics
{
	#define DEFAULT_FONT_PATH		"../Astra/res/fonts/calibri.ttf"
	#define DEFAULT_FONT_SIZE		24

	class TextBox : public Gui
	{
	private:
		std::string m_text;
		unsigned int m_vao, m_vbo;
		const FontAtlas* m_font;

		// TODO: Implement bolding and outlining
		//uniform float width;
		//uniform float edge;
		//uniform vec3 outlineColor;
		//uniform float outlineWidth;
		//uniform float outlineEdge;
	public:
		TextBox(const char* text, const FontAtlas* font, const Math::Vec2& position, float rotation, const Math::Vec2& scale);
		TextBox(const char* text, const Math::Vec2& position, float rotation, const Math::Vec2& scale);
		~TextBox();

		inline const bool HasCustomVao() const override { return true; }
		inline const int GetCustomVao() const override { return m_vao; }
		inline const int GetCustomVbo() const override { return m_vbo; }

		inline unsigned int GetVertexCount() const { return m_text.size() * 6; }

		inline const FontAtlas* GetFont() const { return m_font; }
		inline const std::string& GetText() const { return m_text; }
		
		inline void SetText(const std::string& string) { GenerateVertices(string); }
	
		inline const Math::Vec2& GetSize() const override { return Math::Vec2::One; }
	private:
		void GenerateVertices(const std::string& string);
	};
}