#include "astra_pch.h"

#include "TerrainRenderer.h"
#include "Astra/math/Mat4Utils.h"

#include "Astra/graphics/entities/PointLight.h"
#include "Astra/graphics/shadows/ShadowMapController.h"

namespace Astra::Graphics
{
	TerrainRenderer::TerrainRenderer(const Color* fogColor)
		: Renderer(), m_fogColor(fogColor), m_toShadowSpaceMatrix(NULL)
		#if ASTRA_DEBUG
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

	void TerrainRenderer::Draw(float delta, const std::unordered_map<unsigned int, std::vector<const Graphics::Terrain*>>& terrains,
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

		for (const auto& directory : terrains)
		{
			const auto* mesh = directory.second.front()->GetMesh();
			glBindVertexArray(mesh->GetVAO());

			BindTerrainTextures(directory.second.front());
			for (const auto* terrain : directory.second)
			{
				m_shader->SetUniformMat4(NORMAL_MATRIX_TAG, terrain->GetNormalMatrix());
				m_shader->SetUniformMat4(TRANSFORM_MATRIX_TAG, terrain->GetModelMatrix());
				glDrawElements(GL_TRIANGLES, mesh->GetVertexCount(), GL_UNSIGNED_INT, NULL);
			}
		}
	#if ASTRA_DEBUG
		if (m_wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	#endif
		glBindVertexArray(0);
		m_shader->Stop();
	#if ASTRA_DEBUG
		glCheckError();
	#endif
	}

	void TerrainRenderer::AddLight(unsigned int index, Light* light)
	{
		UpdateLight(index, light);
		light->SetCallback(std::bind(&Renderer::UpdateLight, this, index, light));
	}

	void TerrainRenderer::BindTerrainTextures(const Terrain* terrain)
	{
		// TODO: Add shine and reflectivity back to terrains
		m_shader->SetUniform1f(MATERIAL_REFLECTIVITY, 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrain->material.GetBackgroundId());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, terrain->material.GetRedId());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, terrain->material.GetGreenId());
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, terrain->material.GetBlueId());
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, terrain->material.GetBlendMapId());
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, terrain->material.GetSpecularId());
	}
}