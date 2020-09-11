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
		
		void Draw(const Math::Mat4& viewMatrix) override;
		void AddEntity(const Entity* entity);
		void AddLight(Light* light);
		void UpdateLights();
	private:
		void PrepareEntity(const Entity& entity);
	};
}