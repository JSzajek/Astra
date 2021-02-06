#pragma once

#include <string>

#include "Gui.h"
#include "utility/FontAtlas.h"

namespace Astra::Graphics
{
	class TextBox : public Gui
	{
	private:
		std::string m_text;
		unsigned int m_vao, m_vbo;
		const FontAtlas* m_font;
	public:
		TextBox(const char* text, FontAtlas* font, const Math::Vec2& position, float rotation, const Math::Vec2& scale)
			: Gui(NULL, position, rotation, scale), m_font(font), m_vao(0), m_vbo(0)
		{
			SetText(text);
			SetType(GuiType::Textbox);
		}

		inline const bool HasCustomVao() const override { return true; }
		inline const int GetCustomVao() const override { return m_vao; }
		inline const int GetCustomVbo() const override { return m_vbo; }

		inline const FontAtlas* GetFont() const { return m_font; }
		inline const std::string& GetText() const { return m_text; }
		
		inline void SetText(const std::string& string) { GenerateVertices(string); }
	
		inline const Math::Vec2& GetSize() const override { return Math::Vec2::One; }
	private:
		void GenerateVertices(const std::string& string);
	};
}
