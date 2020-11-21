#pragma once

#include <stack>
#include "../renderers/ParticleRenderer.h"

namespace Astra::Graphics
{
	class ParticleController
	{
	private:
		ParticleShader* m_particleShader;
		ParticleRenderer* m_particleRenderer;
		std::stack<Particle*> m_leftovers;
	public:
		ParticleController(const ParticleController&) = delete;
		void operator=(const ParticleController&) = delete;

		static ParticleController& Get()
		{
			static ParticleController instance;
			return instance;
		}

		static void AddParticle(Particle* particle)
		{
			Get().AddParticleImpl(particle);
		}

		static Particle* GetParticle()
		{
			return Get().GetParticleImpl();
		}

		static void UpdateProjectionMatrix(const Math::Mat4* projectionMatrix)
		{
			Get().UpdateProjectionMatrixImpl(projectionMatrix);
		}

		static void Update(const Math::Vec3& cameraPosition)
		{
			Get().UpdateImpl(cameraPosition);
		}

		static void Render(const Math::Mat4* viewMatrix)
		{
			Get().RenderImpl(viewMatrix);
		}

		static void Clear()
		{
			Get().ClearImpl();
		}

	private:
		ParticleController();
		~ParticleController();

		void UpdateProjectionMatrixImpl(const Math::Mat4* projectionMatrix);
		void UpdateImpl(const Math::Vec3& cameraPosition);
		void RenderImpl(const Math::Mat4* viewMatrix);
		void AddParticleImpl(Particle* particle);
		inline Particle* GetParticleImpl() 
		{
			if (m_leftovers.size() == 0) { return NULL; }
			auto* left = m_leftovers.top();
			m_leftovers.pop();
			return left; 
		}
		void ClearImpl();
	private:
		void InsertionSort(std::vector<Particle*>& particles);
	};
}