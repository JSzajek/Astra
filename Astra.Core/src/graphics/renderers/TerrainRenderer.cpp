#include "TerrainRenderer.h"
#include "../../math/Mat4Utils.h"

#include "../entities/PointLight.h"
#include "../entities/SpotLight.h"
#include "../shadows/ShadowMapController.h"

namespace Astra::Graphics
{
	TerrainRenderer::TerrainRenderer(const Math::Vec3* fogColor)
		: Renderer(), m_fogColor(fogColor), m_directionalLight(NULL), m_toShadowSpaceMatrix(NULL)
		#if _DEBUG
			, m_wireframe(false)
		#endif
	{
	}

	void TerrainRenderer::SetShader(Shader* shader)
	{
		Renderer::SetShader(shader);

		m_shader->Start();
		m_shader->SetUniform1i(BACKGROUND_TEXTURE,		0);
		m_shader->SetUniform1i(R_TEXTURE,				1);
		m_shader->SetUniform1i(G_TEXTURE,				2);
		m_shader->SetUniform1i(B_TEXTURE,				3);
		m_shader->SetUniform1i(BLEND_MAP,				4);
		m_shader->SetUniform1i(SPECULAR_MAP,			5);
		m_shader->SetUniform1i(SHADOW_MAP_TAG,			6);
		m_shader->SetUniform1f(SHADOW_DISTANCE_TAG,		SHADOW_DISTANCE);
		m_shader->SetUniform1f(TRANSITION_DISTANCE_TAG, TRANSITION_DISTANCE);
		m_shader->SetUniform1f(SHADOW_MAP_SIZE_TAG,		SHADOW_MAP_SIZE);
		m_shader->SetUniform1i(PCF_COUNT_TAG,			PCF_COUNT);
		m_shader->Stop();
	}

	inline void TerrainRenderer::Clear() 
	{ 
		m_terrains.clear();
		m_lights.clear();
	}

	void TerrainRenderer::Draw(float delta, const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		m_shader->Start();
		m_shader->SetUniform3f(FOG_COLOR, *m_fogColor);
		m_shader->SetUniform4f(CLIP_PLANE, clipPlane);

		m_shader->SetUniformMat4(VIEW_MATRIX_TAG, viewMatrix);
		m_shader->SetUniform4f(INVERSE_VIEW_VECTOR_TAG, inverseViewVector);
		m_shader->SetUniformMat4(TO_SHADOW_SPACE_MATRIX_TAG, m_toShadowSpaceMatrix);

	#if _DEBUG
		if (m_wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	#endif

		for (const auto& directory : m_terrains)
		{
			PrepareTerrain(directory.second.front());
			for (const Terrain* terrain : directory.second)
			{
				m_shader->SetUniformMat4(NORMAL_MATRIX_TAG, terrain->GetNormalMatrix());
				m_shader->SetUniformMat4(TRANSFORM_MATRIX_TAG, terrain->GetModelMatrix());
				glDrawElements(terrain->vertexArray->drawType, terrain->vertexArray->vertexCount, GL_UNSIGNED_INT, NULL);
			}
		}
	#if _DEBUG
		if (m_wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	#endif
		UnbindVertexArray();
		m_shader->Stop();
	#if _DEBUG
		glCheckError();
	#endif
	}

	void TerrainRenderer::AddTerrain(const Terrain* terrain)
	{
		auto temp = m_terrains.find(terrain->vertexArray->vaoId);
		if (temp != m_terrains.end())
		{
			temp->second.emplace_back(terrain);
		}
		else
		{
			m_terrains[terrain->vertexArray->vaoId] = std::vector<const Terrain*>();
			m_terrains[terrain->vertexArray->vaoId].emplace_back(terrain);
		}
	}

	void TerrainRenderer::AddLight(Light* light)
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
		light->SetCallback(std::bind(&TerrainRenderer::UpdateLight, this, light));
		UpdateLight(light);
	}

	void TerrainRenderer::UpdateLight(const Light* light)
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

	void TerrainRenderer::PrepareTerrain(const Terrain* terrain)
	{
		glBindVertexArray(terrain->vertexArray->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::TextureCoords));
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Normals));

		BindTerrainTextures(terrain);
	}

	void TerrainRenderer::BindTerrainTextures(const Terrain* terrain)
	{
		// TODO: Add shine and reflectivity back to terrains
		m_shader->SetUniform1f(MATERIAL_REFLECTIVITY, 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrain->texturePack->backgroundTexture->GetId());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, terrain->texturePack->rTexture->GetId());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, terrain->texturePack->gTexture->GetId());
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, terrain->texturePack->bTexture->GetId());
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, terrain->blendMap->GetId());
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, terrain->texturePack->specularTexture->id);
	}
}