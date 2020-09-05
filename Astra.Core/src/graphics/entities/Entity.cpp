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

	Entity::Entity(const VertexArray* vertArray, const ImageMaterial* material, const Math::Vec3& position)
		: vertexArray(vertArray), material(material), Spatial(position)
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
	}

	/*Entity::Entity(const Math::Vec3& position, float xRotation, float yRotation, float zRotation, float scale)
	{

	}*/
}