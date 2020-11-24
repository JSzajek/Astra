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
		WaterFrameBuffer* m_buffer;
		std::vector<const WaterTile*> m_waterTiles;
		std::vector<const Light*> m_lights;
		const Light* m_directionalLight;
		const Math::Mat4* m_toShadowSpaceMatrix;
		float m_near, m_far;
	public:
		WaterRenderer(float near, float far);
		
		void SetShader(Shader* shader) override;
		void Clear() override;
		inline void SetShadowMatrix(const Math::Mat4* shadowMatrix) { m_toShadowSpaceMatrix = shadowMatrix; }

		void Draw(float delta = 0, const Math::Mat4* viewMatrix = NULL, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane) override;
		void AddTile(const WaterTile* tile) { m_waterTiles.emplace_back(tile); }
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