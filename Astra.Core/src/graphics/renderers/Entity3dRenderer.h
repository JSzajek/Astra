#pragma once

#include <vector>

#include "Renderer.h"
#include "../loaders/Loader.h"
#include "../shaders/LightingShader.h"
#include "../shaders/BasicShader.h"
#include "../entities/Entity.h"
#include "../entities/Light.h"

#include "../../logger/Logger.h"

namespace Astra::Graphics
{
	class Entity3dRenderer : public Renderer
	{
	private:
		std::unordered_map<GLuint, std::vector<const Entity*>> m_entities;
		std::vector<const Light*> m_lights;
		const Math::Vec3* m_skyColor;
	public:
		Entity3dRenderer(Shader* shader, const Math::Vec3* skyColor);
		inline void AddEntity(const Entity* entity) 
		{
			auto temp = m_entities.find(entity->vertexArray->vaoId);
			if (temp != m_entities.end())
			{
				temp->second.emplace_back(entity);
			}
			else
			{
				m_entities[entity->vertexArray->vaoId] = std::vector<const Entity*>();
				m_entities[entity->vertexArray->vaoId].emplace_back(entity);
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
		void PrepareEntity(const Entity& entity);
	};
}