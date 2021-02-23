#include "astra_pch.h"

#include "ShadowMapRenderer.h"
#include "Astra/math/Mat4Utils.h"

namespace Astra::Graphics
{
	ShadowMapRenderer::ShadowMapRenderer(ShadowShader* shader, ShadowFrameBuffer* buffer, const Math::Mat4& projectionViewMatrix)
		: Renderer(), m_buffer(buffer), projectionViewMatrix(projectionViewMatrix)
	{
		Renderer::SetShader(shader);
	}

	void ShadowMapRenderer::Draw(float delta, const std::unordered_map<unsigned int, std::vector<const Model*>>& models,
								 const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		BindFrameBuffer(m_buffer->GetBuffer()->GetId(), m_buffer->GetWidth(), m_buffer->GetHeight());
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);

		m_shader->Start();
		for (const auto& directory : models)
		{
			for (const auto& mesh : directory.second.front()->GetMeshes())
			{
				PrepareMesh(mesh);
				for (const auto* model : directory.second)
				{
					m_shader->SetUniformMat4(MODEL_VIEW_PROJ_MATRIX_TAG, projectionViewMatrix * (*model->GetModelMatrix()));
					m_shader->SetUniform1f(NUMBER_OF_ROWS_TAG, static_cast<float>(model->GetRowCount()));
					m_shader->SetUniform2f(OFFSET_TAG, model->GetMaterialXOffset(), model->GetMaterialYOffset());
					
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

					glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, NULL);
				}
				glBindVertexArray(0);
			}
		}
		UnbindFrameBuffer();
		m_shader->Stop();
	#if ASTRA_DEBUG
		glCheckError();
	#endif
	}

	void ShadowMapRenderer::PrepareMesh(const Mesh& mesh)
	{
		glBindVertexArray(mesh.GetVAO());

		if (const auto* material = mesh.GetMaterial())
		{
			if (material->GetTransparency())
			{
				glDisable(GL_CULL_FACE);
			}
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->GetTextureId(TextureType::DiffuseMap));
		}
	}
}