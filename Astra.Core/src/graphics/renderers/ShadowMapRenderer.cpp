#include "ShadowMapRenderer.h"

#include "../../math/Mat4Utils.h"

namespace Astra::Graphics
{
	ShadowMapRenderer::ShadowMapRenderer(ShadowShader* shader, ShadowFrameBuffer* buffer, const Math::Mat4& projectionViewMatrix)
		: Renderer((Shader*) shader), m_buffer(buffer), projectionViewMatrix(projectionViewMatrix)
	{
	}

	void ShadowMapRenderer::Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane)
	{
		BindFrameBuffer(m_buffer->GetBuffer().GetId(), m_buffer->GetWidth(), m_buffer->GetHeight());
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);

		m_shader->Start();
		for (const auto& directory : m_entities)
		{
			glBindVertexArray(directory.second.front()->vertexArray->vaoId);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			m_shader->SetUniform1f(ShadowShader::NumberOfRowsTag, directory.second.front()->material->GetRowCount());
			if (directory.second.front()->material->transparent)
			{
				glDisable(GL_CULL_FACE);
			}
			if (directory.second.front()->material != NULL)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, directory.second.front()->material->id);
			}

			for (const Entity* entity : directory.second)
			{
				PrepareEntity(entity);

				glDrawElements(entity->vertexArray->drawType, entity->vertexArray->vertexCount, GL_UNSIGNED_INT, NULL);
			}
		}
		UnbindVertexArray();
		UnbindFrameBuffer();
		m_shader->Stop();
	}

	void ShadowMapRenderer::AddEntity(const Entity* entity)
	{
		auto temp = m_entities.find(entity->vertexArray->vaoId);
		if (temp != m_entities.end())
		{
			temp->second.emplace_back(entity);
		}
		else
		{
			m_entities[entity->vertexArray->vaoId] = std::vector<const Entity*>();
			m_entities[entity->vertexArray->vaoId].emplace_back(entity);
		}
	}

	void ShadowMapRenderer::PrepareEntity(const Entity* entity)
	{
		m_shader->SetUniform2f(ShadowShader::OffsetTag, entity->GetMaterialXOffset(), entity->GetMaterialYOffset());

		Math::Mat4 transformMatrix = Math::Mat4Utils::Transformation(*entity);
		m_shader->SetUniformMat4(ShadowShader::ModelViewProjMatrixTag, projectionViewMatrix * transformMatrix);
	}
}