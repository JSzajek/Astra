#pragma once

#include <vector>
#include <unordered_map>

#include "Astra/graphics/buffers/FrameBuffer.h"
#include "Astra/graphics/buffers/VertexArray.h"
#include "Astra/graphics/buffers/Texture.h"

#include "Astra/graphics/materials/ParticleMaterial.h"
#include "Astra/graphics/materials/TerrainMaterial.h"
#include "Astra/graphics/materials/SkyboxMaterial.h"
#include "Astra/graphics/materials/WaterMaterial.h"
#include "Astra/graphics/materials/ImageMaterial.h"
#include "Astra/graphics/materials/GuiMaterial.h"

#include "Astra/graphics/entities/Model.h"

#include "Astra/graphics/guis/utility/FontAtlas.h"

namespace Astra::Graphics
{
	class ResourceManager
	{
	
	#define RESOURCE_UNLOAD(resource, ...) { if (resource != NULL) { ResourceManager::Unload(resource, __VA_ARGS__); resource = NULL; } }

	#define LIST_UNLOAD(list, obj)	for (auto it = list.begin(); it != list.end(); it++)   \
									{ if (it->second == obj) { list.erase(it); delete obj; break; } }

	private:
		std::unordered_map<size_t, Model*> m_loadedModels;
		std::unordered_map<size_t, ImageMaterial*> m_loadedImageMaterials;
		std::unordered_map<size_t, GuiMaterial*> m_loadedGuiMaterials;
		std::unordered_map<size_t, FontAtlas*> m_loadedFontAtlases;
		std::unordered_map<size_t, TerrainMaterial*> m_loadedTerrainMaterials;
		std::unordered_map<size_t, TerrainMaterialPack*> m_loadedTerrainMaterialPacks;
		std::unordered_map<size_t, SkyboxMaterial*> m_loadedSkyboxMaterials;
		std::unordered_map<size_t, WaterMaterial*> m_loadedWaterMaterials;
		std::unordered_map<size_t, ParticleMaterial*> m_loadedParticleMaterials;
		std::unordered_map<size_t, Texture*> m_textureDirectory;
		std::unordered_map<size_t, CubeMapTexture*> m_cubeMapTextureDirectory;
		std::unordered_map<const void*, unsigned int> m_loaded;
	public:
		ResourceManager(const ResourceManager&) = delete;
		void operator=(const ResourceManager&) = delete;

		static ResourceManager& Get()
		{
			static ResourceManager instance;
			return instance;
		}

		static bool QueryTexture(const char* filepath, Texture** texture)
		{
			return Get().QueryTextureImpl(filepath, texture);
		}

		static bool QueryTexture(const std::vector<const char*>& filepaths, CubeMapTexture** texture)
		{
			return Get().QueryTextureImpl(filepaths, texture);
		}

		static bool QueryFontAtlasTexture(const char* filepath, unsigned int fontSize, Texture** texture)
		{
			return Get().QueryFontAtlasTextureImpl(filepath, fontSize, texture);
		}

		static GuiMaterial* LoadGuiMaterial(const char* filepath, size_t rowCount = 1)
		{
			return Get().LoadGuiMaterialImpl(filepath, rowCount);
		}

		static ImageMaterial* LoadMaterial(const std::vector<Texture*>& textures, size_t hash)
		{
			return Get().LoadMaterialImpl(textures, hash);
		}

		static TerrainMaterial* LoadTerrainMaterial(const char* filepath)
		{
			return Get().LoadTerrainMaterialImpl(filepath);
		}

		static TerrainMaterialPack* LoadTerrainMaterialPack(const char* background, const char* red, const char* blue, const char* green)
		{
			return Get().LoadTerrainMaterialPackImpl(background, red, blue, green);
		}

		static SkyboxMaterial* LoadSkyboxMaterial(std::vector<const char*> first, std::vector<const char*> second)
		{
			return Get().LoadSkyboxMaterialImpl(first, second);
		}

		static WaterMaterial* LoadWaterMaterial(const char* diffuse = WaterMaterial::DefaultDiffuseMap, 
												const char* dudvMap = WaterMaterial::DefaultDuDvMap, 
												const char* normalMap = WaterMaterial::DefaultNormalMap)
		{
			return Get().LoadWaterMaterialImpl(diffuse, dudvMap, normalMap);
		}

		static ParticleMaterial* LoadParticleMaterial(const char* filepath, unsigned int rowCount)
		{
			return Get().LoadParticleMaterialImpl(filepath, rowCount);
		}

		static FontAtlas* LoadFontAtlas(const char* filepath, unsigned int fontSize)
		{
			return Get().LoadFontAtlasImpl(filepath, fontSize);
		}

		static Model* LoadModel(const char* const filepath, bool calcNormals = false)
		{
			return Get().LoadModelImpl(filepath, calcNormals);
		}

		static void ToggleHDRTextures(bool enabled)
		{
			Get().ToggleHDRTexturesImpl(enabled);
		}

