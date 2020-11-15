#include "ConeParticleSystem.h"

#include "../Window.h"

namespace Astra::Graphics
{
	ConeParticleSystem::ConeParticleSystem(const ParticleMaterial* const material, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan, float scale)
		: m_material(material), m_particlePerSecond(particlesPerSecond), m_speed(speed), m_gravityComplient(gravityComplient), m_lifeSpan(lifeSpan), m_scale(scale),
			m_speedError(0), m_lifeError(0), m_scaleError(0), m_randomRotation(false), m_direction(Math::Vec3::Zero), m_directionDeviation(0)
	{
	}

	void ConeParticleSystem::SetDirection(const Math::Vec3& direction, float deviation)
	{
		m_direction = direction;
		m_directionDeviation = deviation * PI;
	}

	void ConeParticleSystem::GenerateParticles(const Math::Vec3& center)
	{
		float delta = Window::delta;
		float particlesToCreate = m_particlePerSecond * (delta * 10);
		int count = static_cast<int>(floorf(particlesToCreate));
		int partialParticle = static_cast<int>(particlesToCreate) % 1; // Revisit this
		for (int i = 0; i < count; i++)
		{
			EmitParticle(center);
		}
		if (Math::Random() < partialParticle)
		{
			EmitParticle(center);
		}
	}

	void ConeParticleSystem::EmitParticle(const Math::Vec3& center)
	{
		Math::Vec3 velocity = m_direction.Magnitude() != 0 ? GenerateRandomUnitVectorInCone(m_direction, m_directionDeviation) 
															: GenerateRandomUnitVector();
		velocity.Normalize();
		velocity *= GenerateValue(m_speed, m_speedError);
		float scale = GenerateValue(m_scale, m_scaleError);
		float lifeSpan = GenerateValue(m_lifeSpan, m_lifeError);

		Particle particle(m_material, center, velocity, m_gravityComplient, lifeSpan, GenerateRotation(), scale);
	}
	
	float ConeParticleSystem::GenerateValue(float average, float errorMargin)
	{
		return average + ((Math::Random() - 0.5f) * 2.0f * errorMargin);
	}
	
	float ConeParticleSystem::GenerateRotation()
	{
		return m_randomRotation ? Math::Random() * 360.0f : 0;
	}
	
	const Math::Vec3 ConeParticleSystem::GenerateRandomUnitVectorInCone(const Math::Vec3& coneDir, float angle)
	{
		float cosAngle = cos(angle);
		float theta = Math::Random() * 2.0f * PI;
		float z = cosAngle + (Math::Random() * (1 - cosAngle));
		float rootOneMinusZSquared = sqrtf(1 - z * z);
		float x = rootOneMinusZSquared * cos(theta);
		float y = rootOneMinusZSquared * sin(theta);
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
	
	const Math::Vec3 ConeParticleSystem::GenerateRandomUnitVector()
	{
		float theta = Math::Random() * 2.0f * PI;
		float z = (Math::Random() * 2) - 1;
		float rootOneMinusZSquared = sqrtf(1 - (z * z));
		float x = rootOneMinusZSquared * cos(theta);
		float y = rootOneMinusZSquared * sin(theta);
		return Math::Vec3(x, y, z);
	}
}