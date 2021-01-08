#include "GuiRenderer.h"
#include "../../math/Mat4Utils.h"
#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	GuiRenderer::GuiRenderer(Shader* shader) 
		: Renderer()
	{
		Renderer::SetShader(shader);
		
		m_defaultVAO = CreateDefaultQuadVao();
		CreateInstancedBuffer(&m_defaultVBO, MAX_GUIS * sizeof(GuiInfo), true);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_shader->Start();
		int textureSet[MAX_TEXTURE_SLOTS];
		for (size_t i = 0; i < MAX_TEXTURE_SLOTS; i++) { textureSet[i] = i; }
		m_shader->SetUniform1iv("instanced_Textures", 16, textureSet);
		m_shader->Stop();
	}

	void GuiRenderer::AddGui(Gui* gui, int level)
	{
		for (auto& layer : m_layers)
		{
			if (layer->GetLevel() == level)
			{
				layer->AddGui(gui);
				return;
			}
		}

		GuiLayer* newLayer = new GuiLayer(level);
		newLayer->AddGui(gui);
		m_layers.emplace_back(newLayer);
	}

	void GuiRenderer::Flush(std::unordered_map<unsigned int, unsigned int>& mapping, size_t& offset)
	{
		size_t i = 0;
		for (const auto& t : mapping)
		{
			// Bind all of the relevent textures involved in this pass
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, t.first);
			i++;
		}

		// Flush and render the current stored particles
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, offset);
		mapping.clear();
		offset = 0;
	}

	void GuiRenderer::Draw(float delta, const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		if (m_layers.size() == 0) { return; }

		m_shader->Start();
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(m_defaultVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_defaultVBO);

		std::unordered_map<unsigned int, unsigned int> m_textureMapping;
		size_t offset = 0;
		unsigned int slot;

		for (const auto* layer : m_layers)
		{
			for (auto* gui : layer->GetGuis())
			{
				if (offset == MAX_GUIS)
				{
					Flush(m_textureMapping, offset);
				}

				const auto& temp = m_textureMapping.find(gui->Material->GetId());
				if (temp == m_textureMapping.end())
				{
					if (m_textureMapping.size() + 1 == MAX_TEXTURE_SLOTS)
					{
						Flush(m_textureMapping, offset);
					}
					slot = m_textureMapping.size();
					m_textureMapping[gui->Material->GetId()] = m_textureMapping.size();
				}
				else
				{
					slot = temp->second;
				}

				// Determine whether subbuffer once or per offset is more efficient
				Math::Mat4 transform = Math::Mat4Utils::Transformation(*gui);
				glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(GuiInfo), 16 * sizeof(float), transform.data);
				float dat[1] =
				{
					static_cast<float>(slot)
				};
				glBufferSubData(GL_ARRAY_BUFFER, (offset * sizeof(GuiInfo)) + (16 * sizeof(float)), 1 * sizeof(float), dat);
				offset++;
			}
			if (offset > 0)
			{
				Flush(m_textureMapping, offset);
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		//UnbindVertexArray();
		m_shader->Stop();
	#if _DEBUG
		glCheckError();
	#endif
	}

	unsigned int GuiRenderer::CreateDefaultQuadVao()
	{
		static const std::vector<float> m_defaultVertices = { -1, 1, -1, -1, 1, 1, 1, -1 };

		unsigned int quadVao, quadVbo;
		glGenVertexArrays(1, &quadVao);
		glGenBuffers(1, &quadVbo);
		glBindVertexArray(quadVao);
		glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_defaultVertices.size(), &m_defaultVertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return quadVao;
	}

	void GuiRenderer::CreateInstancedBuffer(unsigned int* id, size_t size, bool gen)
	{
		if (gen)
		{
			glGenBuffers(1, id);
		}
		glBindBuffer(GL_ARRAY_BUFFER, *id);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GuiInfo), (void*)0);							// Transform Matrix
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GuiInfo), (void*)(1 * 4 * sizeof(float)));	// Transform Matrix
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(GuiInfo), (void*)(2 * 4 * sizeof(float)));	// Transform Matrix
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(GuiInfo), (void*)(3 * 4 * sizeof(float)));	// Transform Matrix
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GuiInfo), (void*)(4 * 4 * sizeof(float)));	// Texture Index

		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
	}
}