#include "Particle.h"

#include "ParticleController.h"
#include "../Window.h"

namespace Astra::Graphics
{
	Particle::Particle(const ParticleMaterial* const material, const Math::Vec3& position, const Math::Vec3& velocity, float gravity, float lifeSpan, float rotation, float scale)
		: Material(material), Position(position), m_velocity(velocity), m_gravity(gravity), m_lifeSpan(lifeSpan), m_elapsedTime(0), Rotation(rotation), Scale(scale)
	{
		ParticleController::AddParticle(*this);
	}

	bool Particle::Update()
	{
		float delta = Window::delta;
		m_velocity.y += GRAVITY * (float)m_gravity * delta;
		Math::Vec3 change(m_velocity);
		change *= delta;
		Position += change;
		m_elapsedTime += delta;
		return m_elapsedTime < m_lifeSpan;
	}
}