#include "NormalEntity3dRenderer.h"
#include "../../math/Mat4Utils.h"

#include "../shadows/ShadowMapController.h"
#include <functional>

namespace Astra::Graphics
{
	NormalEntity3dRenderer::NormalEntity3dRenderer(Shader* shader, const Math::Vec3* fogColor)
		: Renderer(shader), m_skyColor(fogColor)
	{
		m_shader->Start();
		m_shader->SetUniform1i(NormalEntityShader::ModelTextureTag,	0);
		m_shader->SetUniform1i(NormalEntityShader::NormalMapTag,	1);
		m_shader->SetUniform1i(Shader::ShadowMapTag,				5);
		m_shader->SetUniform1f(Shader::ShadowDistanceTag,			SHADOW_DISTANCE);
		m_shader->SetUniform1f(Shader::TransitionDistanceTag,		TRANSITION_DISTANCE);
		m_shader->SetUniform1f(Shader::MapSizeTag,					SHADOW_MAP_SIZE);
		m_shader->SetUniform1i(Shader::PcfCountTag,					PCF_COUNT);
		m_shader->Stop();
	}

	void NormalEntity3dRenderer::Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane)
	{
		m_viewMatrix = viewMatrix;
		m_shader->Start();
		if (m_shader->GetType() == ShaderType::NormalMapped)
		{
			m_shader->SetUniform4f(NormalEntityShader::ClipPaneTag, clipPlane);
			m_shader->SetUniform3f(NormalEntityShader::SkyColorTag, *m_skyColor);
		}
		m_shader->SetUniformMat4(Shader::ViewMatrixTag, viewMatrix);
		m_shader->SetUniformMat4(Shader::ToShadowSpaceMatrixTag, m_toShadowSpaceMatrix);
		for (const auto& directory : m_entities)
		{
			PrepareEntity(directory.second.front());
			for (const Entity* entity : directory.second)
			{
				m_shader->SetUniform2f(NormalEntityShader::OffsetTag, entity->GetMaterialXOffset(), entity->GetMaterialYOffset());

				m_shader->SetUniformMat4(Shader::TransformMatrixTag, Math::Mat4Utils::Transformation(*entity));
				glDrawElements(entity->vertexArray->drawType, entity->vertexArray->vertexCount, GL_UNSIGNED_INT, NULL);
			}
		}
		UnbindVertexArray();
		m_shader->Stop();
	}

	void NormalEntity3dRenderer::AddEntity(const Entity* entity)
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

	void NormalEntity3dRenderer::AddLight(Light* light)
	{
		if (m_lights.size() + 1 > MAX_LIGHTS)
		{
			Logger::Log("Too Many Lights");
			m_lights.pop_back();
		}
		m_lights.emplace_back(light);
		light->SetCallback(std::bind(&NormalEntity3dRenderer::UpdateLights, this));
		UpdateLights();
	}

	void NormalEntity3dRenderer::UpdateLights()
	{
		m_shader->Start();
		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			if (i < m_lights.size())
			{
				m_shader->SetUniform3f(NormalEntityShader::GetLightPositionTag(i), NormalEntityShader::ConvertToEyeSpacePosition(m_lights[i]->GetTranslation(), m_viewMatrix));
				m_shader->SetUniform3f(NormalEntityShader::GetLightColorTag(i), m_lights[i]->GetColor());
				//m_shader->SetUniform3f(NormalEntityShader::GetAttenuationTag(i), m_lights[i]->GetAttenuation());
			}
			else					   
			{
				m_shader->SetUniform3f(NormalEntityShader::GetLightPositionTag(i), Math::Vec3(0));
				m_shader->SetUniform3f(NormalEntityShader::GetLightColorTag(i), Math::Vec3(0));
				//m_shader->SetUniform3f(NormalEntityShader::GetAttenuationTag(i), Math::Vec3(1, 0, 0));
			}
		}
		m_shader->Stop();
	}

	void NormalEntity3dRenderer::PrepareEntity(const Entity* entity)
	{
		glBindVertexArray(entity->vertexArray->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::TextureCoords));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Normals));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Tangents));

		m_shader->SetUniform1f(NormalEntityShader::NumberOfRowsTag, entity->material->GetRowCount());
		//if (entity->material->transparent)
		{
			glDisable(GL_CULL_FACE);
		}

		if (m_shader->GetType() == ShaderType::NormalMapped)
		{
			//m_shader->SetUniform1f(NormalEntityShader::UseFakeLightingTag, entity->material->fakeLight);
			//m_shader->SetUniform1f(NormalEntityShader::ShineDampenerTag, entity->material->shineDampener);
			//m_shader->SetUniform1f(NormalEntityShader::ReflectivityTag, entity->material->reflectivity);
		}

		if (entity->material != NULL)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, entity->material->GetId());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, entity->normalMap.id);
		}
	}
}