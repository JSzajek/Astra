#pragma once

#include <unordered_map>
#include <vector>

#include "Renderer.h"
#include "Astra/graphics/entities/Model.h"
#include "Astra/graphics/buffers/ShadowFrameBuffer.h"
#include "Astra/graphics/shaders/ShadowShader.h"

namespace Astra::Graphics
{
	class ShadowMapRenderer : public Renderer
	{
	private:
		ShadowFrameBuffer* m_buffer;
		Math::Mat4 projectionViewMatrix;
	public:
		ShadowMapRenderer(ShadowShader* shader, ShadowFrameBuffer* buffer, const Math::Mat4& projectionViewMatrix);
		
		inline void SetProjectionViewMatrix(const Math::Mat4& matrix) { projectionViewMatrix = matrix; }

		void Draw(float delta,
				  const std::unordered_map<unsigned int, std::vector<const Model*>>& models,
				  const Math::Mat4* viewMatrix = NULL, 
				  const Math::Vec4& inverseViewVector = NULL, 
				  const Math::Vec4& clipPlane = DefaultClipPlane);
	private:
		void PrepareMesh(const Mesh& mesh);
	};
}