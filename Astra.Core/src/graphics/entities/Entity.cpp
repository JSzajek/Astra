#include "Entity.h"

namespace Astra::Graphics
{
	Entity::Entity()
		: vertexArray(NULL), material(NULL), Spatial(), m_textureIndex(0)
	{
	}
	
	Entity::~Entity()
	{
		delete vertexArray;
		delete material;
	}

	Entity::Entity(const Math::Vec3& position)
		: vertexArray(NULL), material(NULL), Spatial(position), m_textureIndex(0)
	{
	}

	Entity::Entity(const VertexArray* vertArray, const Math::Vec3& position)
		: vertexArray(vertArray), material(new ImageMaterial()), Spatial(position), m_textureIndex(0)
	{
	}

	Entity::Entity(const VertexArray* vertArray, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: vertexArray(vertArray), material(new ImageMaterial()), Spatial(position, rotation, scale), m_textureIndex(0)
	{
	}

	Entity::Entity(const VertexArray* vertArray, const ImageMaterial* material, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: vertexArray(vertArray), material(material), Spatial(position, rotation, scale), m_textureIndex(0)
	{
	}

	Entity::Entity(const VertexArray* vertArray, const ImageMaterial* material, int textureIndex, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: vertexArray(vertArray), material(material), m_textureIndex(textureIndex), Spatial(position, rotation, scale)
	{
	}

	Entity::Entity(const Entity& other)
		: vertexArray(other.vertexArray), material(other.material), m_textureIndex(other.m_textureIndex)
	{
		memcpy(data, other.data, 3 * 3 * sizeof(float));
	}

	float Entity::GetMaterialXOffset() const
	{
		int column = m_textureIndex % material->GetRowCount();
		return (float)column / (float)material->GetRowCount();
	}

	float Entity::GetMaterialYOffset() const
	{
		int row = m_textureIndex / material->GetRowCount();
		return (float)row / (float)material->GetRowCount();
	}

}