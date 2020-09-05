#pragma once

#include "Spatial.h"
#include "../buffers/Texture.h"
#include "../buffers/VertexArray.h"

namespace Astra::Graphics
{
	class Entity : public Spatial
	{
	public:
		const VertexArray* const vertexArray;
		const Texture* const texture;
	public:
		Entity();
		~Entity();
		Entity(const Math::Vec3& position);
		Entity(const VertexArray* vertArray, const Texture* texture, const Math::Vec3& position);
		Entity(const VertexArray* vertArray, const Texture* texture, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale);
		
		Entity(const Entity& other);
		//Entity(const Math::Vec3& position, float xRotation, float yRotation, float zRotation, float scale);
	};
}