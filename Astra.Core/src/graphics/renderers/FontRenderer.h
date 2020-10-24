#pragma once

#include "Renderer.h"
#include "../fonts/GuiText.h"
#include "../fonts/TextMesh.h"
#include "../shaders/FontShader.h"

namespace Astra::Graphics
{
	class FontRenderer : public Renderer
	{
	private:
		std::unordered_map<FontType*, std::vector<GuiText>> m_texts;
	public:
		FontRenderer(FontShader* shader);
		void Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane = DefaultClipPlane) override;
		
		inline std::unordered_map<FontType*, std::vector<GuiText>>& GetTexts() { return m_texts; }
	private:
		void RenderText(const GuiText& text);
	};
}