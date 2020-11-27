#include "ParticleRenderer.h"
#include "../../math/Mat4Utils.h"
#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	ParticleRenderer::ParticleRenderer(ParticleShader* shader)
		: Renderer(), m_modelViewMatrix(new Math::Mat4(1)), m_viewMatrix(new Math::Mat4())
	{	
		Renderer::SetShader(shader);
	}

	ParticleRenderer::~ParticleRenderer()
	{
		delete m_modelViewMatrix;

		for (const auto& directory : m_particles)
		{
			for (auto* particle : directory.second)
			{
				delete particle;
			}
		}
	}

	void ParticleRenderer::AddParticle(Particle* particle)
	{
		unsigned int id = particle->Material->GetId();
		auto temp = m_particles.find(id);
		if (temp != m_particles.end())
		{
			temp->second.push_back(particle);
		}
		else
		{
			m_particles[id] = std::vector<Particle*>();
			m_particles[id].push_back(particle);

			unsigned int vao, vbo;
			vao = CreateDefaultQuadVao();
			CreateInstancedBuffer(&vbo, MAX_PARTICLES * 22 * sizeof(float), true);
			m_buffers[id] = ParticleBuffer(vao, vbo, MAX_PARTICLES);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	}

	void ParticleRenderer::Draw(float delta, const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		if (m_particles.size() == 0) { return; }
		m_shader->Start();
		m_viewMatrix = viewMatrix;
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);

		UpdateParticleData();

		for (const auto& directory : m_buffers)
		{
			const auto& particles = m_particles[directory.first];
			glBlendFunc(GL_SRC_ALPHA, particles.front()->GetAdditive() ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA);
			auto buff = directory.second;
			glBindVertexArray(buff.VAO);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, directory.first);

			glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles.size());
		}

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glBindVertexArray(0);
		m_shader->Stop();
	#if _DEBUG
		glCheckError();
	#endif
	}

	void ParticleRenderer::UpdateParticleData()
	{
		for (const auto& directory : m_particles)
		{
			auto& buff = m_buffers[directory.first];
			glBindVertexArray(buff.VAO);
			glBindBuffer(GL_ARRAY_BUFFER, buff.VBO);
			size_t offset = 0;

			// TODO: Add capping for max particle rendering
			for (auto* particle : directory.second) // Fill data buffer
			{
				m_modelViewMatrix->SetIdentity();
				m_modelViewMatrix->Translate(particle->Position);
				m_modelViewMatrix->columns[0][0] = m_viewMatrix->columns[0].x;
				m_modelViewMatrix->columns[0][1] = m_viewMatrix->columns[1].x;
				m_modelViewMatrix->columns[0][2] = m_viewMatrix->columns[2].x;
				m_modelViewMatrix->columns[1][0] = m_viewMatrix->columns[0].y;
				m_modelViewMatrix->columns[1][1] = m_viewMatrix->columns[1].y;
				m_modelViewMatrix->columns[1][2] = m_viewMatrix->columns[2].y;
				m_modelViewMatrix->columns[2][0] = m_viewMatrix->columns[0].z;
				m_modelViewMatrix->columns[2][1] = m_viewMatrix->columns[1].z;
				m_modelViewMatrix->columns[2][2] = m_viewMatrix->columns[2].z;
				m_modelViewMatrix->Rotate(particle->Rotation, Math::Vec3::Z_Axis);
				m_modelViewMatrix->Scale(particle->Scale);
				*m_modelViewMatrix = (*m_viewMatrix) * (*m_modelViewMatrix);
				
				glBufferSubData(GL_ARRAY_BUFFER, offset * 22 * sizeof(float), 16 * sizeof(float), m_modelViewMatrix->data);
				float dat[6] = 
				{
					particle->GetTexOffset1().x,
					particle->GetTexOffset1().y,
					particle->GetTexOffset2().x,
					particle->GetTexOffset2().y,
					static_cast<float>(particle->Material->GetRowCount()),
					particle->GetBlendFactor()
				};
				glBufferSubData(GL_ARRAY_BUFFER, (offset * 22 * sizeof(float)) + (16 * sizeof(float)), 6 * sizeof(float), dat);
				offset++;
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	}

	unsigned int ParticleRenderer::CreateDefaultQuadVao()
	{
		static const std::vector<float> m_defaultVertices = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f };

		unsigned int quadVao, quadVbo;
		glGenVertexArrays(1, &quadVao);
		glGenBuffers(1, &quadVbo);
		glBindVertexArray(quadVao);
		glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_defaultVertices.size(), &m_defaultVertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return quadVao;
	}

	void ParticleRenderer::CreateInstancedBuffer(unsigned int* id, size_t size, bool gen)
	{
		if (gen)
		{
			glGenBuffers(1, id);
		}
		glBindBuffer(GL_ARRAY_BUFFER, *id);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(1 * 4 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(2 * 4 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(3 * 4 * sizeof(float)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(4 * 4 * sizeof(float)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(5 * 4 * sizeof(float)));

		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
	}
}