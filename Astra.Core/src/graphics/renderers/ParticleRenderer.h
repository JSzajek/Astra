#pragma once

#include <unordered_map>

#include "Renderer.h"

#include "../particles/Particle.h"
#include "../shaders/ParticleShader.h"

namespace Astra::Graphics
{
	class ParticleRenderer : public Renderer
	{
	private:
		const VertexArray* m_defaultQuad;
		std::unordered_map<unsigned int, std::vector<Particle*>> m_particles;
		const Math::Mat4* m_viewMatrix;
		Math::Mat4* m_modelViewMatrix;
	public:
		ParticleRenderer(ParticleShader* shader);
		~ParticleRenderer();
		
		inline void Clear() override { m_particles.clear(); }
		inline std::unordered_map<unsigned int, std::vector<Particle*>>& GetParticles() { return m_particles; }
		
		void AddParticle(Particle* particle);
		void Draw(const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane) override;
	private:
		void UpdateModelViewMatrix(const Math::Vec3& position, float rotation, float scale);
	};
}