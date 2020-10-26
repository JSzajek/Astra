#pragma once

#include "../../math/Maths.h"
#include "Particle.h"

namespace Astra::Graphics
{
	class ParticleSystem
	{
	private:
		float m_particlePerSecond;
		float m_speed;
		float m_gravityComplient;
		float m_lifeSpan;
		const ParticleMaterial* const m_material;
	public:
		ParticleSystem(const ParticleMaterial* const material, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan);
		
		void GenerateParticles(const Math::Vec3& center);
	private:
		void EmitParticle(const Math::Vec3& center);
	};
}