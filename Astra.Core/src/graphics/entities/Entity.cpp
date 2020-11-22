#include "Entity.h"

#include "../loaders/ObjLoader.h"
#include "../../math/Mat4Utils.h"

namespace Astra::Graphics
{
	Entity::Entity()
		: vertexArray(NULL), material(NULL), normalMap(NULL), 
			Spatial(), m_textureIndex(0), m_normalMapped(false), m_parallaxMapped(false), m_height(0),
			selectedModelMatrix(new Math::Mat4()), m_selected(false)
	{
	}
	
	Entity::~Entity()
	{
		ObjLoader::UnloadVertexArray(vertexArray);
		vertexArray = NULL;
		delete material;
		delete selectedModelMatrix;
	}

	Entity::Entity(const Math::Vec3& position)
		: vertexArray(NULL), material(NULL), normalMap(NULL), Spatial(position), 
			m_textureIndex(0), m_normalMapped(false), m_parallaxMapped(false), m_height(0),
			selectedModelMatrix(new Math::Mat4()), m_selected(false)
	{
	}

	Entity::Entity(const char* const objpath, const ImageMaterial* material, int textureIndex,
		const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: vertexArray(ObjLoader::LoadObjectModel(objpath)), material(material), m_textureIndex(textureIndex), 
			Spatial(position, rotation, scale), m_normalMapped(false), m_parallaxMapped(false), m_height(0),
			selectedModelMatrix(new Math::Mat4()), m_selected(false)
	{
	}

	Entity::Entity(const char* const filepath, const char* const normalMapTexture, const ImageMaterial* material, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: vertexArray(ObjLoader::LoadNormalMappedObjectModel(filepath)), 
			normalMap(Loader::LoadTexture(normalMapTexture, false)),  material(material), 
			m_textureIndex(0), Spatial(position, rotation, scale), m_normalMapped(true), m_parallaxMapped(false), m_height(0),
			selectedModelMatrix(new Math::Mat4()), m_selected(false)
	{
	}

	Entity::Entity(const Entity& other)
		: vertexArray(other.vertexArray), material(other.material), normalMap(other.normalMap), Spatial(other),
			m_textureIndex(other.m_textureIndex), m_normalMapped(other.m_normalMapped), m_parallaxMapped(other.m_parallaxMapped), m_height(other.m_height),
			selectedModelMatrix(other.selectedModelMatrix), m_selected(other.m_selected)
	{
	}

	Entity::Entity(const char* const filepath, const char* const normalMapTexture, const char* const heightMapTexture,
			float height, const ImageMaterial* material, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: vertexArray(ObjLoader::LoadNormalMappedObjectModel(filepath)), normalMap(Loader::LoadTexture(normalMapTexture, false)),
			parallaxMap(Loader::LoadTexture(heightMapTexture, false)), material(material), m_textureIndex(0),
				Spatial(position, rotation, scale), m_normalMapped(true), m_parallaxMapped(true), m_height(height),
					selectedModelMatrix(new Math::Mat4()), m_selected(false)
	{
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