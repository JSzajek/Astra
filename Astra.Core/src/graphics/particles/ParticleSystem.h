#pragma once

#include "../../math/Maths.h"
#include "Particle.h"
#include "../gizmos/Gizmo.h"

namespace Astra::Graphics
{
	class ParticleSystem
	{
	private:
	#if _DEBUG
		const Gizmo* m_gizmo;
	#endif
		float m_particlePerSecond;
		float m_speed;
		float m_gravityComplient;
		float m_lifeSpan;
		const ParticleMaterial* const m_material;
		const Math::Vec3* m_center;
	public:
		ParticleSystem(const ParticleMaterial* const material, const Math::Vec3* center, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan);
	#if _DEBUG
		~ParticleSystem();
		inline const Gizmo* const GetGizmo() const { return m_gizmo; }
	#endif
		
		void GenerateParticles() const;
	private:
		void EmitParticle() const;
	};
}