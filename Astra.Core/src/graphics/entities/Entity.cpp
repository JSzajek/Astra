#include "Entity.h"

#include "../loaders/ObjLoader.h"

namespace Astra::Graphics
{
	Entity::Entity()
		: vertexArray(NULL), material(NULL), normalMap(NULL), 
			Spatial(), m_textureIndex(0), m_normalMapped(false), m_parallaxMapped(false), m_height(0)
	{
	}
	
	Entity::~Entity()
	{
		delete vertexArray;
		delete material;
	}

	Entity::Entity(const Math::Vec3& position)
		: vertexArray(NULL), material(NULL), normalMap(NULL), Spatial(position), 
			m_textureIndex(0), m_normalMapped(false), m_parallaxMapped(false), m_height(0)
	{
	}

	Entity::Entity(const VertexArray* vertArray, const Math::Vec3& position)
		: vertexArray(vertArray), material(new ImageMaterial()), normalMap(NULL), 
			Spatial(position), m_textureIndex(0), m_normalMapped(false), m_parallaxMapped(false), m_height(0)
	{
	}

	Entity::Entity(const VertexArray* vertArray, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: vertexArray(vertArray), material(new ImageMaterial()), normalMap(NULL), 
			Spatial(position, rotation, scale), m_textureIndex(0), m_normalMapped(false), m_parallaxMapped(false), m_height(0)
	{
	}

	Entity::Entity(const VertexArray* vertArray, const ImageMaterial* material, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: vertexArray(vertArray), material(material), normalMap(NULL), 
			Spatial(position, rotation, scale), m_textureIndex(0), m_normalMapped(false), m_parallaxMapped(false), m_height(0)
	{
	}

	Entity::Entity(const VertexArray* vertArray, const ImageMaterial* material, int textureIndex, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: vertexArray(vertArray), material(material), normalMap(NULL), m_textureIndex(textureIndex), 
			Spatial(position, rotation, scale), m_normalMapped(false), m_parallaxMapped(false), m_height(0)
	{
	}

	Entity::Entity(const char* const filepath, const char* const normalMapTexture, const ImageMaterial* material, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: vertexArray(ObjLoader::LoadNormalMappedObjectModel(filepath)), 
			normalMap(Loader::LoadTexture(normalMapTexture, false)),  material(material), 
			m_textureIndex(0), Spatial(position, rotation, scale), m_normalMapped(true), m_parallaxMapped(false), m_height(0)
	{
	}

	Entity::Entity(const Entity& other)
		: vertexArray(other.vertexArray), material(other.material), normalMap(other.normalMap), Spatial(other),
			m_textureIndex(other.m_textureIndex), m_normalMapped(other.m_normalMapped), m_parallaxMapped(other.m_parallaxMapped), m_height(other.m_height)
	{
	}

	Entity::Entity(const char* const filepath, const char* const normalMapTexture, const char* const heightMapTexture,
			float height, const ImageMaterial* material, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
		: vertexArray(ObjLoader::LoadNormalMappedObjectModel(filepath)), normalMap(Loader::LoadTexture(normalMapTexture, false)),
			parallaxMap(Loader::LoadTexture(heightMapTexture, false)), material(material), m_textureIndex(0),
				Spatial(position, rotation, scale), m_normalMapped(true), m_parallaxMapped(true), m_height(height)
	{

	}
}