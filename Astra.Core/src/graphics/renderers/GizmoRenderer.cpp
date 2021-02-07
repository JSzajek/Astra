#include "GizmoRenderer.h"

#include "../loaders/Loader.h"
#include "../ResourceManager.h"

namespace Astra::Graphics
{
#if _DEBUG
	GizmoRenderer::GizmoRenderer(GizmoShader* shader)
		: m_viewMatrix(NULL)
	{
		Renderer::SetShader(shader);
		m_defaultQuad = Loader::Load(GL_TRIANGLE_STRIP, { -0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5, -0.5 }, 2);
		m_modelViewMatrix = new Math::Mat4(1);
	}

	GizmoRenderer::~GizmoRenderer()
	{
		ResourceManager::Unload(m_defaultQuad);
		delete m_modelViewMatrix;
	}

	void GizmoRenderer::AddGizmo(const Gizmo* gizmo)
	{
		unsigned int id = gizmo->GetTextureId();
		auto temp = m_gizmos.find(id);
		if (temp != m_gizmos.end())
		{
			temp->second.push_back(gizmo);
		}
		else
		{
			m_gizmos[id] = std::vector<const Gizmo*>();
			m_gizmos[id].push_back(gizmo);
		}
	}

	void GizmoRenderer::Draw(float delta, const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		if (m_gizmos.size() == 0) { return; }
		m_shader->Start();
		m_viewMatrix = viewMatrix;
		glBindVertexArray(m_defaultQuad->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		for (const auto& directory : m_gizmos)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, directory.first);
			for (const Gizmo* gizmo : directory.second)
			{
				UpdateModelViewMatrix(gizmo->Position, gizmo->Scale);
				glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);
			}
		}

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glDisableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
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