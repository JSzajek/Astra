#include "GuiRenderer.h"
#include "../../math/Mat4Utils.h"
#include "../loaders/Loader.h"

#include "../ResourceManager.h"

#include <algorithm>

namespace Astra::Graphics
{
	GuiRenderer::GuiRenderer(Shader* guiShader, Shader* fontShader)
		: Renderer()
	{
		Renderer::SetShader(guiShader);
		m_fontShader = fontShader;
		
		m_defaultVAO = CreateDefaultQuadVao();
		CreateInstancedBuffer(&m_defaultVBO, MAX_GUIS * sizeof(GuiInfo));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_shader->Start();
		int textureSet[MAX_TEXTURE_SLOTS];
		for (size_t i = 0; i < MAX_TEXTURE_SLOTS; i++) { textureSet[i] = i; }
		m_shader->SetUniform1iv("instanced_Textures", MAX_TEXTURE_SLOTS, textureSet);
		m_shader->Stop();
	}

	GuiRenderer::~GuiRenderer()
	{
		glDeleteVertexArrays(1, &m_defaultVAO);
		glDeleteBuffers(1, &m_defaultVBO);
	}

	void GuiRenderer::UpdateProjectionMatrix(const Math::Mat4* projectionMatrix)
	{
		m_shader->Start();
		m_shader->SetUniformMat4(PROJECTION_MATRIX_TAG, projectionMatrix);
		m_shader->Stop();

		m_fontShader->Start();
		auto ortho = Math::Mat4::Orthographic(0, 960, 540, 0, -1, 1);

		m_fontShader->SetUniformMat4(PROJECTION_MATRIX_TAG, ortho);
		m_fontShader->Stop();
	}

	void GuiLayer::Add(Gui* gui)
	{
		//if (gui->GetType() == GuiType::Label)
		//{
		//	// Add The font to the texts to render
		//	auto text = static_cast<Label*>(gui)->GetText();
		//	FontType* font = text->GetFontType();
		//	const TextMeshData* data = font->LoadText(text);
		//	int vao = Loader::Load(GL_TRIANGLES, data->VertexPositions, data->TextureCoords);
		//	text->SetMeshInfo(vao, data->GetVertexCount());
		//	auto found = m_texts.find(font);
		//	if (found != m_texts.end())
		//	{
		//		found->second.push_back(text);
		//	}
		//	else
		//	{
		//		std::vector<GuiText*> textBatch = std::vector<GuiText*>();
		//		textBatch.push_back(text);
		//		m_texts.insert(std::make_pair(font, textBatch));
		//	}
		//	delete data; // Clean up
		//}
		
		if (gui->GetType() == GuiType::Textbox)
		{
			m_texts.emplace_back(static_cast<TextBox*>(gui));
		}
		else if (gui->HasCustomVao())
		{
			m_customTextures.emplace_back(gui);
		}
		else
		{
			// Add Texture to textures to render
			m_textures.emplace_back(gui);
		}
	}

