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
		std::vector<const WaterTile*> m_waterTiles;
		std::vector<const Light*> m_lights;
		const Light* m_directionalLight;
		const Math::Vec3* m_fogColor;
		Math::Mat4 m_toShadowSpaceMatrix;
		float m_near, m_far;
	public:
		WaterRenderer(Camera* camera, const Math::Vec3* fogColor, float near, float far);
		
		void SetShader(Shader* shader) override;
		void Clear() override;
		inline void SetShadowMatrix(const Math::Mat4& shadowMatrix) { m_toShadowSpaceMatrix = shadowMatrix; }

		void Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane = DefaultClipPlane) override;
		void AddTile(const WaterTile* tile) { m_waterTiles.emplace_back(tile); }
		inline void SetCamera(Camera* camera) { m_camera = camera; }
		inline void SetFrameBuffer(WaterFrameBuffer* frameBuffer) { m_buffer = frameBuffer; }
		void AddLight(Light* light);
		void UpdateLight(const Light* light);
	protected:
		void UnbindVertexArray() override;
	private:
		void PrepareRender();
		void PrepareTile(const WaterTile* tile);
	};
}