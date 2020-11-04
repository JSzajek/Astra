#pragma once

#include "../../math/Vec3.h"
#include "../buffers/Texture.h"

namespace Astra::Graphics
{
#if _DEBUG
	struct Gizmo
	{
	private:
		Texture m_texture;
	public:
		Math::Vec3 Position;
		float Scale;
	public:
		Gizmo(const char* const imagepath, const Math::Vec3& position, float scale);
		Gizmo(const Gizmo& other);

		inline unsigned int GetTextureId() const { return m_texture.id; }
	};
#endif
}