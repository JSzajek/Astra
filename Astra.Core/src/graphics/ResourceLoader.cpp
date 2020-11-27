#include "ResourceLoader.h"

#include "ResourceManager.h"

namespace Astra::Graphics
{
	ResourceLoader::ResourceLoader()
	{
	}

	Entity* ResourceLoader::LoadEntityImpl(const char* filepath, bool calcTangents, int textureIndex, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
	{
		return new Entity(filepath, calcTangents, textureIndex, position, rotation, scale);
	}

	ImageMaterial* ResourceLoader::LoadMaterialImpl(const char* diffuse, const char* specular, const char* emission, size_t rowCount, float reflectivity, bool transparent)
	{
		size_t hash = std::hash<std::string>{}(std::string(diffuse) + std::string(specular) + std::string(emission ? emission : ""));
		hash += rowCount + std::hash<float>{}(reflectivity) + std::hash<bool>{}(transparent);
		
		auto found = m_loadedImageMaterials.find(hash);
		if (found != m_loadedImageMaterials.end())
		{
			ResourceManager::TrackImageMaterial(found->second);
			return found->second;
		}
	
		auto* material = new ImageMaterial(diffuse, specular, emission, rowCount, reflectivity, transparent);
		ResourceManager::TrackImageMaterial(material);
		m_loadedImageMaterials[hash] = material;
		return material;
	}
	
	ImageMaterial* ResourceLoader::LoadMaterialImpl(const char* diffuse, const char* specular, const char* normalMap, const char* parallaxMap, float heightOffset, const char* emission, size_t rowCount, float reflectivity, bool transparent)
	{
		size_t hash = std::hash<std::string>{}(std::string(diffuse) + std::string(specular) + std::string(normalMap) + std::string(parallaxMap ? parallaxMap : "") + std::string(emission ? emission : ""));
		hash += rowCount + std::hash<float>{}(reflectivity) + std::hash<bool>{}(transparent);

		auto found = m_loadedImageMaterials.find(hash);
		if (found != m_loadedImageMaterials.end())
		{
			ResourceManager::TrackImageMaterial(found->second);
			return found->second;
		}

		auto* material = new ImageMaterial(diffuse, specular, normalMap, parallaxMap, heightOffset, emission, rowCount, reflectivity, transparent);
		ResourceManager::TrackImageMaterial(material);
		m_loadedImageMaterials[hash] = material;
		return material;
	}
}