		template <typename T>
		static T* Track(T* ptr)
		{
			auto& loaded = Get().m_loaded;
			if (loaded.find(ptr) != loaded.end())
			{
				loaded[ptr]++;
			}
			else
			{
				loaded[ptr] = 1;
			}
			return ptr;
		}

		/* Template Override */
		static void Unload(const Texture* ptr)
		{
			if (Get().UnloadResource((void*)ptr))
			{
				Get().UnloadTexture(ptr);
			}
		}

		/* Template Override */
		static void Unload(const Model* ptr)
		{
			Get().UnloadModel(ptr); // Models are unique pointers
		}

		/* Template Override */
		static void Unload(const CubeMapTexture* ptr)
		{
			if (Get().UnloadResource((void*)ptr))
			{
				Get().UnloadCubeMapTexture(ptr);
			}
		}

		/* Template Overload */
		static void Unload(const Texture* ptr, unsigned int fontSize)
		{
			if (Get().UnloadResource((void*)ptr))
			{
				Get().UnloadTexture(ptr, fontSize);
			}
		}

		/* Template Override */
		static void Unload(const ImageMaterial* ptr)
		{
			if (Get().UnloadResource((void*)ptr))
			{
				Get().UnloadImageMaterial(ptr);
			}
		}

		/* Template Override */
		static void Unload(const GuiMaterial* ptr)
		{
			if (Get().UnloadResource((void*)ptr))
			{
				Get().UnloadGuiMaterial(ptr);
			}
		}

		/* Template Override */
		static void Unload(const FontAtlas* ptr)
		{
			if (Get().UnloadResource((void*)ptr))
			{
				Get().UnloadFontAtlas(ptr);
			}
		}

		/* Template Override */
		static void Unload(const TerrainMaterial* ptr)
		{
			if (Get().UnloadResource((void*)ptr))
			{
				Get().UnloadTerrainMaterial(ptr);
			}
		}

		/* Template Override */
		static void Unload(const TerrainMaterialPack* ptr)
		{
			if (Get().UnloadResource((void*)ptr))
			{
				Get().UnloadTerrainMaterialPack(ptr);
			}
		}

		/* Template Override */
		static void Unload(const SkyboxMaterial* ptr)
		{
			if (Get().UnloadResource((void*)ptr))
			{
				Get().UnloadSkyboxMaterial(ptr);
			}
		}

		/* Template Override */
		static void Unload(const WaterMaterial* ptr)
		{
			if (Get().UnloadResource((void*)ptr))
			{
				Get().UnloadWaterMaterial(ptr);
			}
		}

		/* Template Override */
		static void Unload(const ParticleMaterial* ptr)
		{
			if (Get().UnloadResource((void*)ptr))
			{
				Get().UnloadParticleMaterial(ptr);
			}
		}

		template <typename T>
		static void Unload(const T* ptr)
		{
			if (Get().UnloadResource((void*)ptr))
			{
				delete ptr;
			}
		}

	private:
		ResourceManager();
		~ResourceManager();
		
		bool QueryTextureImpl(const char* filepath, Texture** texture);
		bool QueryTextureImpl(const std::vector<const char*>& filepaths, CubeMapTexture** texture);
		bool QueryFontAtlasTextureImpl(const char* filepath, unsigned int fontSize, Texture** texture);

		Model* LoadModelImpl(const char* const filepath, bool calcTangents);
		ImageMaterial* LoadMaterialImpl(const std::vector<Texture*>& textures, size_t hash);
		GuiMaterial* LoadGuiMaterialImpl(const char* filepath, size_t rowCount);
		FontAtlas* LoadFontAtlasImpl(const char* filepath, unsigned int fontSize);
	
		TerrainMaterial* LoadTerrainMaterialImpl(const char* filepath);
		TerrainMaterialPack* LoadTerrainMaterialPackImpl(const char* background, const char* red, const char* blue, const char* green);

		SkyboxMaterial* LoadSkyboxMaterialImpl(std::vector<const char*> first, std::vector<const char*> second);
		WaterMaterial* LoadWaterMaterialImpl(const char* diffuse, const char* dudvMap, const char* normalMap);

		ParticleMaterial* LoadParticleMaterialImpl(const char* filepath, unsigned int rowCount);
	private:
		void UnloadTexture(const Texture* texture);
		void UnloadModel(const Model* model);
		void UnloadCubeMapTexture(const CubeMapTexture* texture);
		void UnloadTexture(const Texture* atlas, unsigned int fontSize);
		void UnloadImageMaterial(const ImageMaterial* material);
		void UnloadGuiMaterial(const GuiMaterial* material);
		void UnloadFontAtlas(const FontAtlas* atlas);
		void UnloadTerrainMaterial(const TerrainMaterial* material);
		void UnloadTerrainMaterialPack(const TerrainMaterialPack* material);
		void UnloadSkyboxMaterial(const SkyboxMaterial* material);
		void UnloadWaterMaterial(const WaterMaterial* material);
		void UnloadParticleMaterial(const ParticleMaterial* material);
		bool UnloadResource(void* ptr);

		void ToggleHDRTexturesImpl(bool enabled);
	};
}