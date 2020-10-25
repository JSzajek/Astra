#include "ParticleController.h"

namespace Astra::Graphics
{
	ParticleController::ParticleController()
	{
		m_particleShader = new ParticleShader();
		m_particleRenderer = new ParticleRenderer(m_particleShader);
	}

	ParticleController::~ParticleController()
	{
		delete m_particleRenderer;
	}

	void ParticleController::UpdateProjectionMatrixImpl(const Math::Mat4& projectionMatrix)
	{
		m_particleRenderer->UpdateProjectionMatrix(projectionMatrix);
	}

	void ParticleController::UpdateImpl()
	{
		auto iter = m_particleRenderer->GetParticles().begin();
		while (iter != m_particleRenderer->GetParticles().end())
		{
			bool stillAlive = (*iter).Update();
			if (!stillAlive)
			{
				iter = m_particleRenderer->GetParticles().erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	void ParticleController::AddParticleImpl(const Particle& particle)
	{
		m_particleRenderer->AddParticle(particle);
	}

	void ParticleController::RenderImpl(const Math::Mat4& viewMatrix)
	{
		m_particleRenderer->Draw(viewMatrix, NULL);
	}
}