#pragma once

#include "../../math/Vec3.h"
#include "../materials/ParticleMaterial.h"

namespace Astra::Graphics
{
	#define		GRAVITY			-50

	struct Particle
	{
	public:
		Math::Vec3 Position;
		float Rotation;
		float Scale;
		const ParticleMaterial* const Material;
	private:
		Math::Vec3 m_velocity;
		float m_gravity;
		float m_lifeSpan;
		float m_elapsedTime;
	public:
		Particle(const ParticleMaterial* const material, const Math::Vec3& position, const Math::Vec3& velocity, float gravity, float lifeSpan, float rotation, float scale);
		bool Update();
	};
}