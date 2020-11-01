#pragma once

#include "Renderer.h"
#include "../entities/Camera.h"
#include "../buffers/WaterFrameBuffer.h"
#include "../buffers/Texture.h"
#include "../entities/terrains/WaterTile.h"
#include "../entities/Light.h"
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
		const Light* m_light;
		float m_near, m_far;
	public:
		WaterRenderer(Camera* camera, float near, float far);
		
		void SetShader(Shader* shader) override;
		inline void Clear() override 
		{ 
			m_waterTiles.clear(); 
			m_light = NULL;
		}

		void Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane = DefaultClipPlane) override;
		void AddTile(const WaterTile& tile) { m_waterTiles.push_back(tile); }
		inline void SetCamera(Camera* camera) { m_camera = camera; }
		inline void SetFrameBuffer(WaterFrameBuffer* frameBuffer) { m_buffer = frameBuffer; }
		inline void AddLight(const Light* light) 
		{
			if (m_light == NULL)
			{
				m_light = light; 
			}
		}
	protected:
		void UnbindVertexArray() override;
	private:
		void PrepareRender();
		void PrepareTile(const WaterTile& tile);
	};
}