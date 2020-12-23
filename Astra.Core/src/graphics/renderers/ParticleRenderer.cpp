#include "ParticleRenderer.h"
#include "../../math/Mat4Utils.h"
#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	ParticleRenderer::ParticleRenderer(ParticleShader* shader)
		: Renderer(), m_modelViewMatrix(new Math::Mat4(1)), m_viewMatrix(new Math::Mat4())
	{
		Renderer::SetShader(shader);

		m_defaultVAO = CreateDefaultQuadVao();
		CreateInstancedBuffer(&m_defaultVBO, MAX_PARTICLES * sizeof(ParticleInfo), true);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		
		m_shader->Start();
		int textureSet[MAX_TEXTURE_SLOTS];
		for (size_t i = 0; i < MAX_TEXTURE_SLOTS; i++) { textureSet[i] = i; }
		m_shader->SetUniform1iv("instanced_Textures", 16, textureSet);
		m_shader->Stop();
	}

	ParticleRenderer::~ParticleRenderer()
	{
		delete m_modelViewMatrix;
		for (size_t x = 0; x < NUM_OF_TYPES; x++)
		{
			for (auto* particle : m_particles[x])
			{
				delete particle;
			}
		}
	}

	void ParticleRenderer::Clear()
	{
		for (auto& particles : m_particles)
		{
			particles.clear();
		}
	}

	void ParticleRenderer::AddParticle(Particle* particle)
	{
		// Check if particles textures is within any of the texture buffers
		if (!particle->GetAdditive())
		{
			m_particles[0].push_back(particle);
		}
		else
		{
			m_particles[1].push_back(particle);
		}
	}

	void ParticleRenderer::Flush(std::unordered_map<unsigned int, unsigned int>& mapping, size_t& offset)
	{
		size_t i = 0;
		for (const auto& t : mapping)
		{
			// Bind all of the relevent textures involved in this pass
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, t.first);
			i++;
		}

		// Flush and render the current stored particles
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, offset);
		mapping.clear();
		offset = 0;
	}

	void ParticleRenderer::Draw(float delta, const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector, const Math::Vec4& clipPlane)
	{
		if (m_particles[0].size() == 0 && m_particles[1].size() == 0) return;
		
		m_shader->Start();
		m_viewMatrix = viewMatrix;
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);

		glBindVertexArray(m_defaultVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_defaultVBO);

		std::unordered_map<unsigned int, unsigned int> m_textureMapping;
		size_t offset = 0;
		unsigned int slot;
		for (size_t x = 0; x < NUM_OF_TYPES; x++)
		{
			glBlendFunc(GL_SRC_ALPHA, x == 1 ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA);
			for (const auto* particle : m_particles[x])
			{
				if (offset == MAX_PARTICLES)
				{
					Flush(m_textureMapping, offset);
				}

				const auto& temp = m_textureMapping.find(particle->Material->GetId());
				if (temp == m_textureMapping.end())
				{
					if (m_textureMapping.size() + 1 == MAX_TEXTURE_SLOTS)
					{
						Flush(m_textureMapping, offset);
					}
					slot = m_textureMapping.size();
					m_textureMapping[particle->Material->GetId()] = m_textureMapping.size();
				}
				else
				{
					slot = temp->second;
				}

				// Determine whether subbuffer once or per offset is more efficient.
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

				glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(ParticleInfo), 16 * sizeof(float), m_modelViewMatrix->data);
				float dat[7] =
				{
					particle->GetTexOffset1().x,
					particle->GetTexOffset1().y,
					particle->GetTexOffset2().x,
					particle->GetTexOffset2().y,
					static_cast<float>(slot),
					static_cast<float>(particle->Material->GetRowCount()),
					particle->GetBlendFactor()
				};
				glBufferSubData(GL_ARRAY_BUFFER, (offset * sizeof(ParticleInfo)) + (16 * sizeof(float)), 7 * sizeof(float), dat);
				offset++;
			}

			if (offset > 0)
			{
				Flush(m_textureMapping, offset);
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glBindVertexArray(0);
		m_shader->Stop();
	#if _DEBUG
		glCheckError();
	#endif
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
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleInfo), (void*)0);						// Model View Matrix
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleInfo), (void*)(1 * 4 * sizeof(float)));	// Model View Matrix
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleInfo), (void*)(2 * 4 * sizeof(float)));	// Model View Matrix
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleInfo), (void*)(3 * 4 * sizeof(float)));	// Model View Matrix
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleInfo), (void*)(4 * 4 * sizeof(float)));	// Texture Offsets
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleInfo), (void*)(5 * 4 * sizeof(float)));	// Animation Info

		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
	}
}