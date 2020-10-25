#pragma once

#include "../renderers/ParticleRenderer.h"

namespace Astra::Graphics
{
	class ParticleController
	{
	private:
		ParticleShader* m_particleShader;
		ParticleRenderer* m_particleRenderer;
	public:
		ParticleController(const ParticleController&) = delete;
		void operator=(const ParticleController&) = delete;

		static ParticleController& Get()
		{
			static ParticleController instance;
			return instance;
		}

		static void AddParticle(const Particle& particle)
		{
			Get().AddParticleImpl(particle);
		}

		static void UpdateProjectionMatrix(const Math::Mat4& projectionMatrix)
		{
			Get().UpdateProjectionMatrixImpl(projectionMatrix);
		}

		static void Update()
		{
			Get().UpdateImpl();
		}

		static void Render(const Math::Mat4& viewMatrix)
		{
			Get().RenderImpl(viewMatrix);
		}

	private:
		ParticleController();
		~ParticleController();

		void UpdateProjectionMatrixImpl(const Math::Mat4& projectionMatrix);
		void UpdateImpl();
		void RenderImpl(const Math::Mat4& viewMatrix);
		void AddParticleImpl(const Particle& particle);
	};
}