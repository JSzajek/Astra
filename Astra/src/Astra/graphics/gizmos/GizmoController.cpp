#include "astra_pch.h"

#include "GizmoController.h"

namespace Astra::Graphics
{
#if ASTRA_DEBUG
	GizmoController::GizmoController()
	{
		m_gizmoRenderer = new GizmoRenderer(new GizmoShader());
	}

	GizmoController::~GizmoController()
	{
		delete m_gizmoRenderer;
	}

	void GizmoController::UpdateProjectionMatrixImpl(const Math::Mat4* projectionMatrix)
	{
		m_gizmoRenderer->UpdateProjectionMatrix(projectionMatrix);
	}

	void GizmoController::RenderImpl(const std::unordered_map<unsigned int, std::vector<const Graphics::Gizmo*>>& gizmos, const Math::Mat4* viewMatrix)
	{
		m_gizmoRenderer->Draw(gizmos, viewMatrix);
	}
#endif
}