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

		static void AddGizmo(const Gizmo* gizmo)
		{
			Get().AddGizmoImpl(gizmo);
		}

		static void UpdateProjectionMatrix(const Math::Mat4* projectionMatrix)
		{
			Get().UpdateProjectionMatrixImpl(projectionMatrix);
		}

		static void Render(const Math::Mat4* viewMatrix)
		{
			Get().RenderImpl(viewMatrix);
		}

		//static void Clear()
		//{
		//	Get().ClearImpl();
		//}

	private:
		GizmoController();
		~GizmoController();

		void UpdateProjectionMatrixImpl(const Math::Mat4* projectionMatrix);
		void RenderImpl(const Math::Mat4* viewMatrix);
		void AddGizmoImpl(const Gizmo* gizmo);
		//void ClearImpl();
	};
#endif
}