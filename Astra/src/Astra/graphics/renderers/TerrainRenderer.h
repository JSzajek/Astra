#pragma once

#include <vector>

#include "Renderer.h"
#include "Astra/graphics/Color.h"

#include "../loaders/Loader.h"
#include "../entities/terrains/Terrain.h"
#include "../entities/Light.h"
#include "../shaders/TerrainShader.h"

namespace Astra::Graphics
{
	class TerrainRenderer : public Renderer
	{
	private:
		const Color* m_fogColor;
		const Math::Mat4* m_toShadowSpaceMatrix;
	#if ASTRA_DEBUG
		bool m_wireframe;
	#endif
	public:
		TerrainRenderer(const Color* fogColor);
		
	#if ASTRA_DEBUG
		void SetWireframe(bool wireframe) { m_wireframe = wireframe; };
	#endif
		void SetShader(Shader* shader) override;

		inline void SetShadowMatrix(const Math::Mat4* shadowMatrix) { m_toShadowSpaceMatrix = shadowMatrix; }

		void Draw(float delta, 
				  const std::unordered_map<unsigned int, std::vector<const Graphics::Terrain*>>& terrains,
				  const Math::Mat4* viewMatrix = NULL, 
				  const Math::Vec4& inverseViewVector = NULL, 
				  const Math::Vec4& clipPlane = DefaultClipPlane);

		inline void AddLight(Light* light) { AddLight(0, light); }
		void AddLight(unsigned int index, Light* light);
	private:
		void PrepareTerrain(const Terrain* terrain);
		void BindTerrainTextures(const Terrain* terrain);
	};
}