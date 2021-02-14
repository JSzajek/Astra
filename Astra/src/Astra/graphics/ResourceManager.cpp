#include "astra_pch.h"

#include "ResourceManager.h"

namespace Astra::Graphics
{
	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
		//ASTRA_CORE_ASSERT(m_loaded.size() == 0, "Resource Manager: Not all Resources Were Unloaded.")
	}

	bool ResourceManager::QueryTextureImpl(const char* filepath, Texture** texture)
	{
		size_t hash = std::hash<std::string>{}(std::string(filepath));
		auto found = m_textureDirectory.find(hash);
		if (found != m_textureDirectory.end())
		{
			*texture = found->second;
			ResourceManager::Track(*texture);
			return true;
		}
		*texture = new Texture(filepath);
		ResourceManager::Track(*texture);
		m_textureDirectory[hash] = *texture;
		return false;
	}
	
	bool ResourceManager::QueryFontAtlasTextureImpl(const char* filepath, unsigned int fontSize, Texture** texture)
	{
		size_t hash = std::hash<std::string>{}(std::string(filepath));
		hash += fontSize;
		auto found = m_textureDirectory.find(hash);
		if (found != m_textureDirectory.end())
		{
			*texture = found->second;
			ResourceManager::Track(*texture);
			return true;
		}
		*texture = new Texture(filepath);
		ResourceManager::Track(*texture);
		m_textureDirectory[hash] = *texture;
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

	GuiMaterial* ResourceManager::LoadGuiMaterialImpl(const char* filepath, size_t rowCount)
	{
		size_t hash = std::hash<std::string>{}(std::string(filepath));
		hash += rowCount;

		auto found = m_loadedGuiMaterials.find(hash);
		if (found != m_loadedGuiMaterials.end())
		{
			Track(found->second);
			return found->second;
		}

		auto* material = new GuiMaterial(filepath, rowCount);
		Track(material);
		m_loadedGuiMaterials[hash] = material;
		return material;
	}

	FontAtlas* ResourceManager::LoadFontAtlasImpl(const char* filepath, unsigned int fontSize)
	{
		size_t hash = std::hash<std::string>{}(std::string(filepath));

		auto found = m_loadedFontAtlases.find(hash);
		if (found != m_loadedFontAtlases.end())
		{
			Track(found->second);
			return found->second;
		}

		auto* material = new FontAtlas(filepath, fontSize);
		Track(material);
		m_loadedFontAtlases[hash] = material;
		return material;
	}

	void ResourceManager::UnloadTexture(const Texture* texture)
	{
		size_t hash = std::hash<std::string>{}(std::string(texture->m_filePath));
		m_textureDirectory.erase(m_textureDirectory.find(hash));
		delete texture;
	}
	
	void ResourceManager::UnloadTexture(const Texture* atlas, unsigned int fontSize)
	{
		size_t hash = std::hash<std::string>{}(std::string(atlas->m_filePath));
		hash += fontSize;
		m_textureDirectory.erase(m_textureDirectory.find(hash));
		delete atlas;
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

	void ResourceManager::UnloadGuiMaterial(const GuiMaterial* material)
	{
		for (auto it = m_loadedGuiMaterials.begin(); it != m_loadedGuiMaterials.end(); it++)
		{
			if (it->second == material)
			{
				m_loadedGuiMaterials.erase(it);
				delete material;
				break;
			}
		}
	}

	void ResourceManager::UnloadFontAtlas(const FontAtlas* atlas)
	{
		for (auto it = m_loadedFontAtlases.begin(); it != m_loadedFontAtlases.end(); it++)
		{
			if (it->second == atlas)
			{
				m_loadedFontAtlases.erase(it);
				delete atlas;
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