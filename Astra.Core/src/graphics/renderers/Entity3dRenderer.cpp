#include "Entity3dRenderer.h"
#include "../../math/Mat4Utils.h"

#include <functional>

namespace Astra::Graphics
{
	Entity3dRenderer::Entity3dRenderer(Shader* shader, const Math::Vec3* fogColor)
		: Renderer(shader), m_skyColor(fogColor)
	{
	}

	void Entity3dRenderer::Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane)
	{
		m_shader->Start();
		if (m_shader->GetType() == ShaderType::Lighting)
		{
			m_shader->SetUniform4f(LightingShader::ClipPaneTag, clipPlane);
			m_shader->SetUniform3f(LightingShader::SkyColorTag, *m_skyColor);
		}
		m_shader->SetUniformMat4(Shader::ViewMatrixTag, viewMatrix);
		for (const auto& directory : m_entities)
		{
			std::vector<const Entity*> entities = directory.second;
			PrepareEntity(*entities.front());
			for (const Entity* entity : entities)
			{
				m_shader->SetUniform2f(LightingShader::OffsetTag, entity->GetMaterialXOffset(), entity->GetMaterialYOffset());

				m_shader->SetUniformMat4(Shader::TransformMatrixTag, Math::Mat4Utils::Transformation(*entity));
				glDrawElements(entity->vertexArray->drawType, entity->vertexArray->vertexCount, GL_UNSIGNED_INT, NULL);
			}
		}
		UnbindVertexArray();
		m_shader->Stop();
 	}

	void Entity3dRenderer::AddEntity(const Entity* entity)
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

	void Entity3dRenderer::AddLight(Light* light)
	{
		if (m_lights.size() + 1 > MAX_LIGHTS)
		{
			Logger::Log("Too Many Lights");
			m_lights.pop_back();
		}
		m_lights.emplace_back(light);
		light->SetCallback(std::bind(&Entity3dRenderer::UpdateLights, this));
		UpdateLights();
	}

	void Entity3dRenderer::UpdateLights()
	{
		m_shader->Start();
		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			if (i < m_lights.size())
			{
				m_shader->SetUniform3f(LightingShader::GetLightPositionTag(i), m_lights[i]->GetTranslation());
				m_shader->SetUniform3f(LightingShader::GetLightColorTag(i), m_lights[i]->GetColor());
				m_shader->SetUniform3f(LightingShader::GetAttenuationTag(i), m_lights[i]->GetAttenuation());
			}
			else
			{
				m_shader->SetUniform3f(LightingShader::GetLightPositionTag(i), Math::Vec3(0));
				m_shader->SetUniform3f(LightingShader::GetLightColorTag(i), Math::Vec3(0));
				m_shader->SetUniform3f(LightingShader::GetAttenuationTag(i), Math::Vec3(1, 0, 0));
			}
		}
		m_shader->Stop();
	}

	void Entity3dRenderer::PrepareEntity(const Entity& entity)
	{
		glBindVertexArray(entity.vertexArray->vaoId);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		m_shader->SetUniform1f(LightingShader::NumberOfRowsTag, entity.material->GetRowCount());

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