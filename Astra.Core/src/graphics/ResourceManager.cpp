#include "ResourceManager.h"


namespace Astra::Graphics
{
	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
	#if _DEBUG
		if (m_loaded.size() == 0)
		{
			Logger::Log("Resource Manager: Cleaned All Loaded Resources");
		}
	#endif
	}

	bool ResourceManager::QueryTextureImpl(const char* filepath, Texture** texture)
	{
		auto found = m_textureDirectory.find(filepath);
		if (found != m_textureDirectory.end())
		{
			*texture = found->second;
			ResourceManager::Track(*texture);
			return true;
		}
		*texture = new Texture(filepath);
		ResourceManager::Track(*texture);
		m_textureDirectory[filepath] = *texture;
		return false;
	}
	
	Entity* ResourceManager::LoadEntityImpl(const char* filepath, bool calcTangents, int textureIndex, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale)
	{
		return new Entity(filepath, calcTangents, textureIndex, position, rotation, scale);
	}

	ImageMaterial* ResourceManager::LoadMaterialImpl(const char* diffuse, const char* specular, const char* emission, size_t rowCount, float reflectivity, bool transparent)
	{
		size_t hash = std::hash<std::string>{}(std::string(diffuse) + std::string(specular) + std::string(emission ? emission : ""));
		hash += rowCount + std::hash<float>{}(reflectivity)+std::hash<bool>{}(transparent);

		auto found = m_loadedImageMaterials.find(hash);
		if (found != m_loadedImageMaterials.end())
		{
			Track(found->second);
			return found->second;
		}

		auto* material = new ImageMaterial(diffuse, specular, emission, rowCount, reflectivity, transparent);
		Track(material);
		m_loadedImageMaterials[hash] = material;
		return material;
	}

	ImageMaterial* ResourceManager::LoadMaterialImpl(const char* diffuse, const char* specular, const char* normalMap, const char* parallaxMap, float heightOffset, const char* emission, size_t rowCount, float reflectivity, bool transparent)
	{
		size_t hash = std::hash<std::string>{}(std::string(diffuse) + std::string(specular) + std::string(normalMap) + std::string(parallaxMap ? parallaxMap : "") + std::string(emission ? emission : ""));
		hash += rowCount + std::hash<float>{}(reflectivity)+std::hash<bool>{}(transparent);

		auto found = m_loadedImageMaterials.find(hash);
		if (found != m_loadedImageMaterials.end())
		{
			Track(found->second);
			return found->second;
		}

		auto* material = new ImageMaterial(diffuse, specular, normalMap, parallaxMap, heightOffset, emission, rowCount, reflectivity, transparent);
		Track(material);
		m_loadedImageMaterials[hash] = material;
		return material;
	}

	void ResourceManager::UnloadTexture(const Texture* texture)
	{
		m_textureDirectory.erase(m_textureDirectory.find(texture->m_filePath));
		delete texture;
	}

	void ResourceManager::UnloadImageMaterial(const ImageMaterial* material)
	{
		for (auto it = m_loadedImageMaterials.begin(); it != m_loadedImageMaterials.end(); it++)
		{
			if (it->second == material)
			{
				m_loadedImageMaterials.erase(it);
				delete material;
				break;
			}
		}
	}

	bool ResourceManager::UnloadResource(void* ptr)
	{
		auto& loaded = Get().m_loaded;
		const auto found = loaded.find(ptr);
		if (found != loaded.end())
		{
			(*found).second--;
			if (!(*found).second)
			{
				loaded.erase(found);
				return true;
			}
		}
		return false;
	}
}