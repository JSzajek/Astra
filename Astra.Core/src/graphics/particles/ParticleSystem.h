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
		const Math::Vec3* m_center;
	public:
		ParticleSystem(const ParticleMaterial* const material, const Math::Vec3* center, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan);
		
		void GenerateParticles() const;
	private:
		void EmitParticle() const;
	};
}