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
		m_shader->SetUniform1i(DIFFUSE_MAP,				0);
		m_shader->SetUniform1i(NORMAL_MAP,				1);
		m_shader->SetUniform1i(SPECULAR_MAP,			2);
		m_shader->SetUniform1i(PARALLAX_MAP,			3);
		m_shader->SetUniform1i(EMISSION_MAP,			4);
		m_shader->SetUniform1i(SHADOW_MAP_TAG,			6);
		m_shader->SetUniform1f(SHADOW_DISTANCE_TAG,		SHADOW_DISTANCE);
		m_shader->SetUniform1f(TRANSITION_DISTANCE_TAG,	TRANSITION_DISTANCE);
		m_shader->SetUniform1f(SHADOW_MAP_SIZE_TAG,		SHADOW_MAP_SIZE);
		m_shader->SetUniform1i(PCF_COUNT_TAG,			PCF_COUNT);
		m_shader->Stop();
	}

	void NormalEntity3dRenderer::Clear()
	{
		m_entities.clear();
		m_lights.clear();
	}

	void NormalEntity3dRenderer::Draw(const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		m_shader->Start();
		m_shader->SetUniform3f(FOG_COLOR, *m_fogColor);
		m_shader->SetUniform4f(CLIP_PLANE, clipPlane);

		m_shader->SetUniformMat4(VIEW_MATRIX_TAG, viewMatrix);
		m_shader->SetUniform4f(INVERSE_VIEW_VECTOR_TAG, inverseViewVector);
		m_shader->SetUniformMat4(TO_SHADOW_SPACE_MATRIX_TAG, m_toShadowSpaceMatrix);
		for (const auto& directory : m_entities)
		{
			PrepareEntity(directory.second.front());
			for (const Entity* entity : directory.second)
			{
				m_shader->SetUniform2f(OFFSET_TAG, entity->GetMaterialXOffset(), entity->GetMaterialYOffset());
				m_shader->SetUniformMat4(NORMAL_MATRIX_TAG, entity->GetNormalMatrix());
				m_shader->SetUniformMat4(TRANSFORM_MATRIX_TAG, entity->GetModelMatrix());
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
		light->SetCallback(std::bind(&NormalEntity3dRenderer::UpdateLight, this, light));
		UpdateLight(light);
	}

	void NormalEntity3dRenderer::UpdateLight(const Light* light)
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
			size_t i = 0;
			for (; i < m_lights.size(); i++)
			{
				if (m_lights[i] == light)
				{
					break;
				}
			}

			m_shader->SetUniform3f(Shader::GetPointLightPositionTag(i), m_lights[i]->GetTranslation());
			m_shader->SetUniform3f(Shader::GetPointLightAmbientTag(i), m_lights[i]->GetAmbient());
			m_shader->SetUniform3f(Shader::GetPointLightDiffuseTag(i), m_lights[i]->GetDiffuse());
			m_shader->SetUniform3f(Shader::GetPointLightSpecularTag(i), m_lights[i]->GetSpecular());
			m_shader->SetUniform3f(Shader::GetPointLightAttenuationTag(i), (static_cast<const PointLight*>(m_lights[i]))->GetAttenuation());
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
		
		m_shader->SetUniform1f(NUMBER_OF_ROWS, static_cast<float>(entity->material->GetRowCount()));
		if (entity->material->Transparent)
		{
			glDisable(GL_CULL_FACE);
		}

		if (entity->material != NULL)
		{
			m_shader->SetUniform1f(HEIGHT_SCALE, entity->GetHeightOffset());
			m_shader->SetUniform1i(FAKE_LIGHT, entity->material->FakeLight);
			m_shader->SetUniform1i(NORMAL_MAPPED_FLAG_TAG, entity->IsNormalMapped());
			m_shader->SetUniform1i(PARALLAX_MAPPED_FLAG_TAG, entity->IsParallaxMapped());
			m_shader->SetUniform1i(GLOWING, entity->material->HasGlow());

			m_shader->SetUniform1f(MATERIAL_REFLECTIVITY, entity->material->Reflectivity);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, entity->material->GetId());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, entity->normalMap.id);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, entity->material->GetSpecularId());
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, entity->parallaxMap.id);
			if (entity->material->HasGlow())
			{
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, entity->material->GetEmissionId());
			}
		}
	}
}