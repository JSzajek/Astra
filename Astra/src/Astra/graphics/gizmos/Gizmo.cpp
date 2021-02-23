#include "astra_pch.h"

#include "Gizmo.h"
#include "Astra/graphics/loaders/Loader.h"
#include "Astra/graphics/ResourceManager.h"

namespace Astra::Graphics
{
#if ASTRA_DEBUG
	Gizmo::Gizmo(const char* const imagepath, const Math::Vec3& position, float scale)
		: Position(position), Scale(scale), m_texture(Loader::LoadTexture(imagepath, false))
	{
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