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
		inline void AddTerrain(const Terrain* terrain)
		{
			auto temp = m_terrains.find(terrain->vertexArray->vaoId);
			if (temp != m_terrains.end())
			{
				temp->second.emplace_back(terrain);
			}
			else
			{
				m_terrains[terrain->vertexArray->vaoId] = std::vector<const Terrain*>();
				m_terrains[terrain->vertexArray->vaoId].emplace_back(terrain);
			}
		}
		inline void AddLight(const Light* light)
		{
			if (m_lights.size() + 1 > MAX_LIGHTS)
			{
				Logger::Log("Too Many Lights");
				m_lights.pop_back();

			}
			m_lights.push_back(light);
		}
		void Draw(const Math::Mat4& viewMatrix) override;
	private:
		void PrepareTerrain(const Terrain& terrain);
		void BindTerrainTextures(const Terrain& terrain);
	};
}