#pragma once

#include "../../math/Vec3.h"
#include "../buffers/Texture.h"

namespace Astra::Graphics
{
#if ASTRA_DEBUG
	struct Gizmo
	{
	private:
		const Texture* m_texture;
	public:
		Math::Vec3 Position;
		float Scale;
	public:
		Gizmo(const char* const imagepath, const Math::Vec3& position, float scale);
		Gizmo(const Gizmo& other);
		~Gizmo();

		inline unsigned int GetTextureId() const { return m_texture->id; }
	};
#endif
}