#pragma once

#include "Renderer.h"
#include "../textures/GuiTexture.h"

namespace Astra::Graphics
{
	struct GuiBuffer
	{
		unsigned int VAO;
		unsigned int VBO;
		size_t MaxSize;

		GuiBuffer() 
			: VAO(0), VBO(0), MaxSize(0)
		{
		}

		GuiBuffer(unsigned int vao, unsigned int vbo, size_t size)
			: VAO(vao), VBO(vbo), MaxSize(size)
		{
		}
	};

	#define NUM_INSTANCES	10

	class GuiRenderer : public Renderer
	{
	private:
		std::unordered_map<unsigned int, std::vector<GuiTexture*>> m_guis;
		std::unordered_map<unsigned int, GuiBuffer> m_buffers;
	public:
		GuiRenderer(Shader* shader);
		void AddGui(GuiTexture* gui);
		inline void Clear() override { m_guis.clear(); m_buffers.clear(); }
		void Draw(float delta = 0, const Math::Mat4* viewMatrix = NULL, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane) override;
	private:
		unsigned int CreateInstancedBuffer(size_t size);
		unsigned int CreateDefaultQuadVao();
	};
}