#pragma once

#include <stack>
#include <vector>

#include "Renderer.h"
#include "../loaders/Loader.h"
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
		std::stack<std::pair<GLuint, std::vector<const Entity*>>> m_selected;
		const Math::Vec3* m_fogColor;
		const Light* m_directionalLight;
		Math::Mat4 m_toShadowSpaceMatrix;
		Shader* m_selectionShader;
	public:
		NormalEntity3dRenderer(const Math::Vec3* fogColor);
		
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