#pragma once

#include <vector>

#include "Renderer.h"
#include "../loaders/Loader.h"
#include "../entities/Terrain.h"
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
	public:
		TerrainRenderer(Shader* shader, const Math::Vec3* skyColor);
		
		void Draw(const Math::Mat4& viewMatrix) override;
		void AddTerrain(const Terrain* terrain);
		void AddLight(Light* light);
		void UpdateLights();
	private:
		void PrepareTerrain(const Terrain& terrain);
		void BindTerrainTextures(const Terrain& terrain);
	};
}