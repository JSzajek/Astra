#include "astra_pch.h"

#include "SelectionRenderer.h"

#include "Astra/math/Mat4Utils.h"

#include "Astra/graphics/shaders/BasicEntityShader.h"
#include "Astra/graphics/shaders/SelectionShader.h"

namespace Astra::Graphics
{
	const Color SelectionRenderer::DefaultColor = Color(0.5f, 0.25f, 0, 1.0f);

	SelectionRenderer::SelectionRenderer()
		: m_entityShader(new BasicEntityShader())
	{
		SetShader(new SelectionShader());
	}

	void SelectionRenderer::SetShader(Shader* shader)
	{
		Renderer::SetShader(shader);

		m_entityShader->Start();
		m_entityShader->SetUniform1i(DIFFUSE_MAP, 0);
		m_entityShader->Stop();

		SetSelectionColor(DefaultColor);
	}

	void SelectionRenderer::UpdateProjectionMatrix(const Math::Mat4* projectionMatrix)
	{
		m_shader->Start();
		m_shader->SetUniformMat4(PROJECTION_MATRIX_TAG, projectionMatrix);
		m_shader->Stop();

		m_entityShader->Start();
		m_entityShader->SetUniformMat4(PROJECTION_MATRIX_TAG, projectionMatrix);
		m_entityShader->Stop();
	}

	void SelectionRenderer::SetSelectionColor(const Color& color)
	{
		m_shader->Start();
		m_shader->SetUniform3f(SELECTION_COLOR_TAG, color);
		m_shader->Stop();
	}

	void SelectionRenderer::Draw(float delta, const std::unordered_map<unsigned int, std::vector<const Entity*>>& entities,
		const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector,
		const Math::Vec4& clipPlane)
	{
		// Disable drawing to color and depth buffer.
		glColorMask(false, false, false, false);
		glDepthMask(false);
		
		m_entityShader->Start();

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		m_entityShader->SetUniformMat4(VIEW_MATRIX_TAG, viewMatrix);
		for (const auto& directory : entities)
		{
			PrepareEntity(m_entityShader, directory.second.front());
			for (const Entity* entity : directory.second)
			{
				m_entityShader->SetUniform2f(OFFSET_TAG, entity->GetMaterialXOffset(), entity->GetMaterialYOffset());
				m_entityShader->SetUniformMat4(TRANSFORM_MATRIX_TAG, entity->GetModelMatrix());
				glDrawElements(entity->vertexArray->drawType, entity->vertexArray->vertexCount, GL_UNSIGNED_INT, NULL);
			}
			UnbindVertexArray();
		}
		m_entityShader->Stop();

		glStencilMask(0x00);

	#if ASTRA_DEBUG
		glCheckError();
	#endif 

		// Re-enable drawing to color and depth buffer before drawing outline.
		glColorMask(true, true, true, true);
		glDepthMask(true);
		DrawSelected(entities, viewMatrix);
	}

	void SelectionRenderer::PrepareEntity(Shader* shader, const Entity* entity)
	{
		glBindVertexArray(entity->vertexArray->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::TextureCoords));

		const auto* material = entity->material;

		if (material != NULL)
		{
			if (material->Transparent)
			{
				glDisable(GL_CULL_FACE);
			}
			m_entityShader->SetUniform1f(NUMBER_OF_ROWS, static_cast<float>(material->GetRowCount()));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->GetId());
		}
	}

	void SelectionRenderer::DrawSelected(const std::unordered_map<unsigned int, std::vector<const Entity*>>& entities, const Math::Mat4* viewMatrix)
	{
		m_shader->Start();
		m_shader->SetUniformMat4(VIEW_MATRIX_TAG, viewMatrix);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		for (const auto& directory : entities)
		{
			const Entity* first = directory.second.front();
			const auto* material = first->material;

			glBindVertexArray(first->vertexArray->vaoId);
			glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
			glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::TextureCoords));

			if (material != NULL)
			{
				if (material->Transparent)
				{
					glDisable(GL_CULL_FACE);
				}
				m_shader->SetUniform1f(NUMBER_OF_ROWS, static_cast<float>(material->GetRowCount()));

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, material->GetId());
			}

			for (const Entity* entity : directory.second)
			{
				m_shader->SetUniform2f(OFFSET_TAG, entity->GetMaterialXOffset(), entity->GetMaterialYOffset());
				m_shader->SetUniformMat4(TRANSFORM_MATRIX_TAG, entity->GetSelectedModelMatrix());
				glDrawElements(entity->vertexArray->drawType, entity->vertexArray->vertexCount, GL_UNSIGNED_INT, NULL);
			}
		}

		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glStencilMask(0xFF);
		
		UnbindVertexArray();
		m_shader->Stop();
	#if ASTRA_DEBUG
		glCheckError();
	#endif 
	}
}