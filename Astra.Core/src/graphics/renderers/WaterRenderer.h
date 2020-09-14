#pragma once

#include <functional>

#include "Renderer.h"
#include "../entities/Camera.h"
#include "../buffers/WaterFrameBuffer.h"
#include "../entities/WaterTile.h"

namespace Astra::Graphics
{
	class WaterFrameBufferHash {
	public:
		size_t operator()(const WaterFrameBuffer& p) const { return p.id; }
	};
	class WaterRenderer : public Renderer
	{
	private:
		const VertexArray* m_defaultQuad;
		Camera* m_camera;
		Math::Vec4 m_reflectionClipPlane;
		Math::Vec4 m_refractionClipPlane;
		std::unordered_map<WaterFrameBuffer, std::vector<WaterTile>, WaterFrameBufferHash> m_waterTiles;
		std::function<void(const Math::Vec4&)> m_renderCallback;
	public:
		WaterRenderer(Shader* shader, Camera* camera, std::function<void(const Math::Vec4&)> renderCallback);
		void Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane = DefaultClipPlane) override;
		void AddTile(const WaterFrameBuffer& buffer, const WaterTile& tile);
		inline void SetCamera(Camera* camera) { m_camera = camera; }
	};
}