//#pragma once
//
//#include "Astra/math/Maths.h"
//#include "ParticleSystem.h"
//#include "Particle.h"
//
//namespace Astra::Graphics
//{
//	class ConeParticleSystem : public ParticleSystem
//	{
//	private:
//		
//	public:
//		ConeParticleSystem(const char* const name, const ParticleMaterial* const material, const Math::Vec3& center, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan, float scale, bool additive = false);
//		ConeParticleSystem(const ParticleMaterial* const material, const Math::Vec3& center, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan, float scale, bool additive = false);
//		ConeParticleSystem(const ConeParticleSystem& other);
//	public:
//		virtual void Free() override;
//		inline virtual std::string ToString() const override { return !Name.length() ? ("Cone_Particle_System&" + std::to_string(m_uid)) : Name; }
//	public:
//		
//	private:
//		void EmitParticle() const override;
//		float GenerateValue(float average, float errorMargin) const;
//		float GenerateRotation() const;
//		const Math::Vec3 GenerateRandomUnitVectorInCone(const Math::Vec3& coneDir, float angle) const;
//		const Math::Vec3 GenerateRandomUnitVector() const;
//	};
//}