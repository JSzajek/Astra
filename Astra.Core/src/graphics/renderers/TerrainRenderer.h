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
		const Math::Vec3* m_skyColor;
		Math::Mat4 m_toShadowSpaceMatrix;
	public:
		TerrainRenderer(Shader* shader, const Math::Vec3* fogColor);
		
		inline void SetShadowMatrix(const Math::Mat4& shadowMatrix) { m_toShadowSpaceMatrix = shadowMatrix; }

		void Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane = DefaultClipPlane) override;
		void AddTerrain(const Terrain* terrain);
		void AddLight(Light* light);
		void UpdateLights();
	private:
		void PrepareTerrain(const Terrain& terrain);
		void BindTerrainTextures(const Terrain& terrain);
	};
}