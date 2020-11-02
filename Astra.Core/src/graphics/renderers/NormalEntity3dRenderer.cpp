#include "NormalEntity3dRenderer.h"
#include "../../math/Mat4Utils.h"

#include "../entities/PointLight.h"
#include "../entities/SpotLight.h"
#include "../shadows/ShadowMapController.h"
#include <functional>

namespace Astra::Graphics
{
	NormalEntity3dRenderer::NormalEntity3dRenderer(const Math::Vec3* fogColor)
		: Renderer(), m_fogColor(fogColor), m_directionalLight(NULL)
	{
	}

	void NormalEntity3dRenderer::SetShader(Shader* shader)
	{
		Renderer::SetShader(shader);

		m_shader->Start();
		m_shader->SetUniform1i(DIFFUSE_MAP,	0);
		m_shader->SetUniform1i(NORMAL_MAP,  1);
		m_shader->SetUniform1i(SPECULAR_MAP,2);

		/*m_shader->SetUniform1i(Shader::ShadowMapTag,				6);
		m_shader->SetUniform1f(Shader::ShadowDistanceTag,			SHADOW_DISTANCE);
		m_shader->SetUniform1f(Shader::TransitionDistanceTag,		TRANSITION_DISTANCE);
		m_shader->SetUniform1f(Shader::MapSizeTag,					SHADOW_MAP_SIZE);
		m_shader->SetUniform1i(Shader::PcfCountTag,					PCF_COUNT);*/
		m_shader->Stop();
	}

	void NormalEntity3dRenderer::Clear()
	{
		m_entities.clear();
		m_lights.clear();
	}

	void NormalEntity3dRenderer::Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane)
	{
		m_viewMatrix = viewMatrix;
		m_shader->Start();
		m_shader->SetUniform3f(FOG_COLOR, *m_fogColor);
		m_shader->SetUniform4f(CLIP_PLANE, clipPlane);
		
		for (int i = 0; i < m_lights.size(); i++)
		{
			m_shader->SetUniform3f(Shader::GetPointLightPositionTag(i), m_lights[i]->GetTranslation());
			m_shader->SetUniform3f(Shader::GetPointLightAmbientTag(i), m_lights[i]->GetAmbient());
			m_shader->SetUniform3f(Shader::GetPointLightDiffuseTag(i), m_lights[i]->GetDiffuse());
			m_shader->SetUniform3f(Shader::GetPointLightSpecularTag(i), m_lights[i]->GetSpecular());
			m_shader->SetUniform3f(Shader::GetPointLightAttenuationTag(i), (static_cast<const PointLight*>(m_lights[i]))->GetAttenuation());
		}

		m_shader->SetUniform3f(DIR_LIGHT_DIRECTION, m_directionalLight->GetRotation());
		m_shader->SetUniform3f(DIR_LIGHT_AMBIENT, m_directionalLight->GetAmbient());
		m_shader->SetUniform3f(DIR_LIGHT_DIFFUSE, m_directionalLight->GetDiffuse());
		m_shader->SetUniform3f(DIR_LIGHT_SPECULAR, m_directionalLight->GetSpecular());

		m_shader->SetUniformMat4(Shader::ViewMatrixTag, viewMatrix);
		//m_shader->SetUniform4f(Shader::InverseViewVectorTag, viewMatrix.Inverse() * Math::Back4D);
		//m_shader->SetUniformMat4(Shader::ToShadowSpaceMatrixTag, m_toShadowSpaceMatrix);
		for (const auto& directory : m_entities)
		{
			PrepareEntity(directory.second.front());
			for (const Entity* entity : directory.second)
			{
				m_shader->SetUniform2f(OFFSET_TAG, entity->GetMaterialXOffset(), entity->GetMaterialYOffset());
				
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

	void NormalEntity3dRenderer::AddLight(const Light* light)
	{
		m_lights.emplace_back(light);
		/*if (m_lights.size() + 1 > MAX_LIGHTS)
		{
			Logger::Log("Too Many Lights");
			m_lights.pop_back();
		}
		light->SetCallback(std::bind(&NormalEntity3dRenderer::UpdateLights, this));
		UpdateLights();*/
	}

	void NormalEntity3dRenderer::UpdateLights()
	{
		//m_shader->Start();
		//for (int i = 0; i < MAX_LIGHTS; i++)
		//{
		//	if (i < m_lights.size())
		//	{
		//		m_shader->SetUniform3f(NormalEntityShader::GetLightPositionTag(i), NormalEntityShader::ConvertToEyeSpacePosition(m_lights[i]->GetTranslation(), m_viewMatrix));
		//		m_shader->SetUniform3f(NormalEntityShader::GetLightColorTag(i), m_lights[i]->GetColor());
		//		//m_shader->SetUniform3f(NormalEntityShader::GetAttenuationTag(i), m_lights[i]->GetAttenuation());
		//	}
		//	else					   
		//	{
		//		m_shader->SetUniform3f(NormalEntityShader::GetLightPositionTag(i), Math::Vec3(0));
		//		m_shader->SetUniform3f(NormalEntityShader::GetLightColorTag(i), Math::Vec3(0));
		//		//m_shader->SetUniform3f(NormalEntityShader::GetAttenuationTag(i), Math::Vec3(1, 0, 0));
		//	}
		//}
		//m_shader->Stop();
	}

	void NormalEntity3dRenderer::PrepareEntity(const Entity* entity)
	{
		glBindVertexArray(entity->vertexArray->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::TextureCoords));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Normals));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Tangents));

		m_shader->SetUniform1f(NUMBER_OF_ROWS, entity->material->GetRowCount());
		if (entity->material->Transparent)
		{
			glDisable(GL_CULL_FACE);
		}

		if (m_shader->GetType() == ShaderType::NormalMapped)
		{
			//m_shader->SetUniform1f(NormalEntityShader::ShineDampenerTag, entity->material->shineDampener);
		}

		if (entity->material != NULL)
		{
			m_shader->SetUniform1f(FAKE_LIGHT, entity->material->FakeLight);
			m_shader->SetUniform1f(MATERIAL_REFLECTIVITY, entity->material->Reflectivity);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, entity->material->GetId());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, entity->normalMap.id);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, entity->material->GetSpecularId());
		}
	}
}