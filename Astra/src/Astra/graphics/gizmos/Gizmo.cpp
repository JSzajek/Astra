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
		: LayerEntity(), Position(position), Scale(scale), m_texture(Resource::LoadTexture(TextureCreationSpec(imagepath, false)))
	{
	}
#endif
}