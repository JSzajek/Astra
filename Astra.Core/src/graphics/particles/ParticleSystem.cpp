#include "ParticleSystem.h"

#include "../Window.h"

namespace Astra::Graphics
{
	ParticleSystem::ParticleSystem(const ParticleMaterial* const material, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan)
		: m_material(material), m_particlePerSecond(particlesPerSecond), m_speed(speed), m_gravityComplient(gravityComplient), m_lifeSpan(lifeSpan)
	{
	}

	void ParticleSystem::GenerateParticles(const Math::Vec3& center)
	{
		float delta = Window::delta;
		float particlesToCreate = m_particlePerSecond * (delta * 10);
		int count = static_cast<int>(floorf(particlesToCreate));
		float partialParticle = static_cast<int>(particlesToCreate) % 1;
		for (int i = 0; i < count; i++)
		{
			EmitParticle(center);
		}
		if (Math::Random() < partialParticle)
		{
			EmitParticle(center);
		}
	}
	
	void ParticleSystem::EmitParticle(const Math::Vec3& center)
	{
		float xDir = Math::Random() * 2.0f - 1.0f;
		float zDir = Math::Random() * 2.0f - 1.0f;
		Math::Vec3 velocity(xDir, 1, zDir);
		velocity.Normalize();
		velocity *= m_speed;

		Particle particle(m_material, center, velocity, m_gravityComplient, m_lifeSpan, 0, 1);
	}
}