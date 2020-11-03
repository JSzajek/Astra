#include "GizmoController.h"

namespace Astra::Graphics
{
#if _DEBUG
	GizmoController::GizmoController()
	{
		m_gizmoRenderer = new GizmoRenderer(new GizmoShader());
	}

	GizmoController::~GizmoController()
	{
		delete m_gizmoRenderer;
	}

	void GizmoController::UpdateProjectionMatrixImpl(const Math::Mat4& projectionMatrix)
	{
		m_gizmoRenderer->UpdateProjectionMatrix(projectionMatrix);
	}

	void GizmoController::RenderImpl(const Math::Mat4& viewMatrix)
	{
		m_gizmoRenderer->Draw(viewMatrix, NULL);
	}

	void GizmoController::AddGizmoImpl(const Gizmo* gizmo)
	{
		m_gizmoRenderer->AddGizmo(gizmo);
	}

	void GizmoController::ClearImpl()
	{
		m_gizmoRenderer->Clear();
	}
#endif
}