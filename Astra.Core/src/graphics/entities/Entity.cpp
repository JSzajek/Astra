#include "Entity.h"

namespace Astra::Graphics
{
	Entity::Entity()
		: vertexArray(NULL), texture(NULL)
	{
	}

	Entity::Entity(const Math::Vec3& position)
		: vertexArray(NULL), texture(NULL)
	{
	}

	Entity::Entity(const VertexArray* vertArray, const Texture* texture, const Math::Vec3& position)
		: vertexArray(vertArray), texture(texture), Spatial(position)
	{
	}

	Entity::Entity(const VertexArray* vertArray, const Texture* texture, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: vertexArray(vertArray), texture(texture), Spatial(position, rotation, scale)
	{

	}

	Entity::Entity(const Entity& other)
		: vertexArray(other.vertexArray), texture(other.texture)
	{
		memcpy(data, other.data, 3 * 3 * sizeof(float));
	}

	/*Entity::Entity(const Math::Vec3& position, float xRotation, float yRotation, float zRotation, float scale)
	{

	}*/
}