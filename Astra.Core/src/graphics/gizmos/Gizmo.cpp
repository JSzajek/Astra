#include "Gizmo.h"

#include "../loaders/Loader.h"
#include "GizmoController.h"
#include "../ResourceManager.h"

namespace Astra::Graphics
{
#if _DEBUG
	Gizmo::Gizmo(const char* const imagepath, const Math::Vec3& position, float scale)
		: Position(position), Scale(scale), m_texture(Loader::LoadTexture(imagepath, false))
	{
		GizmoController::AddGizmo(this);
	}

	Gizmo::Gizmo(const Gizmo& other)
		: Position(other.Position), Scale(other.Scale), m_texture(other.m_texture)
	{
	}

	Gizmo::~Gizmo()
	{
		ResourceManager::Unload(m_texture);
	}

#endif
}