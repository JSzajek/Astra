#pragma once

#include "Renderer.h"
#include "../guis/Gui.h"
#include "../guis/Image.h"
#include "../guis/Label.h"
#include "../guis/TextBox.h"

#include "../shaders/FontShader.h"
//#include "../fonts/GuiText.h"

namespace Astra::Graphics
{
	struct GuiInfo
	{
		Math::Mat4 transformMatrix;
		Math::Vec4 textureIndex;
		Color modulateColor;
	};

	struct GuiLayer
	{
	private:
		int m_level;
		std::vector<Gui*> m_textures;
		std::vector<Gui*> m_customTextures;
		std::vector<TextBox*> m_texts;

		//std::unordered_map<const FontType*, std::vector<GuiText*>> m_texts;
	public:
		GuiLayer(int level) 
			: m_level(level)
		{
		}

		void Add(Gui* gui);
		
		inline int GetLevel() const { return m_level; }
		inline const std::vector<Gui*> GetDefaultGuis() const { return m_textures; }
		inline const std::vector<Gui*> GetCustomGuis() const { return m_customTextures; }
		inline const std::vector<TextBox*> GetTexts() const { return m_texts; }

		//inline const std::unordered_map<const FontType*, std::vector<GuiText*>> GetTexts() const { return m_texts; }
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
		
		Shader* m_fontShader;
		
		std::vector<GuiLayer*> m_layers;

		//std::unordered_map<unsigned int, std::vector<GuiTexture*>> m_guis;
		//std::unordered_map<unsigned int, GuiBuffer> m_buffers;
	public:
		GuiRenderer(Shader* guiShader, Shader* fontShader);
		~GuiRenderer();

		void UpdateProjectionMatrix(const Math::Mat4* projectionMatrix) override;

		//void AddGui(GuiTexture* gui);
		void AddGui(Gui* gui, int layer);
		
		//inline void Clear() override { m_guis.clear(); m_buffers.clear(); }
		void Draw(float delta = 0, const Math::Mat4* viewMatrix = NULL, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane) override;
	private:
		void Flush(std::unordered_map<unsigned int, unsigned int>& mapping, size_t& offset);
		//void RenderText(const GuiText* text);

		void CreateInstancedBuffer(unsigned int* id, size_t size);
		unsigned int CreateDefaultQuadVao();
	};
}