#include "Entity3dRenderer.h"
#include "../../math/Mat4Utils.h"

namespace Astra::Graphics
{
	Entity3dRenderer::Entity3dRenderer(Shader* shader, const Math::Vec3* skyColor)
		: Renderer(shader), m_light(Math::Vec3(0), Math::Vec3(0)), m_skyColor(skyColor)
	{
	}

	void Entity3dRenderer::Draw(const Math::Mat4& viewMatrix)
	{
		m_shader->Start();
		if (m_shader->GetType() == ShaderType::Lighting)
		{
			m_shader->SetUniform3f(LightingShader::SkyColorTag, *m_skyColor);

			m_shader->SetUniform3f(LightingShader::LightPositionTag, m_light.GetTranslation());
			m_shader->SetUniform3f(LightingShader::LightColorTag, m_light.GetColor());
		}
		m_shader->SetUniformMat4(Shader::ViewMatrixTag, viewMatrix);
		for (const auto& directory : m_entities)
		{
			std::vector<const Entity*> entities = directory.second;
			PrepareEntity(*entities.front());
			for (const Entity* entity : entities)
			{
				m_shader->SetUniformMat4(Shader::TransformMatrixTag, Math::Mat4Utils::Transformation(*entity));
				glDrawElements(entity->vertexArray->drawType, entity->vertexArray->vertexCount, GL_UNSIGNED_INT, NULL);
			}
		}
		UnbindVertexArray();
		m_shader->Stop();
 	}

	void Entity3dRenderer::PrepareEntity(const Entity& entity)
	{
		glBindVertexArray(entity.vertexArray->vaoId);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		if (entity.material->transparent)
		{
			glDisable(GL_CULL_FACE);
		}

		if (m_shader->GetType() == ShaderType::Lighting)
		{
			m_shader->SetUniform1f(LightingShader::UseFakeLightingTag, entity.material->fakeLight);
			m_shader->SetUniform1f(LightingShader::ShineDampenerTag, entity.material->shineDampener);
			m_shader->SetUniform1f(LightingShader::ReflectivityTag, entity.material->reflectivity);
		}
		
		if (entity.material != NULL)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, entity.material->id);
		}
	}
}