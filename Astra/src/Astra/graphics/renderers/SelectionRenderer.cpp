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

	void SelectionRenderer::Draw(float delta, const std::unordered_map<unsigned int, std::vector<const Model*>>& models,
									const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		// Disable drawing to color and depth buffer.
		glColorMask(false, false, false, false);
		glDepthMask(false);
		
		m_entityShader->Start();
		m_entityShader->SetUniformMat4(VIEW_MATRIX_TAG, viewMatrix);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		for (const auto& directory : models)
		{
			const auto& mesh = directory.second.front()->GetMesh();
			glBindVertexArray(mesh->GetVAO());

			for (const auto* model : directory.second)
			{
				PrepareModel(model);
				m_entityShader->SetUniform1f(NUMBER_OF_ROWS, static_cast<float>(model->GetRowCount()));
				m_entityShader->SetUniform2f(OFFSET_TAG, model->GetMaterialXOffset(), model->GetMaterialYOffset());
				m_entityShader->SetUniformMat4(TRANSFORM_MATRIX_TAG, model->GetModelMatrix());
					
				glDrawElements(GL_TRIANGLES, mesh->GetVertexCount(), GL_UNSIGNED_INT, NULL);
			}
			glBindVertexArray(0);
		}
		m_entityShader->Stop();

		glStencilMask(0x00);

	#if ASTRA_DEBUG
		glCheckError();
	#endif 

		// Re-enable drawing to color and depth buffer before drawing outline.
		glColorMask(true, true, true, true);
		glDepthMask(true);

		DrawSelected(models, viewMatrix);
	}

	void SelectionRenderer::PrepareModel(const Model* model)
	{
		const auto& material = model->GetMaterial();
		if (material.GetTransparency())
		{
			glDisable(GL_CULL_FACE);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material.GetTextureId(TextureType::DiffuseMap));
	}

	void SelectionRenderer::DrawSelected(const std::unordered_map<unsigned int, std::vector<const Model*>>& models, 
											const Math::Mat4* viewMatrix)
	{
		m_shader->Start();
		m_shader->SetUniformMat4(VIEW_MATRIX_TAG, viewMatrix);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		for (const auto& directory : models)
		{
			const auto& mesh = directory.second.front()->GetMesh();
			glBindVertexArray(mesh->GetVAO());

			for (const auto* model : directory.second)
			{
				PrepareModel(model);
				m_shader->SetUniform1f(NUMBER_OF_ROWS, static_cast<float>(model->GetRowCount()));
				m_shader->SetUniform2f(OFFSET_TAG, model->GetMaterialXOffset(), model->GetMaterialYOffset());
				m_shader->SetUniformMat4(TRANSFORM_MATRIX_TAG, model->GetSelectedModelMatrix());
				glDrawElements(GL_TRIANGLES, mesh->GetVertexCount(), GL_UNSIGNED_INT, NULL);
			}
			glBindVertexArray(0);
		}

		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glStencilMask(0xFF);
		
		m_shader->Stop();
	#if ASTRA_DEBUG
		glCheckError();
	#endif 
	}
}