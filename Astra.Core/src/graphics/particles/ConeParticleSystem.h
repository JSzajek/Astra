#pragma once

#include "../../math/Maths.h"
#include "ParticleSystem.h"
#include "Particle.h"

namespace Astra::Graphics
{
	class ConeParticleSystem : public ParticleSystem
	{
	private:
		float m_scale;
		float m_speedError;
		float m_lifeError;
		float m_scaleError;
		bool m_randomRotation;
		Math::Vec3 m_direction;
		float m_directionDeviation;
	public:
		ConeParticleSystem(const ParticleMaterial* const material, const Math::Vec3* center, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan, float scale, bool additive = false);
		
		void SetDirection(const Math::Vec3& direction, float deviation);
		inline void SetRandomRotation(bool randomize) { m_randomRotation = randomize; }
		inline void SetSpeedError(float error) { m_speedError = error; }
		inline void SetLifeError(float error) { m_lifeError = error; }
		inline void SetScaleError(float error) { m_scaleError = error; }
	private:
		void EmitParticle() const override;
		float GenerateValue(float average, float errorMargin) const;
		float GenerateRotation() const;
		const Math::Vec3 GenerateRandomUnitVectorInCone(const Math::Vec3& coneDir, float angle) const;
		const Math::Vec3 GenerateRandomUnitVector() const;
	};
}