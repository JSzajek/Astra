#pragma once

#include <unordered_map>
#include <vector>

#include "Renderer.h"
#include "../entities/Entity.h"
#include "../buffers/ShadowFrameBuffer.h"
#include "../shaders/ShadowShader.h"

namespace Astra::Graphics
{
	class ShadowMapRenderer : public Renderer
	{
	private:
		std::unordered_map<unsigned int, std::vector<const Entity*>> m_entities;
		ShadowFrameBuffer* m_buffer;
		Math::Mat4 projectionViewMatrix;
	public:
		ShadowMapRenderer(ShadowShader* shader, ShadowFrameBuffer* buffer, const Math::Mat4& projectionViewMatrix);
		
		inline void SetProjectionViewMatrix(const Math::Mat4& matrix) { projectionViewMatrix = matrix; }

		inline void Clear() override { m_entities.clear(); }
		void AddEntity(const Entity* entity);
		void Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane = DefaultClipPlane) override;
	private:
		void PrepareEntity(const Entity* entity);
		void InitializePerEntity(const Entity* entity);
	};
}