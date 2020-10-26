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
		std::unordered_map<GLuint, std::vector<Particle>> m_particles;
		Math::Mat4 m_viewMatrix;
	public:
		ParticleRenderer(ParticleShader* shader);
		~ParticleRenderer();

		inline std::unordered_map<GLuint, std::vector<Particle>>& GetParticles() { return m_particles; }
		
		void AddParticle(const Particle& particle);
		void Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane = DefaultClipPlane) override;
	private:
		void UpdateModelViewMatrix(const Math::Vec3& position, float rotation, float scale);
	};
}