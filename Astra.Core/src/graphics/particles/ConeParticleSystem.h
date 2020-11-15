#pragma once

#include "../../math/Maths.h"
#include "Particle.h"

namespace Astra::Graphics
{
	class ConeParticleSystem
	{
	private:
		float m_particlePerSecond;
		float m_speed;
		float m_gravityComplient;
		float m_lifeSpan;
		float m_scale;
		const ParticleMaterial* const m_material;

		float m_speedError;
		float m_lifeError;
		float m_scaleError;
		bool m_randomRotation;
		Math::Vec3 m_direction;
		float m_directionDeviation;
	public:
		ConeParticleSystem(const ParticleMaterial* const material, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan, float scale);
		
		inline void SetRandomRotation(bool randomize) { m_randomRotation = randomize; }
		inline void SetSpeedError(float error) { m_speedError = error; }
		inline void SetLifeError(float error) { m_lifeError = error; }
		inline void SetScaleError(float error) { m_scaleError = error; }
		
		void SetDirection(const Math::Vec3& direction, float deviation);
		void GenerateParticles(const Math::Vec3& center);
	private:
		void EmitParticle(const Math::Vec3& center);
		float GenerateValue(float average, float errorMargin);
		float GenerateRotation();
		const Math::Vec3 GenerateRandomUnitVectorInCone(const Math::Vec3& coneDir, float angle);
		const Math::Vec3 GenerateRandomUnitVector();
	};
}