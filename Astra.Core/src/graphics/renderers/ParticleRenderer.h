#pragma once

#include "Renderer.h"

#include "../particles/Particle.h"
#include "../shaders/ParticleShader.h"

namespace Astra::Graphics
{
	class ParticleRenderer : public Renderer
	{
	private:
		const VertexArray* m_defaultQuad;
		std::vector<Particle> m_particles;
		Math::Mat4 m_viewMatrix;
	public:
		ParticleRenderer(ParticleShader* shader);
		~ParticleRenderer();
		inline void AddParticle(const Particle& particle) { m_particles.push_back(particle); }
		inline std::vector<Particle>& GetParticles() { return m_particles; }
		void Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane = DefaultClipPlane) override;
	private:
		void UpdateModelViewMatrix(const Math::Vec3& position, float rotation, float scale);
	};
}