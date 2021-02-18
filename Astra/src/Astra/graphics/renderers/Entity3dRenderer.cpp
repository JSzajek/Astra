#include "astra_pch.h"

#include "Entity3dRenderer.h"

#include "Astra/math/Mat4Utils.h"

#include "Astra/graphics/shaders/EntityShader.h"
#include "Astra/graphics/shaders/SelectionShader.h"

#include "Astra/graphics/entities/PointLight.h"
#include "Astra/graphics/entities/SpotLight.h"
#include "Astra/graphics/shadows/ShadowMapController.h"

namespace Astra::Graphics
{
	Entity3dRenderer::Entity3dRenderer(const Color* fogColor)
		: Renderer(), m_fogColor(fogColor), m_directionalLight(NULL), m_toShadowSpaceMatrix(NULL)
		#if ASTRA_DEBUG
			, m_wireframe(false)
		#endif
	{
	}

	void Entity3dRenderer::SetShader(Shader* shader)
	{
		Renderer::SetShader(shader);

		m_shader->Start();
		m_shader->SetUniform1i(DIFFUSE_MAP,				0);
		m_shader->SetUniform1i(SPECULAR_MAP,			1);
		m_shader->SetUniform1i(EMISSION_MAP,			2);
		m_shader->SetUniform1i(SHADOW_MAP_TAG,			6);
		m_shader->SetUniform1f(SHADOW_DISTANCE_TAG,		SHADOW_DISTANCE);
		m_shader->SetUniform1f(TRANSITION_DISTANCE_TAG,	TRANSITION_DISTANCE);
		m_shader->SetUniform1f(SHADOW_MAP_SIZE_TAG,		SHADOW_MAP_SIZE);
		m_shader->SetUniform1i(PCF_COUNT_TAG,			PCF_COUNT);
		m_shader->Stop();
	}

	void Entity3dRenderer::Draw(float delta, const std::unordered_map<unsigned int, std::vector<const Entity*>>& m_entities, 
								const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		m_shader->Start();
		m_shader->SetUniform3f(FOG_COLOR, *m_fogColor);
		m_shader->SetUniform4f(CLIP_PLANE, clipPlane);

		m_shader->SetUniformMat4(VIEW_MATRIX_TAG, viewMatrix);
		m_shader->SetUniform4f(INVERSE_VIEW_VECTOR_TAG, inverseViewVector);
		m_shader->SetUniformMat4(TO_SHADOW_SPACE_MATRIX_TAG, m_toShadowSpaceMatrix);

	#if ASTRA_DEBUG
		if (m_wireframe) 
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	#endif

		//glStencilFunc(GL_ALWAYS, 1, 0xFF);

		for (const auto& directory : m_entities)
		{
			PrepareEntity(directory.second.front());
			for (const Entity* entity : directory.second)
			{
				/*if (entity->IsSelected())
				{
					glStencilMask(0xFF);
				}*/

				m_shader->SetUniform2f(OFFSET_TAG, entity->GetMaterialXOffset(), entity->GetMaterialYOffset());
				m_shader->SetUniformMat4(NORMAL_MATRIX_TAG, entity->GetNormalMatrix());
				m_shader->SetUniformMat4(TRANSFORM_MATRIX_TAG, entity->GetModelMatrix());
				glDrawElements(entity->vertexArray->drawType, entity->vertexArray->vertexCount, GL_UNSIGNED_INT, NULL);

				//glStencilMask(0x00);
			}
			UnbindVertexArray();
		}

	#if ASTRA_DEBUG
		if (m_wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	#endif
		m_shader->Stop();
	#if ASTRA_DEBUG
		glCheckError();
	#endif 
	}

	void Entity3dRenderer::PrepareEntity(const Entity* entity)
	{
		glBindVertexArray(entity->vertexArray->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::TextureCoords));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Normals));

		m_shader->SetUniform1f(NUMBER_OF_ROWS, static_cast<float>(entity->material->GetRowCount()));

		if (entity->material->Transparent)
		{
			glDisable(GL_CULL_FACE);
		}

		if (entity->material != NULL)
		{
			m_shader->SetUniform1i(FAKE_LIGHT, entity->material->FakeLight);
			m_shader->SetUniform1f(MATERIAL_REFLECTIVITY, entity->material->Reflectivity);
			m_shader->SetUniform1i(GLOWING, entity->material->HasGlow());

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, entity->material->GetId());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, entity->material->GetSpecularId());
			if (entity->material->HasGlow())
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, entity->material->GetEmissionId());
			}
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
			size_t i = 0;
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
}