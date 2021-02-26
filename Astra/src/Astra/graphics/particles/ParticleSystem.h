#pragma once

#include "Particle.h"
#include "Astra/math/Maths.h"
#include "Astra/graphics/LayerEntity.h"
#include "Astra/graphics/gizmos/Gizmo.h"

namespace Astra::Graphics
{
	class ParticleSystem : public LayerEntity
	{
	private:
	#if ASTRA_DEBUG
		Gizmo m_gizmo;
	#endif
	protected:
		ParticleMaterial m_material;
		Math::Vec3 m_center;
		float m_particlePerSecond;
		float m_speed;
		float m_gravityComplient;
		float m_lifeSpan;
		bool m_additive;
		float m_scale;
		float m_speedError;
		float m_lifeError;
		float m_scaleError;
		bool m_randomRotation;
		Math::Vec3 m_direction;
		float m_directionDeviation;
	public:
		ParticleSystem();
		ParticleSystem(const char* const name, const ParticleMaterial& material, const Math::Vec3& center, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan, bool additive = false);
		ParticleSystem(const ParticleMaterial& material, const Math::Vec3& center, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan, bool additive = false);
		ParticleSystem(const ParticleSystem& other);
		~ParticleSystem();
	#if ASTRA_DEBUG
		inline const Gizmo& GetGizmo() const { return m_gizmo; }
	#endif
		inline Math::Vec3* GetCenter() { return &m_center; }

		void GenerateParticles(float delta) const;
	public:
		inline virtual std::string ToString() const override { return !Name.length() ? ("Particle_System&" + std::to_string(m_uid)) : Name; }
	public:
		void SetDirection(const Math::Vec3& direction, float deviation);
		inline void SetScale(float scale) { m_scale = scale; }
		inline void SetRandomRotation(bool randomize) { m_randomRotation = randomize; }
		inline void SetSpeedError(float error) { m_speedError = error; }
		inline void SetLifeError(float error) { m_lifeError = error; }
		inline void SetScaleError(float error) { m_scaleError = error; }
	private:
		void EmitParticle() const;
		float GenerateValue(float average, float errorMargin) const;
		float GenerateRotation() const;
		const Math::Vec3 GenerateRandomUnitVectorInCone(const Math::Vec3& coneDir, float angle) const;
		const Math::Vec3 GenerateRandomUnitVector() const;
	};
}