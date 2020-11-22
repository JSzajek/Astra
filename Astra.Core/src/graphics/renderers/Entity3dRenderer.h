#pragma once

#include <vector>
#include <stack>

#include "Renderer.h"
#include "../loaders/Loader.h"
#include "../entities/Entity.h"
#include "../entities/Light.h"

#include "../../logger/Logger.h"

namespace Astra::Graphics
{
	class Entity3dRenderer : public Renderer
	{
	private:
		std::unordered_map<unsigned int, std::vector<const Entity*>> m_entities;
		std::vector<const Light*> m_lights;
		std::stack<std::pair<unsigned int, std::vector<const Entity*>>> m_selected;
		const Light* m_directionalLight;
		const Math::Vec3* m_fogColor;
		Math::Mat4 m_toShadowSpaceMatrix;
		Shader* m_selectionShader;
	public:
		Entity3dRenderer(const Math::Vec3* fogColor);

		void SetShader(Shader* shader) override;
		void SetSelectionColor(const Math::Vec3& color);
		void UpdateProjectionMatrix(const Math::Mat4* projectionMatrix) override;

		void Clear() override;
		inline void SetShadowMatrix(const Math::Mat4& shadowMatrix) { m_toShadowSpaceMatrix = shadowMatrix; }

		void Draw(const Math::Mat4* viewMatrix, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane) override;
		void AddEntity(const Entity* entity);
		void AddLight(Light* light);
		void UpdateLight(const Light* light);
	private:
		void PrepareEntity(const Entity* entity);
		void DrawSelected(const Math::Mat4* viewMatrix);
	};
}