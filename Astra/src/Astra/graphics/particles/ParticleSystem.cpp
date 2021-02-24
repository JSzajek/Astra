#include "astra_pch.h"

#include "ParticleSystem.h"
#include "ParticleController.h"

namespace Astra::Graphics
{
	ParticleSystem::ParticleSystem()
		: LayerEntity(), m_center(), m_particlePerSecond(0), m_speed(0), m_gravityComplient(0), m_lifeSpan(0), m_additive(0)
	{
	#if ASTRA_DEBUG
		m_gizmo = Gizmo(RESOURCE("res/textures/Emitter.png"), Math::Vec3::Zero, 3);
	#endif
	}

	ParticleSystem::ParticleSystem(const char* const name, const ParticleMaterial& material, const Math::Vec3& center, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan, bool additive)
		: LayerEntity(name), m_material(material), m_center(center), m_particlePerSecond(particlesPerSecond), m_speed(speed), 
			m_gravityComplient(gravityComplient), m_lifeSpan(lifeSpan), m_additive(additive), m_scale(0), m_speedError(0), 
			m_lifeError(0), m_scaleError(0), m_randomRotation(false), m_direction(Math::Vec3::Zero), m_directionDeviation(0)
	{
	#if ASTRA_DEBUG
		m_gizmo = Gizmo(RESOURCE("res/textures/Emitter.png"), center, 3);
	#endif
	}

	ParticleSystem::ParticleSystem(const ParticleMaterial& material, const Math::Vec3& center, float particlesPerSecond, float speed, float gravityComplient, float lifeSpan, bool additive)
		: LayerEntity(), m_material(material), m_center(center), m_particlePerSecond(particlesPerSecond), m_speed(speed),
			m_gravityComplient(gravityComplient), m_lifeSpan(lifeSpan), m_additive(additive)
	{
	#if ASTRA_DEBUG
		m_gizmo = Gizmo(RESOURCE("res/textures/Emitter.png"), center, 3);
	#endif
	}

	ParticleSystem::ParticleSystem(const ParticleSystem& other)
		: m_material(other.m_material), m_center(other.m_center), m_particlePerSecond(other.m_particlePerSecond), 
			m_speed(other.m_speed), m_gravityComplient(other.m_gravityComplient), m_lifeSpan(other.m_lifeSpan), m_additive(other.m_additive),
			m_gizmo(other.m_gizmo), m_speedError(other.m_speedError), m_lifeError(other.m_lifeError), m_scaleError(other.m_scaleError),
			m_randomRotation(other.m_randomRotation), m_direction(other.m_direction), m_directionDeviation(other.m_directionDeviation)
	{
	}

	ParticleSystem::~ParticleSystem()
	{
		//RESOURCE_UNLOAD(m_material);
#if ASTRA_DEBUG
		//delete m_gizmo;
#endif
	}

	void ParticleSystem::GenerateParticles(float delta) const
	{
		float particlesToCreate = m_particlePerSecond * (delta * 10);
		int count = static_cast<int>(floorf(particlesToCreate));
		int partialParticle = static_cast<int>(particlesToCreate) % 1; // Revisit this
		for (int i = 0; i < count; i++)
		{
			EmitParticle();
		}
		if (Math::Random() < partialParticle)
		{
			EmitParticle();
		}
	}

	void ParticleSystem::SetDirection(const Math::Vec3& direction, float deviation)
	{
		m_direction = direction;
		m_directionDeviation = deviation * PI;
	}

	void ParticleSystem::EmitParticle() const
	{
		Math::Vec3 velocity = m_direction.Length() != 0 ? GenerateRandomUnitVectorInCone(m_direction, m_directionDeviation)
			: GenerateRandomUnitVector();
		velocity.Normalize();
		velocity *= GenerateValue(m_speed, m_speedError);
		float lifespan = GenerateValue(m_lifeSpan, m_lifeError);
		float scale = GenerateValue(m_scale, m_scaleError);
		float rotation = GenerateRotation();

		auto* particle = ParticleController::GetParticle();
		if (particle == NULL)
		{
			particle = new Particle(m_material, m_center, velocity, m_gravityComplient, lifespan, rotation, scale, m_additive);
		}
		else
		{
			(*particle)(m_material, m_center, velocity, m_gravityComplient, lifespan, rotation, scale, m_additive);
		}
		ParticleController::AddParticle(particle);
	}

	float ParticleSystem::GenerateValue(float average, float errorMargin) const
	{
		return average + ((Math::Random() - 0.5f) * 2.0f * errorMargin);
	}

	float ParticleSystem::GenerateRotation() const
	{
		return m_randomRotation ? Math::Random() * 360.0f : 0;
	}

	const Math::Vec3 ParticleSystem::GenerateRandomUnitVectorInCone(const Math::Vec3& coneDir, float angle) const
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

	const Math::Vec3 ParticleSystem::GenerateRandomUnitVector() const
	{
		float theta = Math::Random() * 2.0f * PI;
		float z = (Math::Random() * 2.0f) - 1.0f;
		float rootOneMinusZSquared = sqrtf(1.0f - (z * z));
		float x = rootOneMinusZSquared * cosf(theta);
		float y = rootOneMinusZSquared * sinf(theta);
		return Math::Vec3(x, y, z);
	}
}