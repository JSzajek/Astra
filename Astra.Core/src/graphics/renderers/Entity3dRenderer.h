#pragma once

#include <vector>

#include "Renderer.h"
#include "../loaders/Loader.h"
#include "../shaders/LightingShader.h"
#include "../shaders/BasicShader.h"
#include "../entities/Entity.h"
#include "../entities/Light.h"

namespace Astra::Graphics
{
	class Entity3dRenderer : public Renderer
	{
	private:
		std::unordered_map<GLuint, std::vector<Entity>> m_entities;
		//std::vector<Light> m_lights;
		Light m_light;
	public:
		Entity3dRenderer(Shader* shader);
		inline void AddEntity(const Entity& entity) 
		{
			auto temp = m_entities.find(entity.vertexArray->vaoId);
			if (temp != m_entities.end())
			{
				temp->second.push_back(entity);
			}
			else
			{
				m_entities[entity.vertexArray->vaoId] = std::vector<Entity>();
				m_entities[entity.vertexArray->vaoId].push_back(entity);
			}
		}
		inline void AddLight(const Light& light) { m_light = light; }
		void Draw(const Math::Mat4& viewMatrix) override;
	private:
		void PrepareEntity(const Entity& entity);
	};
}