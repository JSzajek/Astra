#include "ParticleRenderer.h"
#include "../../math/Mat4Utils.h"
#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	ParticleRenderer::ParticleRenderer(ParticleShader* shader)
		: Renderer((Shader*)shader)
	{
		m_defaultQuad = Loader::Load(GL_TRIANGLE_STRIP, { -0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5, -0.5 }, 2);
	}

	ParticleRenderer::~ParticleRenderer()
	{
		delete m_defaultQuad;
	}

	void ParticleRenderer::Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane)
	{
		if (m_particles.size() == 0) { return; }
		m_shader->Start();
		m_viewMatrix = viewMatrix;
		glBindVertexArray(m_defaultQuad->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		for (const Particle& particle : m_particles)
		{
			UpdateModelViewMatrix(particle.Position, particle.Rotation, particle.Scale);
			glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);
		}

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glDisableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glBindVertexArray(0);
		m_shader->Stop();
	}

	void ParticleRenderer::UpdateModelViewMatrix(const Math::Vec3& position, float rotation, float scale)
	{
		Math::Mat4 modelMatrix = Math::Mat4(1);
		modelMatrix = modelMatrix.Translate(position);
		modelMatrix.columns[0][0] = m_viewMatrix.columns[0][0];
		modelMatrix.columns[0][1] = m_viewMatrix.columns[1][0];
		modelMatrix.columns[0][2] = m_viewMatrix.columns[2][0];
		modelMatrix.columns[1][0] = m_viewMatrix.columns[0][1];
		modelMatrix.columns[1][1] = m_viewMatrix.columns[1][1];
		modelMatrix.columns[1][2] = m_viewMatrix.columns[2][1];
		modelMatrix.columns[2][0] = m_viewMatrix.columns[0][2];
		modelMatrix.columns[2][1] = m_viewMatrix.columns[1][2];
		modelMatrix.columns[2][2] = m_viewMatrix.columns[2][2];
		modelMatrix = modelMatrix.Rotate(rotation, Math::Vec3(0, 0, 1));
		modelMatrix = modelMatrix.Scale(Math::Vec3(scale));
		Math::Mat4 modelViewMatrix = m_viewMatrix * modelMatrix;
		m_shader->SetUniformMat4(ParticleShader::ModelViewMatrixTag, modelViewMatrix);
	}
}