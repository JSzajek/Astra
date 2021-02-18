#pragma once

#include <unordered_map>

#include "Renderer.h"

#include "../particles/Particle.h"
#include "../shaders/ParticleShader.h"

namespace Astra::Graphics
{
	struct ParticleInfo
	{
		Math::Mat4 modelview;
		Math::Vec4 texOffsets;
		Math::Vec3 animInfo;
	};

	#define MAX_TEXTURE_SLOTS		16
	#define MAX_PARTICLES			10000

	#define NUM_OF_TYPES			2	

	class ParticleRenderer : public Renderer
	{
	private:
		unsigned int m_defaultVAO;
		unsigned int m_defaultVBO;

		std::vector<Particle*> m_particles[NUM_OF_TYPES];
		const Math::Mat4* m_viewMatrix;
		Math::Mat4* m_modelViewMatrix;
	public:
		ParticleRenderer(ParticleShader* shader);
		~ParticleRenderer();
		
		inline std::vector<Particle*>& GetParticles(unsigned int index) { return m_particles[index]; }
		
		//void Clear() override;
		void AddParticle(Particle* particle);
		void Draw(float delta = 0, const Math::Mat4* viewMatrix = NULL, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane);
	private:
		void Flush(std::unordered_map<unsigned int, unsigned int>& mapping, size_t& offset);
		void CreateInstancedBuffer(unsigned int* id, size_t size, bool gen = false);
		unsigned int CreateDefaultQuadVao();
	};
}