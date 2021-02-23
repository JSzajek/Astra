#include "astra_pch.h"

#include "WaterRenderer.h"
#include "Astra/math/Mat4Utils.h"
#include "Astra/graphics/loaders/Loader.h"
#include "Astra/graphics/ResourceManager.h"

#include "Astra/graphics/entities/PointLight.h"
#include "Astra/graphics/shadows/ShadowMapController.h"

namespace Astra::Graphics
{
	WaterRenderer::WaterRenderer(float _near, float _far)
		: Renderer(), m_buffer(NULL), m_near(_near), m_far(_far), m_toShadowSpaceMatrix(NULL)
		#if ASTRA_DEBUG
			, m_wireframe(false)
		#endif
	{
		m_defaultQuad = Loader::Load(GL_TRIANGLES, { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 }, 2);
	}

	WaterRenderer::~WaterRenderer()
	{
		ResourceManager::Unload(m_defaultQuad);
	}

	void WaterRenderer::SetShader(Shader* shader)
	{
		Renderer::SetShader(shader);

		m_shader->Start();
		m_shader->SetUniform1f(NEAR_PLANE,				m_near);
		m_shader->SetUniform1f(FAR_PLANE,				m_far);

		m_shader->SetUniform1i(DIFFUSE_MAP,				0);
		m_shader->SetUniform1i(REFLECTION_TEXTURE,		1);
		m_shader->SetUniform1i(REFRACTION_TEXTURE,		2);
		m_shader->SetUniform1i(DUDVMAP_TEXTURE,			3);
		m_shader->SetUniform1i(NORMALMAP_TEXTURE,		4);
		m_shader->SetUniform1i(SPECULAR_MAP,			5);
		m_shader->SetUniform1i(DEPTHMAP_TEXTURE,		7);

		m_shader->SetUniform1i(SHADOW_MAP_TAG,			6);
		m_shader->SetUniform1f(SHADOW_DISTANCE_TAG,		SHADOW_DISTANCE);
		m_shader->SetUniform1f(TRANSITION_DISTANCE_TAG, TRANSITION_DISTANCE);
		m_shader->SetUniform1f(SHADOW_MAP_SIZE_TAG,		SHADOW_MAP_SIZE);
		m_shader->SetUniform1i(PCF_COUNT_TAG,			PCF_COUNT);
		m_shader->Stop();
	}

	void WaterRenderer::Draw(float delta, const std::unordered_map<unsigned int, Graphics::WaterTile>& tiles, 
							 const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		m_shader->Start();
		m_shader->SetUniformMat4(VIEW_MATRIX_TAG, viewMatrix);
		m_shader->SetUniform4f(INVERSE_VIEW_VECTOR_TAG, inverseViewVector);
		m_shader->SetUniformMat4(TO_SHADOW_SPACE_MATRIX_TAG, m_toShadowSpaceMatrix);

	#if ASTRA_DEBUG
		if (m_wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	#endif

		PrepareRender();
		for (const auto& tileDir: tiles)
		{
			PrepareTile(&tileDir.second);
			m_shader->SetUniform1f(MOVE_FACTOR, tileDir.second.material->Increase(delta));
			m_shader->SetUniformMat4(TRANSFORM_MATRIX_TAG, tileDir.second.GetModelMatrix());
			glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);
		}

	#if ASTRA_DEBUG
		if (m_wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	#endif
		glBindVertexArray(0);
		glDisable(GL_BLEND);

		m_shader->Stop();
	#if ASTRA_DEBUG
		glCheckError();
	#endif
	}

	void WaterRenderer::AddLight(unsigned int index, Light* light)
	{
		UpdateLight(index, light);
		light->SetCallback(std::bind(&Renderer::UpdateLight, this, index, light));
	}

	void WaterRenderer::SetReflection(bool enabled)
	{
		m_shader->Start();
		m_shader->SetUniform1i(REFLECTION_TAG, (int)enabled);
		m_shader->Stop();
	}

	void WaterRenderer::PrepareRender()
	{
		glBindVertexArray(m_defaultQuad->vaoId);

		if (m_buffer)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_buffer->GetReflectionBuffer()->GetColorAttachment());
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_buffer->GetRefractionBuffer()->GetColorAttachment());
			glActiveTexture(GL_TEXTURE7);
			glBindTexture(GL_TEXTURE_2D, m_buffer->GetRefractionBuffer()->GetDepthAttachment());
		}
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void WaterRenderer::PrepareTile(const WaterTile* tile)
	{
		m_shader->SetUniform1f(WAVE_STRENGTH, tile->material->waveStrength);
		m_shader->SetUniform1f(MATERIAL_REFLECTIVITY, tile->material->reflectivity);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tile->material->diffuseTexture->id);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, tile->material->dudvTexture->id);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, tile->material->normalTexture->id);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, tile->material->GetSpecularId());
	}

	void WaterRenderer::UnbindVertexArray()
	{
		glDisable(GL_BLEND);
		Renderer::UnbindVertexArray();
	}
}
