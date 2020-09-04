#pragma once

#include <vector>

#include "Renderer.h"
#include "../loaders/Loader.h"
#include "../entities/Terrain.h"
#include "../entities/Light.h"
#include "../shaders/TerrainShader.h"

namespace Astra::Graphics
{
	class TerrainRenderer : public Renderer
	{
	private:
		std::unordered_map<GLuint, std::vector<const Terrain*>> m_terrains;
		//std::vector<Light> m_lights;
		Light m_light;
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
		inline void AddLight(const Light& light) { m_light = light; }
		void Draw(const Math::Mat4& viewMatrix) override;
	private:
		void PrepareTerrain(const Terrain& entity);
	};
}