#pragma once

#include <vector>

#include "Renderer.h"
#include "../loaders/Loader.h"
#include "../shaders/BasicShader.h"
#include "../shaders/NormalEntityShader.h"
#include "../entities/Entity.h"
#include "../entities/Light.h"

#include "../../logger/Logger.h"

namespace Astra::Graphics
{
	class NormalEntity3dRenderer : public Renderer
	{
	private:
		std::unordered_map<GLuint, std::vector<const Entity*>> m_entities;
		std::vector<const Light*> m_lights;
		const Math::Vec3* m_skyColor;
		Math::Mat4 m_viewMatrix;
	public:
		NormalEntity3dRenderer(Shader* shader, const Math::Vec3* fogColor);

		void Draw(const Math::Mat4& viewMatrix, const Math::Vec4& clipPlane = DefaultClipPlane) override;
		void AddEntity(const Entity* entity);
		void AddLight(Light* light);
		void UpdateLights();
	private:
		void PrepareEntity(const Entity* entity);
	};
}