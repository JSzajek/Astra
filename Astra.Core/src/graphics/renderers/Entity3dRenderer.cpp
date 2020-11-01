#include "Entity3dRenderer.h"
#include "../../math/Mat4Utils.h"
#include "../shaders/EntityShader.h"

#include "../entities/PointLight.h"
#include "../entities/SpotLight.h"
#include "../shadows/ShadowMapController.h"
#include <functional>

namespace Astra::Graphics
{
	Entity3dRenderer::Entity3dRenderer(const Math::Vec3* fogColor)
		: Renderer(), m_skyColor(fogColor), m_light(NULL)
	{
	}

	void Entity3dRenderer::SetShader(Shader* shader)
	{
		Renderer::SetShader(shader);

		m_shader->Start();
		m_shader->SetUniform1i(DIFFUSE_MAP, 0);
		m_shader->SetUniform1i(SPECULAR_MAP, 1);
		//m_shader->SetUniform1i(LightingShader::ModelTextureTag,		0);
		//m_shader->SetUniform1i(Shader::ShadowMapTag,				5);
		//m_shader->SetUniform1f(Shader::ShadowDistanceTag,			SHADOW_DISTANCE);
		//m_shader->SetUniform1f(Shader::TransitionDistanceTag,		TRANSITION_DISTANCE);
		//m_shader->SetUniform1f(Shader::MapSizeTag,					SHADOW_MAP_SIZE);
		//m_shader->SetUniform1i(Shader::PcfCountTag,					PCF_COUNT);
		m_shader->Stop();
	}

	void Entity3dRenderer::Clear()
	{
		m_light = NULL;
		m_lights.clear();
		m_entities.clear();
	}

	void Entity3dRenderer::Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane)
	{
		m_shader->Start();
		/*if (m_shader->GetType() == ShaderType::Lighting)
		{
			m_shader->SetUniform4f(LightingShader::ClipPaneTag, clipPlane);
			m_shader->SetUniform3f(LightingShader::SkyColorTag, *m_skyColor);
		}*/

		/*if (m_light != NULL)
		{
			m_shader->SetUniform4f(LIGHT_VECTOR, m_light->GetTranslation(), !m_light->IsDirectional());
			if (m_light->GetType() == LightType::Directional)
			{
				m_shader->SetUniform3f(LIGHT_DIRECTION, m_light->GetRotation());
			}
			else
			{
				if (m_light->GetType() == LightType::Point)
				{
					m_shader->SetUniform3f(LIGHT_ATTENUATION, (static_cast<const PointLight*>(m_light))->GetAttenuation());
				}
				else if (m_light->GetType() == LightType::Spotlight)
				{
					m_shader->SetUniform3f(LIGHT_DIRECTION, m_light->GetRotation());
					m_shader->SetUniform1f(LIGHT_CUTOFF, (static_cast<const SpotLight*>(m_light))->GetCutOff());
					m_shader->SetUniform1f(LIGHT_OUTER_CUTOFF, (static_cast<const SpotLight*>(m_light))->GetOuterCutOff());
				}
				else
				{
					m_shader->SetUniform3f(LIGHT_ATTENUATION, Math::Zero);
				}
			}
			m_shader->SetUniform3f(LIGHT_AMBIENT, m_light->GetAmbient());
			m_shader->SetUniform3f(LIGHT_DIFFUSE, m_light->GetDiffuse());
			m_shader->SetUniform3f(LIGHT_SPECULAR, m_light->GetSpecular());
		}*/

		for (int i = 0; i < m_lights.size(); i++)
		{
			m_shader->SetUniform3f(EntityShader::GetPointLightPositionTag(i), m_lights[i]->GetTranslation());
			m_shader->SetUniform3f(EntityShader::GetPointLightAmbientTag(i), m_lights[i]->GetAmbient());
			m_shader->SetUniform3f(EntityShader::GetPointLightDiffuseTag(i), m_lights[i]->GetDiffuse());
			m_shader->SetUniform3f(EntityShader::GetPointLightSpecularTag(i), m_lights[i]->GetSpecular());
			m_shader->SetUniform3f(EntityShader::GetPointLightAttenuationTag(i), (static_cast<const PointLight*>(m_lights[i]))->GetAttenuation());
		}

		m_shader->SetUniform3f(DIR_LIGHT_DIRECTION, m_directionalLight->GetRotation());
		m_shader->SetUniform3f(DIR_LIGHT_AMBIENT, m_directionalLight->GetAmbient());
		m_shader->SetUniform3f(DIR_LIGHT_DIFFUSE, m_directionalLight->GetDiffuse());
		m_shader->SetUniform3f(DIR_LIGHT_SPECULAR, m_directionalLight->GetSpecular());


		m_shader->SetUniformMat4(Shader::ViewMatrixTag, viewMatrix);
		m_shader->SetUniform4f(Shader::InverseViewVectorTag, viewMatrix.Inverse() * Math::Back4D);
		//m_shader->SetUniformMat4(Shader::ToShadowSpaceMatrixTag, m_toShadowSpaceMatrix);
		for (const auto& directory : m_entities)
		{
			PrepareEntity(directory.second.front());
			for (const Entity* entity : directory.second)
			{
				//m_shader->SetUniform2f(LightingShader::OffsetTag, entity->GetMaterialXOffset(), entity->GetMaterialYOffset());

				Math::Mat4 model_matrix = Math::Mat4Utils::Transformation(*entity);
				Math::Mat4 normal_matrix = model_matrix.Inverse();
				normal_matrix.Transpose();
				m_shader->SetUniformMat4(NORMAL_MATRIX, normal_matrix);
				m_shader->SetUniformMat4(Shader::TransformMatrixTag, model_matrix);
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

	void Entity3dRenderer::AddLight(const Light* light)
	{
		m_lights.emplace_back(light);
		/*if (m_lights.size() + 1 > MAX_LIGHTS)
		{
			Logger::Log("Too Many Lights");
			m_lights.pop_back();
		}
		m_lights.emplace_back(light);
		light->SetCallback(std::bind(&Entity3dRenderer::UpdateLights, this));
		UpdateLights();*/
	}

	void Entity3dRenderer::UpdateLights()
	{
		/*m_shader->Start();
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
		m_shader->Stop();*/
	}

	void Entity3dRenderer::PrepareEntity(const Entity* entity)
	{
		glBindVertexArray(entity->vertexArray->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::TextureCoords));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Normals));

		//m_shader->SetUniform1f(LightingShader::NumberOfRowsTag, entity->material->GetRowCount());

		if (entity->material->Transparent)
		{
			glDisable(GL_CULL_FACE);
		}

		/*if (m_shader->GetType() == ShaderType::Lighting)
		{
			m_shader->SetUniform1f(LightingShader::UseFakeLightingTag, entity->material->fakeLight);
			m_shader->SetUniform1f(LightingShader::ShineDampenerTag, entity->material->shineDampener);
			m_shader->SetUniform1f(LightingShader::ReflectivityTag, entity->material->reflectivity);
		}*/

		if (entity->material != NULL)
		{
			//m_shader->SetUniform3f(MATERIAL_AMBIENT, entity->material->Ambient);
			//m_shader->SetUniform3f(MATERIAL_DIFFUSE, entity->material->Diffuse);
			//m_shader->SetUniform3f(MATERIAL_SPECULAR, entity->material->Specular);
			m_shader->SetUniform1f(MATERIAL_REFLECTIVITY, entity->material->Reflectivity);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, entity->material->GetId());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, entity->material->GetSpecularId());
		}
	}
}