#pragma once

#include <vector>

#include "Renderer.h"
#include "../loaders/Loader.h"
#include "../entities/terrains/Terrain.h"
#include "../entities/Light.h"
#include "../shaders/TerrainShader.h"

#include "../../logger/Logger.h"

namespace Astra::Graphics
{
	class TerrainRenderer : public Renderer
	{
	private:
		std::unordered_map<GLuint, std::vector<const Terrain*>> m_terrains;
		std::vector<const Light*> m_lights;
		const Math::Vec3* m_fogColor;
		const Light* m_directionalLight;
		const Math::Mat4* m_toShadowSpaceMatrix;
	#if _DEBUG
		bool m_wireframe;
	#endif
	public:
		TerrainRenderer(const Math::Vec3* fogColor);
		
	#if _DEBUG
		void SetWireframe(bool wireframe) { m_wireframe = wireframe; };
	#endif
		void SetShader(Shader* shader) override;

		inline void Clear() override;
		inline void SetShadowMatrix(const Math::Mat4* shadowMatrix) { m_toShadowSpaceMatrix = shadowMatrix; }

		void Draw(float delta = 0, const Math::Mat4* viewMatrix = NULL, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane) override;
		void AddTerrain(const Terrain* terrain);
		void AddLight(Light* light);
		void UpdateLight(const Light* light);
	private:
		void PrepareTerrain(const Terrain* terrain);
		void BindTerrainTextures(const Terrain* terrain);
	};
}