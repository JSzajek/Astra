#pragma once

#include "Astra/math/Vec3.h"
#include "Astra/graphics/buffers/Texture.h"
#include "Astra/graphics/entities/LayerEntity.h"

namespace Astra::Graphics
{
#if ASTRA_DEBUG
	struct Gizmo : public LayerEntity
	{
	private:
		Asset<Texture> m_texture;
	public:
		Math::Vec3 Position;
		float Scale;
	public:
		Gizmo();
		Gizmo(const char* const imagepath, const Math::Vec3& position, float scale);

		inline virtual std::string ToString() const override { return !Name.length() ? ("Gizmo_&" + std::to_string(m_uid)) : Name; }
		inline unsigned int GetTextureId() const { return m_texture->id; }
	};
#endif
}