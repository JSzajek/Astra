#pragma once

#include <unordered_map>

#include "Renderer.h"

#include "../gizmos/Gizmo.h"
#include "../shaders/GizmoShader.h"

namespace Astra::Graphics
{
#if ASTRA_DEBUG
	class GizmoRenderer : public Renderer
	{
	private:
		const VertexArray* m_defaultQuad;
		std::unordered_map<unsigned int, std::vector<const Gizmo*>> m_gizmos;
		const Math::Mat4* m_viewMatrix;
		Math::Mat4* m_modelViewMatrix;
	public:
		GizmoRenderer(GizmoShader* shader);
		~GizmoRenderer();

		//inline void Clear() override { m_gizmos.clear(); }
		inline std::unordered_map<unsigned int, std::vector<const Gizmo*>>& GetGizmos() { return m_gizmos; }

		void AddGizmo(const Gizmo* gizmo);
		void Draw(float delta = 0, const Math::Mat4* viewMatrix = NULL, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane);
	private:
		void UpdateModelViewMatrix(const Math::Vec3& position, float scale);
	};
#endif
}