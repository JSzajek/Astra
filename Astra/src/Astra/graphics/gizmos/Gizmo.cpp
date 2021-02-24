#include "astra_pch.h"

#include "Gizmo.h"
#include "Astra/graphics/Resource.h"

namespace Astra::Graphics
{
#if ASTRA_DEBUG
	Gizmo::Gizmo()
		: LayerEntity(), Position(Math::Vec3::Zero), Scale(0), m_texture(NULL)
	{
	}

	Gizmo::Gizmo(const char* const imagepath, const Math::Vec3& position, float scale)
		: LayerEntity(), Position(position), Scale(scale), m_texture(Resource::LoadTexture(imagepath, false))
	{
	}

	Gizmo::Gizmo(const Gizmo& other)
		: LayerEntity(other), Position(other.Position), Scale(other.Scale), m_texture(other.m_texture)
	{
		TRACK(m_texture);
	}

	void Gizmo::operator=(const Gizmo& other)
	{
		Name = other.Name;
		m_uid = other.m_uid;

		Position = other.Position;
		Scale = other.Scale;
		
		m_texture = other.m_texture;
		TRACK(m_texture);
	}

	Gizmo::~Gizmo()
	{
		UNLOAD(m_texture);
	}
#endif
}