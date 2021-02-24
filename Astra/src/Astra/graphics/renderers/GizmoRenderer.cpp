#include "astra_pch.h"

#include "GizmoRenderer.h"
#include "Astra/graphics/loaders/Loader.h"

namespace Astra::Graphics
{
#if ASTRA_DEBUG
	GizmoRenderer::GizmoRenderer(GizmoShader* shader)
		: m_viewMatrix(NULL)
	{
		Renderer::SetShader(shader);
		m_defaultQuad = Loader::Load(GL_TRIANGLE_STRIP, { -0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5, -0.5 }, 2);
		m_modelViewMatrix = new Math::Mat4(1);
	}

	GizmoRenderer::~GizmoRenderer()
	{
		delete m_defaultQuad;
		delete m_modelViewMatrix;
	}

	void GizmoRenderer::Draw(const std::unordered_map<unsigned int, std::vector<const Graphics::Gizmo*>>& gizmos, 
							 const Math::Mat4* viewMatrix)
	{
		if (gizmos.size() == 0) { return; }

		m_shader->Start();
		m_viewMatrix = viewMatrix;
		glBindVertexArray(m_defaultQuad->vaoId);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		for (const auto& directory : gizmos)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, directory.second.front()->GetTextureId());
			for (const Gizmo* gizmo : directory.second)
			{
				UpdateModelViewMatrix(gizmo->Position, gizmo->Scale);
				glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);
			}
		}

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glBindVertexArray(0);
		m_shader->Stop();
		glCheckError();
	}

	void GizmoRenderer::UpdateModelViewMatrix(const Math::Vec3& position, float scale)
	{
		m_modelViewMatrix->SetIdentity();
		m_modelViewMatrix->Translate(position);
		m_modelViewMatrix->columns[0][0] = m_viewMatrix->columns[0].x;
		m_modelViewMatrix->columns[0][1] = m_viewMatrix->columns[1].x;
		m_modelViewMatrix->columns[0][2] = m_viewMatrix->columns[2].x;
		m_modelViewMatrix->columns[1][0] = m_viewMatrix->columns[0].y;
		m_modelViewMatrix->columns[1][1] = m_viewMatrix->columns[1].y;
		m_modelViewMatrix->columns[1][2] = m_viewMatrix->columns[2].y;
		m_modelViewMatrix->columns[2][0] = m_viewMatrix->columns[0].z;
		m_modelViewMatrix->columns[2][1] = m_viewMatrix->columns[1].z;
		m_modelViewMatrix->columns[2][2] = m_viewMatrix->columns[2].z;
		m_modelViewMatrix->Scale(scale);
		*m_modelViewMatrix = (*m_viewMatrix) * (*m_modelViewMatrix);
		m_shader->SetUniformMat4(MODEL_VIEW_MATRIX_TAG, m_modelViewMatrix);
	}
#endif
}