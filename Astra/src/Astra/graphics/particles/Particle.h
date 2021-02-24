#pragma once

#include "Astra/math/Vec3.h"
#include "Astra/graphics/materials/ParticleMaterial.h"

namespace Astra::Graphics
{
	#define		GRAVITY			-50

	struct Particle
	{
	public:
		Math::Vec3 Position;
		float Rotation;
		float Scale;
		ParticleMaterial Material;
	private:
		Math::Vec3 m_velocity;
		float m_gravity;
		float m_lifeSpan;
		float m_elapsedTime;
		
		Math::Vec2 m_texOffset1;
		Math::Vec2 m_texOffset2;
		float m_blendFactor;
		float m_distance;
		bool m_additive;
	public:
		Particle(const ParticleMaterial& material, const Math::Vec3& position, const Math::Vec3& velocity, float gravity, float lifeSpan, float rotation, float scale, bool additive = false);
		Particle(const Particle& other);

		void operator()(const ParticleMaterial& material, const Math::Vec3& position, const Math::Vec3& velocity, float gravity, float lifeSpan, float rotation, float scale, bool additive = false);

		inline const Math::Vec2& GetTexOffset1() const { return m_texOffset1; }
		inline const Math::Vec2& GetTexOffset2() const { return m_texOffset2; }
		inline const float GetBlendFactor() const { return m_blendFactor; }
		inline const float GetDistance() const { return m_distance; }
		inline bool GetAdditive() const { return m_additive; }
		bool Update(float delta, const Math::Vec3& cameraPosition);
	private:
		void UpdateTextureCoordInfo();
		void SetTextureOffset(Math::Vec2* offset, int index);
	};
}