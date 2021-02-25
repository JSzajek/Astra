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
		: Renderer(), m_fogColor(fogColor), m_toShadowSpaceMatrix(NULL)
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

	void Entity3dRenderer::Draw(float delta, const std::unordered_map<unsigned int, std::vector<const Model*>>& models,
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

		for (const auto& directory : models)
		{
			const auto& mesh = directory.second.front()->GetMesh();
			glBindVertexArray(mesh->GetVAO());

			for (const auto* model : directory.second)
			{
				PrepareModel(model);
				
				m_shader->SetUniformMat4(NORMAL_MATRIX_TAG, model->GetNormalMatrix());
				m_shader->SetUniformMat4(TRANSFORM_MATRIX_TAG, model->GetModelMatrix());

				if (model->HasAnimator())
				{
					m_shader->SetUniform1i("animated", true);

					const auto size = model->GetAnimator()->GetCount();
					for (unsigned int i = 0; i < size; ++i)
					{
						m_shader->SetUniformMat4(Shader::GetBoneTransformTag(i), model->GetAnimator()->GetOffsets()[i]);
					}
				}
				else
				{
					m_shader->SetUniform1i("animated", false);
				}

				glDrawElements(mesh->GetDrawType(), mesh->GetVertexCount(), GL_UNSIGNED_INT, NULL);
			}
			glBindVertexArray(0);
			glEnable(GL_CULL_FACE);
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

	void Entity3dRenderer::PrepareModel(const Model* model)
	{
		const auto& material = model->GetMaterial();
		if (material.GetTransparency())
		{
			glDisable(GL_CULL_FACE);
		}

		m_shader->SetUniform2f(OFFSET_TAG, model->GetMaterialXOffset(), model->GetMaterialYOffset());
		m_shader->SetUniform1f(NUMBER_OF_ROWS, static_cast<float>(model->GetRowCount()));

		m_shader->SetUniform1i(FAKE_LIGHT, material.GetFakeLighting());
		m_shader->SetUniform1f(MATERIAL_REFLECTIVITY, material.GetReflectivity());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material.GetTextureId(TextureType::DiffuseMap));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material.GetTextureId(TextureType::SpecularMap));
			
		auto hasGlow = material.HasTexture(TextureType::EmissionMap);
		m_shader->SetUniform1i(GLOWING, hasGlow);
		if (hasGlow)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, material.GetTextureId(TextureType::EmissionMap));
		}
	}

	void Entity3dRenderer::AddLight(unsigned int index, Light* light)
	{
		UpdateLight(index, light);
		light->SetCallback(std::bind(&Renderer::UpdateLight, this, index, light));
	}
}