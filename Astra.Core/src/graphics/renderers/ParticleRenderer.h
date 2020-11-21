#pragma once

#include <unordered_map>

#include "Renderer.h"

#include "../particles/Particle.h"
#include "../shaders/ParticleShader.h"

namespace Astra::Graphics
{
	struct ParticleBuffer
	{
		unsigned int VAO;
		unsigned int VBO;
		size_t MaxSize;

		ParticleBuffer()
			: VAO(0), VBO(0), MaxSize(0)
		{
		}

		ParticleBuffer(unsigned int vao, unsigned int vbo, size_t size)
			: VAO(vao), VBO(vbo), MaxSize(size)
		{
		}
	};

	#define MAX_PARTICLES 10000

	class ParticleRenderer : public Renderer
	{
	private:
		std::unordered_map<unsigned int, std::vector<Particle*>> m_particles;
		std::unordered_map<unsigned int, ParticleBuffer> m_buffers;
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
		void UpdateParticleData();
		void CreateInstancedBuffer(unsigned int* id, size_t size);
		unsigned int CreateDefaultQuadVao();
	};
}