#pragma once

#include "Renderer.h"
#include "../guis/GuiTexture.h"

namespace Astra::Graphics
{
	struct GuiInfo
	{
		Math::Mat4 transformMatrix;
		float textureIndex;
		//Math::Vec2 textureCoords;
	};

	struct GuiLayer
	{
	private:
		int m_level;
		std::vector<Gui*> m_textures;
		// std::vector<Font>
	
	public:
		GuiLayer(int level) 
			: m_level(level)
		{
		}

		inline int GetLevel() const { return m_level; }
		inline void AddGui(Gui* gui) { m_textures.emplace_back(gui); }
		inline const std::vector<Gui*> GetGuis() const { return m_textures; }
	};

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

	#define NUM_INSTANCES		10

	#define MAX_TEXTURE_SLOTS	16
	#define MAX_GUIS			10

	class GuiRenderer : public Renderer
	{
	private:
		unsigned int m_defaultVAO;
		unsigned int m_defaultVBO;

		std::vector<GuiLayer*> m_layers;

		//std::unordered_map<unsigned int, std::vector<GuiTexture*>> m_guis;
		//std::unordered_map<unsigned int, GuiBuffer> m_buffers;
	public:
		GuiRenderer(Shader* shader);
		
		//void AddGui(GuiTexture* gui);
		void AddGui(Gui* gui, int layer);
		
		//inline void Clear() override { m_guis.clear(); m_buffers.clear(); }
		void Draw(float delta = 0, const Math::Mat4* viewMatrix = NULL, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane) override;
	private:
		void Flush(std::unordered_map<unsigned int, unsigned int>& mapping, size_t& offset);

		void CreateInstancedBuffer(unsigned int* id, size_t size, bool gen);
		unsigned int CreateDefaultQuadVao();
	};
}