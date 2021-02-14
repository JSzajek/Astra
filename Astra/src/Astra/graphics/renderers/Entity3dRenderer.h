#pragma once

#include <vector>
#include <stack>

#include "Renderer.h"
#include "../loaders/Loader.h"
#include "../entities/Entity.h"
#include "../entities/Light.h"

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
		const Math::Mat4* m_toShadowSpaceMatrix;
		Shader* m_selectionShader;
		bool m_drawSelection;
	#if ASTRA_DEBUG
		bool m_wireframe;
	#endif
	public:
		Entity3dRenderer(const Math::Vec3* fogColor);

	#if ASTRA_DEBUG
		void SetWireframe(bool wireframe) { m_wireframe = wireframe; };
	#endif
		void SetShader(Shader* shader) override;
		void SetSelectionColor(const Math::Vec3& color);
		void UpdateProjectionMatrix(const Math::Mat4* projectionMatrix) override;

		void Clear() override;
		inline void SetShadowMatrix(const Math::Mat4* shadowMatrix) { m_toShadowSpaceMatrix = shadowMatrix; }
		inline void FlagSelectionDraw() { m_drawSelection = true; }

		void Draw(float delta = 0, const Math::Mat4* viewMatrix = NULL, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane) override;
		void AddEntity(const Entity* entity);
		void AddLight(Light* light);
		void UpdateLight(const Light* light);
	private:
		void PrepareEntity(const Entity* entity);
		void DrawSelected(const Math::Mat4* viewMatrix);
	};
}