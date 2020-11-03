#include "ParticleSystem.h"

#include "../Window.h"

namespace Astra::Graphics
{
	ParticleSystem::ParticleSystem(const ParticleMaterial* const material, const Math::Vec3* center, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan)
		: m_material(material), m_center(center), m_particlePerSecond(particlesPerSecond), m_speed(speed), m_gravityComplient(gravityComplient), m_lifeSpan(lifeSpan)
	{
	#if _DEBUG
		m_gizmo = new Gizmo("../Astra.Core/src/resources/textures/Emitter.png", *center, 3);
	#endif
	}

	void ParticleSystem::GenerateParticles() const
	{
		float delta = Window::delta;
		float particlesToCreate = m_particlePerSecond * (delta * 10);
		int count = static_cast<int>(floorf(particlesToCreate));
		float partialParticle = static_cast<int>(particlesToCreate) % 1;
		for (int i = 0; i < count; i++)
		{
			EmitParticle();
		}
		if (Math::Random() < partialParticle)
		{
			EmitParticle();
		}
	}
	
	void ParticleSystem::EmitParticle() const
	{
		float xDir = Math::Random() * 2.0f - 1.0f;
		float zDir = Math::Random() * 2.0f - 1.0f;
		Math::Vec3 velocity(xDir, 1, zDir);
		velocity.Normalize();
		velocity *= m_speed;

		auto* particle = new Particle(m_material, *m_center, velocity, m_gravityComplient, m_lifeSpan, 0, 1);
	}
}