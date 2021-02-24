#pragma once

#include "../renderers/GizmoRenderer.h"

namespace Astra::Graphics
{
#if ASTRA_DEBUG
	class GizmoController
	{
	private:
		GizmoRenderer* m_gizmoRenderer;
	public:
		GizmoController(const GizmoController&) = delete;
		void operator=(const GizmoController&) = delete;

		static GizmoController& Get()
		{
			static GizmoController instance;
			return instance;
		}

		static void UpdateProjectionMatrix(const Math::Mat4* projectionMatrix)
		{
			Get().UpdateProjectionMatrixImpl(projectionMatrix);
		}

		static void Render(const std::unordered_map<unsigned int, std::vector<const Graphics::Gizmo*>>& gizmos, const Math::Mat4* viewMatrix)
		{
			Get().RenderImpl(gizmos, viewMatrix);
		}
	private:
		GizmoController();
		~GizmoController();

		void UpdateProjectionMatrixImpl(const Math::Mat4* projectionMatrix);
		void RenderImpl(const std::unordered_map<unsigned int, std::vector<const Graphics::Gizmo*>>& gizmos, const Math::Mat4* viewMatrix);
	};
#endif
}