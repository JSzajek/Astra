#pragma once

#include "Renderer.h"
#include "../loaders/Loader.h"
#include "../shaders/GuiShader.h"
#include "../textures/GuiTexture.h"

namespace Astra::Graphics
{
	class GuiRenderer : public Renderer
	{
	private:
		const VertexArray* m_defaultQuad;
		std::vector<const GuiTexture*> m_guis;
	public:
		GuiRenderer(GuiShader* shader);
		~GuiRenderer();
		inline void AddGui(const GuiTexture* gui) { m_guis.emplace_back(gui); }
		void Draw(const Math::Mat4& viewMatrix) override;
	};
}