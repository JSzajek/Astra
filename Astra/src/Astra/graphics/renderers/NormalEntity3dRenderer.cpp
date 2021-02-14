#include "astra_pch.h"

#include "NormalEntity3dRenderer.h"
#include "../../math/Mat4Utils.h"
#include "../shaders/SelectionShader.h"

#include "../entities/PointLight.h"
#include "../entities/SpotLight.h"
#include "../shadows/ShadowMapController.h"

namespace Astra::Graphics
{
	NormalEntity3dRenderer::NormalEntity3dRenderer(const Color* fogColor)
		: Renderer(), m_fogColor(fogColor), m_directionalLight(NULL), m_drawSelection(false),
			m_selectionShader(new SelectionShader()), m_toShadowSpaceMatrix(NULL)
		#if ASTRA_DEBUG
			, m_wireframe(false)
		#endif
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

	void NormalEntity3dRenderer::SetSelectionColor(const Math::Vec3& color)
	{
		m_selectionShader->Start();
		m_selectionShader->SetUniform3f(SELECTION_COLOR_TAG, color);
		m_selectionShader->Stop();
	}

	void NormalEntity3dRenderer::UpdateProjectionMatrix(const Math::Mat4* projectionMatrix)
	{
		m_shader->Start();
		m_shader->SetUniformMat4(PROJECTION_MATRIX_TAG, projectionMatrix);
		m_shader->Stop();

		m_selectionShader->Start();
		m_selectionShader->SetUniformMat4(PROJECTION_MATRIX_TAG, projectionMatrix);
		m_selectionShader->Stop();
	}

	void NormalEntity3dRenderer::Clear()
	{
		m_entities.clear();
		m_lights.clear();
	}

	void NormalEntity3dRenderer::Draw(float delta, const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		glClear(GL_STENCIL_BUFFER_BIT);

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

		for (const auto& directory : m_entities)
		{
			bool selected = false;
			PrepareEntity(directory.second.front());
			for (const Entity* entity : directory.second)
			{
				if (m_drawSelection && entity->IsSelected())
				{
					glStencilFunc(GL_ALWAYS, 1, 0xFF);
					glStencilMask(0xFF);
					selected = true;
				}
				else
				{
					glStencilMask(0x00);
				}

				m_shader->SetUniform2f(OFFSET_TAG, entity->GetMaterialXOffset(), entity->GetMaterialYOffset());
				m_shader->SetUniformMat4(NORMAL_MATRIX_TAG, entity->GetNormalMatrix());
				m_shader->SetUniformMat4(TRANSFORM_MATRIX_TAG, entity->GetModelMatrix());
				glDrawElements(entity->vertexArray->drawType, entity->vertexArray->vertexCount, GL_UNSIGNED_INT, NULL);
			}
			if (selected) { m_selected.emplace(directory); }
			UnbindVertexArray();
		}

	#if ASTRA_DEBUG
		if (m_wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	#endif
		m_shader->Stop();
		if (m_drawSelection)
		{
			DrawSelected(viewMatrix);
		}
	#if ASTRA_DEBUG
		glCheckError();
	#endif
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
		
		auto* material = entity->material;
		if (material != NULL)
		{
			m_shader->SetUniform1f(NUMBER_OF_ROWS, static_cast<float>(material->GetRowCount()));
			if (material->Transparent)
			{
				glDisable(GL_CULL_FACE);
			}

			m_shader->SetUniform1f(HEIGHT_SCALE, material->GetHeightOffset());
			m_shader->SetUniform1i(FAKE_LIGHT, material->FakeLight);
			m_shader->SetUniform1i(NORMAL_MAPPED_FLAG_TAG, material->IsNormalMapped());
			m_shader->SetUniform1i(PARALLAX_MAPPED_FLAG_TAG, material->IsParallaxMapped());
			m_shader->SetUniform1i(GLOWING, material->HasGlow());

			m_shader->SetUniform1f(MATERIAL_REFLECTIVITY, material->Reflectivity);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->GetId());
			if (material->IsNormalMapped())
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, material->GetNormalMapId());
			}
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, material->GetSpecularId());
			if (material->IsParallaxMapped())
			{
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, material->GetParallaxMapId());
			}
			if (material->HasGlow())
			{
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, material->GetEmissionId());
			}
		}
	}

	void NormalEntity3dRenderer::DrawSelected(const Math::Mat4* viewMatrix)
	{
		m_selectionShader->Start();
		m_selectionShader->SetUniformMat4(VIEW_MATRIX_TAG, viewMatrix);
		while (m_selected.size() > 0)
		{
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);

			const auto& directory = m_selected.top();
			const Entity* first = directory.second.front();
			const auto* material = first->material;

			glBindVertexArray(first->vertexArray->vaoId);
			glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
			glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::TextureCoords));
			
			if (material != NULL)
			{
				if (material->Transparent) 
				{
					glDisable(GL_CULL_FACE);
				}
				m_selectionShader->SetUniform1f(NUMBER_OF_ROWS, static_cast<float>(material->GetRowCount()));

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, material->GetId());
			}

			for (const Entity* entity : directory.second)
			{
				if (entity->IsSelected())
				{
					m_selectionShader->SetUniform2f(OFFSET_TAG, entity->GetMaterialXOffset(), entity->GetMaterialYOffset());
					m_selectionShader->SetUniformMat4(TRANSFORM_MATRIX_TAG, entity->GetSelectedModelMatrix());
					glDrawElements(entity->vertexArray->drawType, entity->vertexArray->vertexCount, GL_UNSIGNED_INT, NULL);
				}
			}
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			UnbindVertexArray();
			m_selected.pop();
		}
		m_selectionShader->Stop();
		m_drawSelection = false;
	}
}