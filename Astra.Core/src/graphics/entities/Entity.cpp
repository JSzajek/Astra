#include "Entity.h"

#include "../loaders/ObjLoader.h"
#include "../ResourceManager.h"
#include "../../math/Mat4Utils.h"

namespace Astra::Graphics
{
	Entity::Entity()
		: Spatial(), vertexArray(NULL), material(NULL),
			m_textureIndex(0), selectedModelMatrix(new Math::Mat4()), m_selected(false)
	{
	}
	
	Entity::Entity(const char* const filepath, bool calcTangents, int textureIndex, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: Spatial(position, rotation, scale), m_textureIndex(textureIndex), material(NULL), 
			selectedModelMatrix(new Math::Mat4()), m_selected(false)
	{
		vertexArray = calcTangents ? ObjLoader::LoadNormalMappedObjectModel(filepath) : ObjLoader::LoadObjectModel(filepath);
	}

	Entity::Entity(const Entity& other)
		: vertexArray(other.vertexArray), material(other.material), Spatial(other),
			m_textureIndex(other.m_textureIndex), selectedModelMatrix(other.selectedModelMatrix), m_selected(other.m_selected)
	{
	}

	Entity::~Entity()
	{
		ResourceManager::Unload(vertexArray);
		ResourceManager::Unload(material);
		delete selectedModelMatrix;
	}

	void Entity::UpdateMatrices()
	{
		if (m_selected)
		{
			*(Scale()) *= 1.1f;
			*selectedModelMatrix = Math::Mat4Utils::Transformation(this);
			*(Scale()) /= 1.1f;
		}
		Spatial::UpdateMatrices();
	}
}