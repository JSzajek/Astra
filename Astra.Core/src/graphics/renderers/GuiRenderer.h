#pragma once

#include "Renderer.h"
#include "../textures/GuiTexture.h"

namespace Astra::Graphics
{
	class GuiRenderer : public Renderer
	{
	private:
		const VertexArray* m_defaultQuad;
		std::vector<const GuiTexture*> m_guis;
	public:
		GuiRenderer(Shader* shader);
		~GuiRenderer();
		inline void AddGui(const GuiTexture* gui) { m_guis.emplace_back(gui); }
		inline void Clear() override { m_guis.clear(); }
		void Draw(const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane) override;
	};
}