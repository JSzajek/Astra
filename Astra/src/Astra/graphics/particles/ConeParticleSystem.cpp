#include "astra_pch.h"

#include "ConeParticleSystem.h"
#include "ParticleController.h"

namespace Astra::Graphics
{
	ConeParticleSystem::ConeParticleSystem(const ParticleMaterial* const material, const Math::Vec3* center, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan, float scale, bool additive)
		: ParticleSystem(material, center, particlesPerSecond, speed, gravityComplient, lifeSpan, additive), m_scale(scale),
			m_speedError(0), m_lifeError(0), m_scaleError(0), m_randomRotation(false), m_direction(Math::Vec3::Zero), m_directionDeviation(0)
	{
	}

	void ConeParticleSystem::SetDirection(const Math::Vec3& direction, float deviation)
	{
		m_direction = direction;
		m_directionDeviation = deviation * PI;
	}

	void ConeParticleSystem::EmitParticle() const
	{
		Math::Vec3 velocity = m_direction.Length() != 0 ? GenerateRandomUnitVectorInCone(m_direction, m_directionDeviation) 
																: GenerateRandomUnitVector();
		velocity.Normalize();
		velocity *= GenerateValue(m_speed, m_speedError);
		float lifespan = GenerateValue(m_lifeSpan, m_lifeError);
		float scale = GenerateValue(m_scale, m_scaleError);
		float rotation = GenerateRotation();
		
		/*float lifespan = m_lifeSpan;
		float scale = m_scale;
		float rotation = 0;*/

		auto* particle = ParticleController::GetParticle();
		if (particle == NULL)
		{
			particle = new Particle(m_material, *m_center, velocity, m_gravityComplient, lifespan, rotation, scale, m_additive);
		}
		else
		{
			(*particle)(m_material, *m_center, velocity, m_gravityComplient, lifespan, rotation, scale, m_additive);
		}
		ParticleController::AddParticle(particle);
	}
	
	float ConeParticleSystem::GenerateValue(float average, float errorMargin) const
	{
		return average + ((Math::Random() - 0.5f) * 2.0f * errorMargin);
	}
	
	float ConeParticleSystem::GenerateRotation() const
	{
		return m_randomRotation ? Math::Random() * 360.0f : 0;
	}
	
	const Math::Vec3 ConeParticleSystem::GenerateRandomUnitVectorInCone(const Math::Vec3& coneDir, float angle) const
	{
		float cosAngle = cosf(angle);
		float theta = Math::Random() * 2.0f * PI;
		float z = cosAngle + (Math::Random() * (1.0f - cosAngle));
		float rootOneMinusZSquared = sqrtf(1.0f - z * z);
		float x = rootOneMinusZSquared * cosf(theta);
		float y = rootOneMinusZSquared * sinf(theta);
		Math::Vec3 direction(x, y, z);
		if (coneDir.x != 0 || coneDir.y != 0 || (coneDir.z != 1 && coneDir.z != -1))
		{
			Math::Vec3 rotAxis = coneDir.Cross(Math::Vec3::Z_Axis);
			rotAxis.Normalize();
			float rotAngle = acosf(coneDir.Dot(Math::Vec3::Z_Axis));
			Math::Mat4 rotMat(1);
			rotMat = rotMat.Rotate(-Math::ToDegrees(rotAngle), rotAxis);
			direction = rotMat * direction;
		}
		else if (coneDir.z == -1)
		{
			direction.z *= -1;
		}
		return direction;
	}
	
	const Math::Vec3 ConeParticleSystem::GenerateRandomUnitVector() const
	{
		float theta = Math::Random() * 2.0f * PI;
		float z = (Math::Random() * 2.0f) - 1.0f;
		float rootOneMinusZSquared = sqrtf(1.0f - (z * z));
		float x = rootOneMinusZSquared * cosf(theta);
		float y = rootOneMinusZSquared * sinf(theta);
		return Math::Vec3(x, y, z);
	}
}