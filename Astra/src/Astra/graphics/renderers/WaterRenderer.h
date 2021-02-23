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
		//std::vector<const WaterTile*> m_waterTiles;
		const Math::Mat4* m_toShadowSpaceMatrix;
		float m_near, m_far;
	#if ASTRA_DEBUG
		bool m_wireframe;
	#endif
	public:
		WaterRenderer(float _near, float _far);
		~WaterRenderer();
	#if ASTRA_DEBUG
		void SetWireframe(bool wireframe) { m_wireframe = wireframe; };
	#endif
		void SetShader(Shader* shader) override;
		inline void SetShadowMatrix(const Math::Mat4* shadowMatrix) { m_toShadowSpaceMatrix = shadowMatrix; }

		void Draw(float delta, 
				  const std::unordered_map<unsigned int, Graphics::WaterTile>& tiles,
				  const Math::Mat4* viewMatrix = NULL, 
				  const Math::Vec4& inverseViewVector = NULL, 
				  const Math::Vec4& clipPlane = DefaultClipPlane);

		inline void SetFrameBuffer(WaterFrameBuffer* frameBuffer) { m_buffer = frameBuffer; }
		
		inline void AddLight(Light* light) { AddLight(0, light); }
		void AddLight(unsigned int index, Light* light);
		
		void SetReflection(bool enabled);
	protected:
		void UnbindVertexArray() override;
	private:
		void PrepareRender();
		void PrepareTile(const WaterTile* tile);
	};
}