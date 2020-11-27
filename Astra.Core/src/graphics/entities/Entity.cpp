#include "Entity.h"

#include "../loaders/ObjLoader.h"
#include "../ResourceManager.h"
#include "../../math/Mat4Utils.h"

namespace Astra::Graphics
{
	Entity::Entity()
		: vertexArray(NULL), material(NULL), normalMap(NULL), parallaxMap(NULL),
			Spatial(), m_textureIndex(0), m_normalMapped(false), m_parallaxMapped(false), m_height(0),
			selectedModelMatrix(new Math::Mat4()), m_selected(false)
	{
	}
	
	Entity::~Entity()
	{
		ResourceManager::Unload(vertexArray);
		ResourceManager::Unload(material);
		ResourceManager::Unload(normalMap);
		ResourceManager::Unload(parallaxMap);
		delete selectedModelMatrix;
	}

	Entity::Entity(const Math::Vec3& position)
		: vertexArray(NULL), material(NULL), normalMap(NULL), parallaxMap(NULL), Spatial(position),
			m_textureIndex(0), m_normalMapped(false), m_parallaxMapped(false), m_height(0),
			selectedModelMatrix(new Math::Mat4()), m_selected(false)
	{
	}

	Entity::Entity(const char* const objpath, const ImageMaterial* material, int textureIndex,
		const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: m_textureIndex(textureIndex), normalMap(NULL), parallaxMap(NULL),
			Spatial(position, rotation, scale), m_normalMapped(false), m_parallaxMapped(false), m_height(0),
			selectedModelMatrix(new Math::Mat4()), m_selected(false)
	{
		this->vertexArray = ObjLoader::LoadObjectModel(objpath);
		this->material = ResourceManager::TrackImageMaterial(material);
	}

	Entity::Entity(const char* const filepath, const char* const normalMapTexture, const ImageMaterial* material, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		:  normalMap(Loader::LoadTexture(normalMapTexture, false)), parallaxMap(NULL),
			m_textureIndex(0), Spatial(position, rotation, scale), m_normalMapped(true), m_parallaxMapped(false), m_height(0),
			selectedModelMatrix(new Math::Mat4()), m_selected(false)
	{
		this->vertexArray = ObjLoader::LoadNormalMappedObjectModel(filepath);
		this->material = ResourceManager::TrackImageMaterial(material);
	}

	Entity::Entity(const Entity& other)
		: vertexArray(other.vertexArray), material(other.material), normalMap(other.normalMap), parallaxMap(NULL), Spatial(other),
			m_textureIndex(other.m_textureIndex), m_normalMapped(other.m_normalMapped), m_parallaxMapped(other.m_parallaxMapped), m_height(other.m_height),
			selectedModelMatrix(other.selectedModelMatrix), m_selected(other.m_selected)
	{
	}

	Entity::Entity(const char* const filepath, const char* const normalMapTexture, const char* const heightMapTexture,
			float height, const ImageMaterial* material, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: normalMap(Loader::LoadTexture(normalMapTexture, false)),
			parallaxMap(Loader::LoadTexture(heightMapTexture, false)), m_textureIndex(0),
				Spatial(position, rotation, scale), m_normalMapped(true), m_parallaxMapped(true), m_height(height),
					selectedModelMatrix(new Math::Mat4()), m_selected(false)
	{
		this->vertexArray = ObjLoader::LoadNormalMappedObjectModel(filepath);
		this->material = ResourceManager::TrackImageMaterial(material);
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