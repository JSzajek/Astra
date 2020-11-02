#pragma once

#include "Renderer.h"
#include "../shaders/FontShader.h"
#include "../fonts/GuiText.h"
#include "../fonts/TextMesh.h"

namespace Astra::Graphics
{
	class FontRenderer : public Renderer
	{
	private:
		std::unordered_map<const FontType*, std::vector<GuiText*>> m_texts;
	public:
		FontRenderer(FontShader* shader);
		void Draw(const Math::Mat4& viewMatrix, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane) override;
		
		inline void Clear() override { m_texts.clear(); }
		inline std::unordered_map<const FontType*, std::vector<GuiText*>>& GetTexts() { return m_texts; }
	private:
		void RenderText(const GuiText* text);
	};
}