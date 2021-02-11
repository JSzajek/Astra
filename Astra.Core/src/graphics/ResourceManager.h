#pragma once

#include <unordered_map>

#include "buffers/VertexArray.h"
#include "materials/ImageMaterial.h"
#include "materials/GuiMaterial.h"
#include "materials/ParticleMaterial.h"
#include "materials/SkyboxMaterial.h"
#include "materials/TerrainMaterial.h"
#include "materials/WaterMaterial.h"

#include "guis/utility/FontAtlas.h"

#include "buffers/FrameBuffer.h"
#include "buffers/Texture.h"
#include "entities/Entity.h"

#include "../logger/Logger.h"

namespace Astra::Graphics
{
	class ResourceManager
	{
	
	#define RESOURCE_UNLOAD(resource, ...) { if (resource != NULL) ResourceManager::Unload(resource, __VA_ARGS__); }

	private:
		std::unordered_map<size_t, ImageMaterial*> m_loadedImageMaterials;
		std::unordered_map<size_t, GuiMaterial*> m_loadedGuiMaterials;
		std::unordered_map<size_t, FontAtlas*> m_loadedFontAtlases;
		std::unordered_map<size_t, Texture*> m_textureDirectory;
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

		static bool QueryFontAtlasTexture(const char* filepath, unsigned int fontSize, Texture** texture)
		{
			return Get().QueryFontAtlasTextureImpl(filepath, fontSize, texture);
		}

		static GuiMaterial* LoadGuiMaterial(const char* filepath, size_t rowCount = 1)
		{
			return Get().LoadGuiMaterialImpl(filepath, rowCount);
		}

		static ImageMaterial* LoadMaterial(const char* diffuse, const char* specular, const char* emission = NULL, size_t rowCount = 1, float reflectivity = 16.f, bool transparent = false)
		{
			return Get().LoadMaterialImpl(diffuse, specular, emission, rowCount, reflectivity, transparent);
		}

		static ImageMaterial* LoadMaterial(const char* diffuse, const char* specular, const char* normalMap, const char* parallaxMap = NULL, float heightOffset = 0.f, const char* emission = NULL, size_t rowCount = 1, float reflectivity = 16.f, bool transparent = false)
		{
			return Get().LoadMaterialImpl(diffuse, specular, normalMap, parallaxMap, heightOffset, emission, rowCount, reflectivity, transparent);
		}

		static FontAtlas* LoadFontAtlas(const char* filepath, unsigned int fontSize)
		{
			return Get().LoadFontAtlasImpl(filepath, fontSize);
		}

		static Entity* LoadNormalEntity(const char* filepath, int textureIndex = 0,
			const Math::Vec3& position = Math::Vec3::Zero,
			const Math::Vec3& rotation = Math::Vec3::Zero,
			const Math::Vec3& scale = Math::Vec3::One)
		{
			return Get().LoadEntityImpl(filepath, true, textureIndex, position, rotation, scale);
		}

		static Entity* LoadEntity(const char* filepath, int textureIndex = 0,
			const Math::Vec3& position = Math::Vec3::Zero,
			const Math::Vec3& rotation = Math::Vec3::Zero,
			const Math::Vec3& scale = Math::Vec3::One)
		{
			return Get().LoadEntityImpl(filepath, false, textureIndex, position, rotation, scale);
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
		bool QueryFontAtlasTextureImpl(const char* filepath, unsigned int fontSize, Texture** texture);

		Entity* LoadEntityImpl(const char* filepath, bool calcTangents, int textureIndex, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale);
		ImageMaterial* LoadMaterialImpl(const char* diffuse, const char* specular, const char* emission, size_t rowCount, float reflectivity, bool transparent);
		ImageMaterial* LoadMaterialImpl(const char* diffuse, const char* specular, const char* normalMap, const char* parallaxMap, float heightOffset, const char* emission, size_t rowCount, float reflectivity, bool transparent);
		GuiMaterial* LoadGuiMaterialImpl(const char* filepath, size_t rowCount);
		FontAtlas* LoadFontAtlasImpl(const char* filepath, unsigned int fontSize);
	private:
		void UnloadTexture(const Texture* texture);
		void UnloadTexture(const Texture* atlas, unsigned int fontSize);
		void UnloadImageMaterial(const ImageMaterial* material);
		void UnloadGuiMaterial(const GuiMaterial* material);
		void UnloadFontAtlas(const FontAtlas* atlas);
		bool UnloadResource(void* ptr);
	};
}