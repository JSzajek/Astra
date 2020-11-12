#include "ParticleController.h"
#include <algorithm>

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

	void ParticleController::UpdateProjectionMatrixImpl(const Math::Mat4* projectionMatrix)
	{
		m_particleRenderer->UpdateProjectionMatrix(projectionMatrix);
	}

	void ParticleController::UpdateImpl(const Math::Vec3& cameraPosition)
	{
		auto iter = m_particleRenderer->GetParticles().begin();
		while (iter != m_particleRenderer->GetParticles().end())
		{
			auto particlesIter = (*iter).second.begin();
			while (particlesIter != (*iter).second.end())
			{
				bool stillAlive = (*particlesIter)->Update(cameraPosition);
				if (!stillAlive)
				{
					auto* temp = (*particlesIter);
					particlesIter = (*iter).second.erase(particlesIter);
					delete temp;
				}
				else
				{
					++particlesIter;
				}
			}

			if ((*iter).second.empty())
			{
				iter = m_particleRenderer->GetParticles().erase(iter);
			}
			else
			{
				InsertionSort((*iter).second);
				++iter;
			}
		}
	}

	void ParticleController::AddParticleImpl(Particle* particle)
	{
		m_particleRenderer->AddParticle(particle);
	}

	void ParticleController::RenderImpl(const Math::Mat4* viewMatrix)
	{
		m_particleRenderer->Draw(viewMatrix, NULL);
	}

	void ParticleController::InsertionSort(std::vector<Particle*>& particles)
	{
		for (auto it = particles.begin(); it != particles.end(); it++)
		{
			auto const insertion_point = std::upper_bound(particles.begin(), it, *it,
					[](const Particle* a, const Particle* b) {return a->GetDistance() > b->GetDistance(); });
			std::rotate(insertion_point, it, it + 1);
		}
	}

	void ParticleController::ClearImpl()
	{
		m_particleRenderer->Clear();
	}
}