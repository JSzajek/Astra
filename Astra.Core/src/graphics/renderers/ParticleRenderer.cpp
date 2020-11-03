#include "ParticleRenderer.h"
#include "../../math/Mat4Utils.h"
#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	ParticleRenderer::ParticleRenderer(ParticleShader* shader)
		: Renderer()
	{	
		Renderer::SetShader(shader);
		m_defaultQuad = Loader::Load(GL_TRIANGLE_STRIP, { -0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5, -0.5 }, 2);
		m_modelViewMatrix = new Math::Mat4(1);
	}

	ParticleRenderer::~ParticleRenderer()
	{
		delete m_modelViewMatrix;
		delete m_defaultQuad;
	}

	void ParticleRenderer::AddParticle(Particle* particle)
	{
		unsigned int id = particle->Material->id;
		auto temp = m_particles.find(id);
		if (temp != m_particles.end())
		{
			temp->second.push_back(particle);
		}
		else
		{
			m_particles[id] = std::vector<Particle*>();
			m_particles[id].push_back(particle);
		}
	}

	void ParticleRenderer::Draw(const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		if (m_particles.size() == 0) { return; }
		m_shader->Start();
		m_viewMatrix = viewMatrix;
		glBindVertexArray(m_defaultQuad->vaoId);
		glEnableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		for (const auto& directory : m_particles)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, directory.first);
			for (const Particle* particle : directory.second)
			{
				UpdateModelViewMatrix(particle->Position, particle->Rotation, particle->Scale);
				m_shader->SetUniform2f(TEX_OFFSET_1_TAG, particle->GetTexOffset1());
				m_shader->SetUniform2f(TEX_OFFSET_2_TAG, particle->GetTexOffset2());
				m_shader->SetUniform2f(TEX_COORD_INFO_TAG, particle->Material->GetRowCount(), particle->GetBlendFactor());
				glDrawArrays(m_defaultQuad->drawType, 0, m_defaultQuad->vertexCount);
			}
		}
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glDisableVertexAttribArray(static_cast<unsigned short>(BufferType::Vertices));
		glBindVertexArray(0);
		m_shader->Stop();
	}

	void ParticleRenderer::UpdateModelViewMatrix(const Math::Vec3& position, float rotation, float scale)
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
		m_modelViewMatrix->Rotate(rotation, Math::ZAxis);
		m_modelViewMatrix->Scale(scale);
		*m_modelViewMatrix = (*m_viewMatrix) * (*m_modelViewMatrix);
		m_shader->SetUniformMat4(MODEL_VIEW_MATRIX_TAG, m_modelViewMatrix);
	}
}