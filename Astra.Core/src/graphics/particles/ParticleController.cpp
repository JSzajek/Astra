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
		while (m_leftovers.size() > 0) 
		{
			auto* leftover = m_leftovers.top();
			delete leftover;
			m_leftovers.pop();
		}
	}

	void ParticleController::UpdateProjectionMatrixImpl(const Math::Mat4* projectionMatrix)
	{
		m_particleRenderer->UpdateProjectionMatrix(projectionMatrix);
	}

	void ParticleController::UpdateImpl(float delta, const Math::Vec3& cameraPosition)
	{
		for (size_t x = 0; x < NUM_OF_TYPES; x++)
		{
			auto& particles = m_particleRenderer->GetParticles(x);

			auto particlesIter = particles.begin();
			while (particlesIter != particles.end())
			{ 
				bool stillAlive = (*particlesIter)->Update(delta, cameraPosition);
				if (!stillAlive)
				{
					auto* temp = (*particlesIter);
					particlesIter = particles.erase(particlesIter);
					m_leftovers.emplace(temp);
				}
				else
				{
					++particlesIter;
				}
			}

			if (particles.empty())
			{
			}
			else if (x == 0)
			{
				InsertionSort(particles);
			}
		}

		//auto iter = m_particleRenderer->GetParticles().begin();
		//while (iter != m_particleRenderer->GetParticles().end())
		//{
		//	bool additive = (*(*iter).second.begin())->GetAdditive();

		//	auto particlesIter = (*iter).second.begin();
		//	while (particlesIter != (*iter).second.end())
		//	{
		//		bool stillAlive = (*particlesIter)->Update(delta, cameraPosition);
		//		if (!stillAlive)
		//		{
		//			auto* temp = (*particlesIter);
		//			particlesIter = (*iter).second.erase(particlesIter);
		//			m_leftovers.emplace(temp);
		//		}
		//		else
		//		{
		//			++particlesIter;
		//		}
		//	}

		//	if ((*iter).second.empty())
		//	{
		//		iter = m_particleRenderer->GetParticles().erase(iter);
		//		return;
		//	}
		//	else if (!additive)
		//	{
		//		InsertionSort((*iter).second);
		//	}
		//	++iter;
		//}
	}

	void ParticleController::AddParticleImpl(Particle* particle)
	{
		m_particleRenderer->AddParticle(particle);
	}

	void ParticleController::RenderImpl(const Math::Mat4* viewMatrix)
	{
		m_particleRenderer->Draw(0, viewMatrix, NULL);
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