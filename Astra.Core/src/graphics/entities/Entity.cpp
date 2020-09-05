#include "Entity.h"

namespace Astra::Graphics
{
	Entity::Entity()
		: vertexArray(NULL), material(NULL)
	{
	}

	Entity::Entity(const Math::Vec3& position)
		: vertexArray(NULL), material(NULL)
	{
	}

	Entity::Entity(const VertexArray* vertArray, const Math::Vec3& position)
		: vertexArray(vertArray), material(new ImageMaterial())
	{
	}

	Entity::Entity(const VertexArray* vertArray, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: vertexArray(vertArray), material(new ImageMaterial()), Spatial(position, rotation, scale)
	{
	}

	Entity::Entity(const VertexArray* vertArray, const ImageMaterial* material, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: vertexArray(vertArray), material(material), Spatial(position, rotation, scale)
	{
	}

	Entity::Entity(const Entity& other)
		: vertexArray(other.vertexArray), material(other.material)
	{
		memcpy(data, other.data, 3 * 3 * sizeof(float));
	}

	Entity::~Entity()
	{
		delete vertexArray;
		delete material;
	}
}