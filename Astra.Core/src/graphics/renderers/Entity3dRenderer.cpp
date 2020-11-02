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
		: Renderer(), m_fogColor(fogColor), m_directionalLight(NULL)
	{
	}

	void Entity3dRenderer::SetShader(Shader* shader)
	{
		Renderer::SetShader(shader);

		m_shader->Start();
		m_shader->SetUniform1i(DIFFUSE_MAP, 0);
		m_shader->SetUniform1i(SPECULAR_MAP, 1);
		m_shader->SetUniform1i(Shader::ShadowMapTag,				6);
		m_shader->SetUniform1f(Shader::ShadowDistanceTag,			SHADOW_DISTANCE);
		m_shader->SetUniform1f(Shader::TransitionDistanceTag,		TRANSITION_DISTANCE);
		m_shader->SetUniform1f(Shader::MapSizeTag,					SHADOW_MAP_SIZE);
		m_shader->SetUniform1i(Shader::PcfCountTag,					PCF_COUNT);
		m_shader->Stop();
	}

	void Entity3dRenderer::Clear()
	{
		m_lights.clear();
		m_entities.clear();
	}

	void Entity3dRenderer::Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane)
	{
		m_shader->Start();
		m_shader->SetUniform3f(FOG_COLOR, *m_fogColor);
		m_shader->SetUniform4f(CLIP_PLANE, clipPlane);

		m_shader->SetUniformMat4(Shader::ViewMatrixTag, viewMatrix);
		m_shader->SetUniform4f(Shader::InverseViewVectorTag, viewMatrix.Inverse() * Math::Back4D);
		m_shader->SetUniformMat4(Shader::ToShadowSpaceMatrixTag, m_toShadowSpaceMatrix);
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
		switch (light->GetType())
		{
			case LightType::Directional:
				m_directionalLight = light;
				break;
			case LightType::Point:
				m_lights.emplace_back(light);
				break;
			case LightType::Spotlight:
				break;
		}
		light->SetCallback(std::bind(&Entity3dRenderer::UpdateLight, this, light));
		UpdateLight(light);
	}

	void Entity3dRenderer::UpdateLight(const Light* light)
	{
		m_shader->Start();
		if (light->GetType() == LightType::Directional)
		{
			m_shader->SetUniform3f(DIR_LIGHT_DIRECTION, m_directionalLight->GetRotation());
			m_shader->SetUniform3f(DIR_LIGHT_AMBIENT, m_directionalLight->GetAmbient());
			m_shader->SetUniform3f(DIR_LIGHT_DIFFUSE, m_directionalLight->GetDiffuse());
			m_shader->SetUniform3f(DIR_LIGHT_SPECULAR, m_directionalLight->GetSpecular());
		}

		if (light->GetType() == LightType::Point)
		{
			int i = 0;
			for (; i < m_lights.size(); i++)
			{
				if (m_lights[i] == light) { break; }
			}

			m_shader->SetUniform3f(Shader::GetPointLightPositionTag(i), m_lights[i]->GetTranslation());
			m_shader->SetUniform3f(Shader::GetPointLightAmbientTag(i), m_lights[i]->GetAmbient());
			m_shader->SetUniform3f(Shader::GetPointLightDiffuseTag(i), m_lights[i]->GetDiffuse());
			m_shader->SetUniform3f(Shader::GetPointLightSpecularTag(i), m_lights[i]->GetSpecular());
			m_shader->SetUniform3f(Shader::GetPointLightAttenuationTag(i), (static_cast<const PointLight*>(m_lights[i]))->GetAttenuation());
		}
		m_shader->Stop();
	}

	void Entity3dRenderer::PrepareEntity(const Entity* entity)
	{
		glBindVertexArray(entity->vertexArray->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::TextureCoords));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Normals));

		m_shader->SetUniform1f(NUMBER_OF_ROWS, entity->material->GetRowCount());

		if (entity->material->Transparent)
		{
			glDisable(GL_CULL_FACE);
		}

		if (entity->material != NULL)
		{
			m_shader->SetUniform1f(FAKE_LIGHT, entity->material->FakeLight);
			m_shader->SetUniform1f(MATERIAL_REFLECTIVITY, entity->material->Reflectivity);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, entity->material->GetId());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, entity->material->GetSpecularId());
		}
	}
}