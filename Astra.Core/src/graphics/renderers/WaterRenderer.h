#pragma once

#include "Renderer.h"
#include "../entities/Camera.h"
#include "../buffers/WaterFrameBuffer.h"
#include "../entities/WaterTile.h"
#include "../shaders/WaterShader.h"

namespace Astra::Graphics
{
	class WaterRenderer : public Renderer
	{
	private:
		const VertexArray* m_defaultQuad;
		Camera* m_camera;
		WaterFrameBuffer* m_buffer;
		std::vector<WaterTile> m_waterTiles;
	public:
		WaterRenderer(Shader* shader, Camera* camera);
		void Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane = DefaultClipPlane) override;
		void AddTile(const WaterTile& tile) { m_waterTiles.push_back(tile); }
		inline void SetCamera(Camera* camera) { m_camera = camera; }
		inline void SetFrameBuffer(WaterFrameBuffer* frameBuffer) { m_buffer = frameBuffer; }
	};
}