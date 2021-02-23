//#include "astra_pch.h"
//
//#include "ConeParticleSystem.h"
//#include "ParticleController.h"
//
//namespace Astra::Graphics
//{
//	ConeParticleSystem::ConeParticleSystem(const char* const name, const ParticleMaterial* const material, const Math::Vec3& center, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan, float scale, bool additive)
//		: ParticleSystem(name, material, center, particlesPerSecond, speed, gravityComplient, lifeSpan, additive), 
//	{
//	}
//
//	ConeParticleSystem::ConeParticleSystem(const ParticleMaterial* const material, const Math::Vec3& center, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan, float scale, bool additive)
//		: ParticleSystem(material, center, particlesPerSecond, speed, gravityComplient, lifeSpan, additive), m_scale(scale),
//			m_speedError(0), m_lifeError(0), m_scaleError(0), m_randomRotation(false), m_direction(Math::Vec3::Zero), m_directionDeviation(0)
//	{
//	}
//
//	ConeParticleSystem::ConeParticleSystem(const ConeParticleSystem& other)
//		: ParticleSystem(other), 
//	{
//	}
//
//	void ConeParticleSystem::Free()
//	{
//	}
//
//	
//}