	void GuiRenderer::AddGui(Gui* gui, int level)
	{
		for (auto& layer : m_layers)
		{
			if (layer->GetLevel() == level)
			{
				layer->Add(gui);
				return;
			}
		}

		GuiLayer* newLayer = new GuiLayer(level);
		newLayer->Add(gui);
		m_layers.emplace_back(newLayer);

		// Insertion Sort layers
		for (auto it = m_layers.begin(); it != m_layers.end(); it++)
		{
			auto const insertion_point = std::upper_bound(m_layers.begin(), it, *it,
				[](const GuiLayer* a, const GuiLayer* b) {return b->GetLevel() > a->GetLevel(); });
			std::rotate(insertion_point, it, it + 1);
		}
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
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, offset);
		mapping.clear();
		offset = 0;
	}

	//void GuiRenderer::RenderText(const GuiText* text)
	//{
	//	glBindVertexArray(text->GetMesh());
	//	glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
	//	glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::TextureCoords));

	//	//m_fontShader->SetUniformMat4("transformMatrix", Math::Mat4Utils::Transformation(text->Position, 0, Math::Vec2(1)));

	//	m_fontShader->SetUniform3f(COLOR_TAG, text->Color);
	//	m_fontShader->SetUniform1f(WIDTH_TAG, text->FontWidth());
	//	m_fontShader->SetUniform1f(EDGE_TAG, text->FontEdge());

	//	m_fontShader->SetUniform3f(OUTLINE_COLOR_TAG, text->OutlineColor);
	//	m_fontShader->SetUniform1f(OUTLINE_WIDTH_TAG, text->OutlineWidth());
	//	m_fontShader->SetUniform1f(OUTLINE_EDGE_TAG, text->OutlineEdge());

	//	m_fontShader->SetUniform2f(TRANSLATION_TAG, text->Position);

	//	glDrawArrays(GL_TRIANGLES, 0, text->GetVertexCount());
	//}

	void GuiRenderer::Draw(float delta, const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		if (m_layers.size() == 0) { return; }

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		std::unordered_map<unsigned int, unsigned int> m_textureMapping;
		size_t offset = 0;
		unsigned int slot;

		for (const auto* layer : m_layers)
		{
			

			// Render Guis
			m_shader->Start();

			glBindVertexArray(m_defaultVAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_defaultVBO);

			for (auto* gui : layer->GetDefaultGuis())
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
				glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(GuiInfo), sizeof(Math::Mat4), transform.data);
				Color color = gui->GetModulate();
				float dat[8] =
				{
					static_cast<float>(slot),
					static_cast<float>(gui->Material->GetRowCount()),
					static_cast<float>(static_cast<Image*>(gui)->GetMaterialXOffset()),
					static_cast<float>(static_cast<Image*>(gui)->GetMaterialYOffset()),
					color.GetR(),
					color.GetG(),
					color.GetB(),
					color.GetA(),
				};
				glBufferSubData(GL_ARRAY_BUFFER, (offset * sizeof(GuiInfo)) + sizeof(Math::Mat4), sizeof(dat), dat);
				offset++;
			}
			if (offset > 0)
			{
				Flush(m_textureMapping, offset);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			// Render all custom VAO guis
			for (auto* gui : layer->GetCustomGuis())
			{
				glBindVertexArray(gui->GetCustomVao());
				glBindBuffer(GL_ARRAY_BUFFER, gui->GetCustomVbo());

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gui->Material->GetId());

				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);
				glBindVertexArray(0);
			}

			m_shader->Stop();

			m_fontShader->Start();

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			for (const auto* text : layer->GetTexts())
			{
				Math::Mat4 transform = Math::Mat4Utils::Transformation(*text);
				m_fontShader->SetUniformMat4("transformMatrix", transform);
				m_fontShader->SetUniform3f("textColor", Math::Vec3(1, 0, 0));

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, text->GetFont()->GetId());

				glBindVertexArray(text->GetCustomVao());

				// render quad
				glDrawArrays(GL_TRIANGLES, 0, text->GetText().size() * 6);
			}
			glDisable(GL_BLEND);
			glBindVertexArray(0);

			m_fontShader->Stop();
		}
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

	#if _DEBUG
		glCheckError();
	#endif
	}

	unsigned int GuiRenderer::CreateDefaultQuadVao()
	{
		//static const std::vector<float> m_defaultVertices = { -1, 1, -1, -1, 1, 1, 1, -1 };
		static float m_defaultVertices[] = 
		{
			// Verts	TexCoords
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		unsigned int quadVao, quadVbo;
		glGenVertexArrays(1, &quadVao);
		glGenBuffers(1, &quadVbo);
		glBindVertexArray(quadVao);
		glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_defaultVertices), m_defaultVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Math::Vec4), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return quadVao;
	}

	void GuiRenderer::CreateInstancedBuffer(unsigned int* id, size_t size)
	{
		glGenBuffers(1, id);
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
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(GuiInfo), (void*)(4 * 4 * sizeof(float)));	// Texture Info
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(GuiInfo), (void*)(5 * 4 * sizeof(float)));	// Modulate Color

		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
	}
}