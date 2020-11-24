#include "GuiRenderer.h"
#include "../../math/Mat4Utils.h"
#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	GuiRenderer::GuiRenderer(Shader* shader) : Renderer()
	{
		Renderer::SetShader(shader);
	}

	void GuiRenderer::AddGui(GuiTexture* gui)
	{
		auto temp = m_guis.find(gui->GetId());
		if (temp != m_guis.end())
		{
			const auto& buff = m_buffers[gui->GetId()];
			if (temp->second.size() + 1 > buff.MaxSize)
			{
				// Resize buffer
				size_t newSize = static_cast<size_t>(buff.MaxSize * 1.5f);
				unsigned int newVbo = CreateInstancedBuffer(newSize * sizeof(Math::Mat4));

				// Copy buffer data
				glBindBuffer(GL_ARRAY_BUFFER, buff.VBO);
				glBindBuffer(GL_COPY_WRITE_BUFFER, newVbo);
				glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, buff.MaxSize * sizeof(Math::Mat4));
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
				glDeleteBuffers(1, &buff.VBO);
				m_buffers[gui->GetId()].VBO = newVbo;
				m_buffers[gui->GetId()].MaxSize = newSize;
			}
			gui->SetOffset(temp->second.size());
			temp->second.emplace_back(gui);
		}
		else
		{
			m_guis[gui->GetId()] = std::vector<GuiTexture*>();
			gui->SetOffset(0);

			// Create Default Quad Vao and Instanced Buffer
			unsigned int quadVao = CreateDefaultQuadVao();
			unsigned int instanceVbo = CreateInstancedBuffer(NUM_INSTANCES * sizeof(Math::Mat4));
			glBindVertexArray(0);
			m_buffers[gui->GetId()] = GuiBuffer(quadVao, instanceVbo, NUM_INSTANCES);
			m_guis[gui->GetId()].emplace_back(gui);
		}
		
		// Place into buffer
		GuiBuffer buff = m_buffers[gui->GetId()];
		glBindBuffer(GL_ARRAY_BUFFER, buff.VBO);
		void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		
		Math::Mat4 transform = Math::Mat4Utils::Transformation(*gui);
		memcpy(static_cast<void*>(static_cast<char*>(ptr) + (gui->GetOffset() * 16 * sizeof(float))), transform.data, 16 * sizeof(float));
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GuiRenderer::Draw(float delta, const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		if (m_guis.size() == 0) { return; }
		m_shader->Start();
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		for (const auto& directory : m_buffers)
		{
			auto buff = directory.second;
			glBindVertexArray(buff.VAO);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, directory.first);

			glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_guis[directory.first].size());
		}
		
		UnbindVertexArray();
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
		return quadVao;
	}

	unsigned int GuiRenderer::CreateInstancedBuffer(size_t size)
	{
		unsigned int newVbo;
		glGenBuffers(1, &newVbo);
		glBindBuffer(GL_ARRAY_BUFFER, newVbo);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * 4 * sizeof(float), (void*)(1 * 4 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * 4 * sizeof(float), (void*)(2 * 4 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * 4 * sizeof(float), (void*)(3 * 4 * sizeof(float)));

		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return newVbo;
	}
}