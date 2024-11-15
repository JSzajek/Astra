#include "astra_pch.h"

#include "Particle.h"
#include "ParticleController.h"

namespace Astra::Graphics
{
	Particle::Particle(const ParticleMaterial& material, const Math::Vec3& position, const Math::Vec3& velocity, float gravity, float lifeSpan, float rotation, float scale, bool additive)
		: Material(material), Position(position), m_velocity(velocity), m_gravity(gravity), m_lifeSpan(lifeSpan), m_elapsedTime(0), Rotation(rotation), Scale(scale),
			m_texOffset1(0), m_texOffset2(0), m_blendFactor(0), m_distance(0), m_additive(additive)
	{
	}

	Particle::Particle(const Particle& other)
		: Material(other.Material), Position(other.Position), m_velocity(other.m_velocity), m_gravity(other.m_gravity), 
			m_lifeSpan(other.m_lifeSpan), m_elapsedTime(other.m_elapsedTime), Rotation(other.Rotation), Scale(other.Scale),
				m_texOffset1(other.m_texOffset1), m_texOffset2(other.m_texOffset2), m_blendFactor(other.m_blendFactor), m_distance(other.m_distance), m_additive(other.m_additive)
	{
	}

	void Particle::operator()(const ParticleMaterial& material, const Math::Vec3& position, const Math::Vec3& velocity, float gravity, float lifeSpan, float rotation, float scale, bool additive)
	{
		Material = material;
		Position = position;
		Rotation = rotation;
		Scale = scale;
		m_velocity = velocity;
		m_gravity = gravity; 
		m_additive = additive;
		m_lifeSpan = lifeSpan; 
		m_elapsedTime = m_blendFactor = m_distance = 0;
		m_texOffset1 = m_texOffset2 = Math::Vec2::Zero;
	}

	bool Particle::Update(float delta, const Math::Vec3& cameraPosition)
	{
		m_velocity.y += GRAVITY * (float)m_gravity * delta;
		auto change = m_velocity * delta;
		Position += change;
		m_elapsedTime += delta;
		m_distance = (cameraPosition - Position).LengthSquared();
		UpdateTextureCoordInfo();
		return m_elapsedTime < m_lifeSpan;
	}

	void Particle::UpdateTextureCoordInfo()
	{
		float lifeFactor = m_elapsedTime / m_lifeSpan;
		int stageCount = Material.GetRowCount() * Material.GetRowCount();
		float atlasProgression = lifeFactor * stageCount;
		int index1 = static_cast<int>(floorf(atlasProgression));
		int index2 = index1 < stageCount - 1 ? index1 + 1 : index1;
		m_blendFactor = (float)Math::DecimalNumbers(atlasProgression, 1) / 10;
		SetTextureOffset(&m_texOffset1, index1);
		SetTextureOffset(&m_texOffset2, index2);
	}

	void Particle::SetTextureOffset(Math::Vec2* offset, int index)
	{
		int column = index % Material.GetRowCount();
		int row = index / Material.GetRowCount();
		offset->x = (float)column / (float)Material.GetRowCount();
		offset->y = (float)row / (float)Material.GetRowCount();
	}
}