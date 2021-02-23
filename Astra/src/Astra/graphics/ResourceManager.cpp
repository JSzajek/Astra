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
	#if ASTRA_DEBUG
		*texture = new Texture(filepath);
	#else
		*texture = new Texture();
	#endif
		ResourceManager::Track(*texture);
		m_textureDirectory[hash] = *texture;
		return false;
	}
	
	bool ResourceManager::QueryTextureImpl(const std::vector<const char*>& filepaths, CubeMapTexture** texture)
	{
		std::string combinded;
		for (const auto* file : filepaths)
		{
			combinded += file;
		}
		size_t hash = std::hash<std::string>{}(combinded);

		auto found = m_cubeMapTextureDirectory.find(hash);
		if (found != m_cubeMapTextureDirectory.end())
		{
			*texture = found->second;
			ResourceManager::Track(*texture);
			return true;
		}
		*texture = new CubeMapTexture(filepaths);
		ResourceManager::Track(*texture);
		m_cubeMapTextureDirectory[hash] = *texture;
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

	Model* ResourceManager::LoadModelImpl(const char* const filepath, bool calcTangents)
	{
		size_t hash = std::hash<std::string>{}(filepath);

		auto found = m_loadedModels.find(hash);
		if (found != m_loadedModels.end())
		{
			Track(found->second);
			return new Model(*found->second); // Return copy of loaded data
		}
		auto* material = new Model(filepath, calcTangents);
		Track(material);
		m_loadedModels[hash] = material;
		return material;
	}

	ImageMaterial* ResourceManager::LoadMaterialImpl(const std::vector<Texture*>& textures, size_t hash)
	{
		auto found = m_loadedImageMaterials.find(hash);
		if (found != m_loadedImageMaterials.end())
		{
			Track(found->second);
			return found->second;
		}

		auto* material = new ImageMaterial(textures);
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

	TerrainMaterial* ResourceManager::LoadTerrainMaterialImpl(const char* filepath)
	{
		size_t hash = std::hash<std::string>{}(std::string(filepath));

		auto found = m_loadedTerrainMaterials.find(hash);
		if (found != m_loadedTerrainMaterials.end())
		{
			Track(found->second);
			return found->second;
		}

		auto* material = new TerrainMaterial(filepath);
		Track(material);
		m_loadedTerrainMaterials[hash] = material;
		return material;
	}

	TerrainMaterialPack* ResourceManager::LoadTerrainMaterialPackImpl(const char* background, const char* red, const char* blue, const char* green)
	{
		size_t hash = std::hash<std::string>{}(std::string(background) + red + blue + green);

		auto found = m_loadedTerrainMaterialPacks.find(hash);
		if (found != m_loadedTerrainMaterialPacks.end())
		{
			Track(found->second);
			return found->second;
		}

		auto* _back = LoadTerrainMaterialImpl(background);
		auto* _red = LoadTerrainMaterialImpl(red);
		auto* _blue = LoadTerrainMaterialImpl(blue);
		auto* _green = LoadTerrainMaterialImpl(green);

		auto* material = new TerrainMaterialPack(_back, _red, _blue, _green);
		Track(material);
		m_loadedTerrainMaterialPacks[hash] = material;
		return material;
	}

	SkyboxMaterial* ResourceManager::LoadSkyboxMaterialImpl(std::vector<const char*> first, std::vector<const char*> second)
	{
		std::string combined = std::string(first[0]);
		for (size_t i = 1; i < first.size(); i++)
		{
			combined += first[i];
		}
		for (const auto* file : second)
		{
			combined += file;
		}
		size_t hash = std::hash<std::string>{}(combined);

		auto found = m_loadedSkyboxMaterials.find(hash);
		if (found != m_loadedSkyboxMaterials.end())
		{
			Track(found->second);
			return found->second;
		}

		auto* material = new SkyboxMaterial(first, second);
		Track(material);
		m_loadedSkyboxMaterials[hash] = material;
		return material;
	}

	WaterMaterial* ResourceManager::LoadWaterMaterialImpl(const char* diffuse, const char* dudvMap, const char* normalMap)
	{
		size_t hash = std::hash<std::string>{}(std::string(diffuse) + dudvMap + normalMap);

		auto found = m_loadedWaterMaterials.find(hash);
		if (found != m_loadedWaterMaterials.end())
		{
			Track(found->second);
			return found->second;
		}

		auto* material = new WaterMaterial(diffuse, dudvMap, normalMap);
		Track(material);
		m_loadedWaterMaterials[hash] = material;
		return material;
	}

	ParticleMaterial* ResourceManager::LoadParticleMaterialImpl(const char* filepath, unsigned int rowCount)
	{
		size_t hash = std::hash<std::string>{}(filepath);

		auto found = m_loadedParticleMaterials.find(hash);
		if (found != m_loadedParticleMaterials.end())
		{
			Track(found->second);
			return found->second;
		}

		auto* material = new ParticleMaterial(filepath, rowCount);
		Track(material);
		m_loadedParticleMaterials[hash] = material;
		return material;
	}

	void ResourceManager::UnloadTexture(const Texture* texture)
	{
		size_t hash = std::hash<std::string>{}(std::string(texture->m_filePath));
		m_textureDirectory.erase(m_textureDirectory.find(hash));
		delete texture;
	}

	void ResourceManager::UnloadCubeMapTexture(const CubeMapTexture* texture)
	{
		std::string combinded;
		for (const auto* file : texture->GetFiles())
		{
			combinded += file;
		}
		size_t hash = std::hash<std::string>{}(combinded);
		m_cubeMapTextureDirectory.erase(m_cubeMapTextureDirectory.find(hash));
		delete texture;
	}
	
	void ResourceManager::UnloadTexture(const Texture* atlas, unsigned int fontSize)
	{
		size_t hash = std::hash<std::string>{}(std::string(atlas->m_filePath));
		hash += fontSize;
		m_textureDirectory.erase(m_textureDirectory.find(hash));
		delete atlas;
	}

	void ResourceManager::UnloadModel(const Model* model)
	{
		auto source = m_loadedModels.find(model->GetRenderID());
		auto last = UnloadResource(source->second);
		if (last) // Last reference to model - can clean up vao, vbos, ebos, etc.
		{
			for (auto it = m_loadedModels.begin(); it != m_loadedModels.end(); it++)
			{
				if (it->second == source->second) 
				{ 
					m_loadedModels.erase(it);
					for (auto mesh : source->second->GetMeshes())
					{
						mesh.Unload();
					}
					delete source->second; 
					break; 
				}
			}
		}
		UnloadResource((void*)model);
		delete model;
	}

	void ResourceManager::UnloadImageMaterial(const ImageMaterial* material)
	{
		LIST_UNLOAD(m_loadedImageMaterials, material);
	}

	void ResourceManager::UnloadGuiMaterial(const GuiMaterial* material)
	{
		LIST_UNLOAD(m_loadedGuiMaterials, material);
	}

	void ResourceManager::UnloadFontAtlas(const FontAtlas* atlas)
	{
		LIST_UNLOAD(m_loadedFontAtlases, atlas);
	}

	void ResourceManager::UnloadTerrainMaterial(const TerrainMaterial* material)
	{
		LIST_UNLOAD(m_loadedTerrainMaterials, material);
	}

	void ResourceManager::UnloadTerrainMaterialPack(const TerrainMaterialPack* material)
	{
		LIST_UNLOAD(m_loadedTerrainMaterialPacks, material);
	}

	void ResourceManager::UnloadSkyboxMaterial(const SkyboxMaterial* material)
	{
		LIST_UNLOAD(m_loadedSkyboxMaterials, material);
	}

	void ResourceManager::UnloadWaterMaterial(const WaterMaterial* material)
	{
		LIST_UNLOAD(m_loadedWaterMaterials, material);
	}

	void ResourceManager::UnloadParticleMaterial(const ParticleMaterial* material)
	{
		LIST_UNLOAD(m_loadedParticleMaterials, material);
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

	void ResourceManager::ToggleHDRTexturesImpl(bool enabled)
	{
		for (const auto& image : m_loadedImageMaterials)
		{
			image.second->UpdateDiffuseMap(enabled);
		}
		for (const auto& terrain : m_loadedTerrainMaterials)
		{
			terrain.second->UpdateDiffuseMap(enabled);
		}
		for (const auto& water : m_loadedWaterMaterials)
		{
			water.second->UpdateDiffuseMap(enabled);
		}
		for (const auto& sky : m_loadedSkyboxMaterials)
		{
			sky.second->UpdateDiffuseMap(enabled);
		}
		for (const auto& particle : m_loadedParticleMaterials)
		{
			particle.second->UpdateDiffuseMap(enabled);
		}
	}
}