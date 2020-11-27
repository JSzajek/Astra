#pragma once

#include <unordered_map>

#include "buffers/VertexArray.h"
#include "materials/ImageMaterial.h"
#include "materials/ParticleMaterial.h"
#include "materials/SkyboxMaterial.h"
#include "materials/TerrainMaterial.h"
#include "materials/WaterMaterial.h"
#include "buffers/FrameBuffer.h"
#include "fonts/TextMesh.h"
#include "buffers/Texture.h"

#include "../logger/Logger.h"


namespace Astra::Graphics
{
	class ResourceManager
	{
	
	#define RESOURCE_UNLOAD(resource) { if (resource != NULL) ResourceManager::Unload(resource); }

	private:
		std::unordered_map<const void*, unsigned int> m_loaded;
	public:
		ResourceManager(const ResourceManager&) = delete;
		void operator=(const ResourceManager&) = delete;

		static ResourceManager& Get()
		{
			static ResourceManager instance;
			return instance;
		}

		static const VertexArray* TrackVertexArray(const VertexArray* vao)
		{
			return Get().TrackVertexArrayImpl(vao);
		}

		static const Texture* TrackTexture(const Texture* texture)
		{
			return Get().TrackTextureImpl(texture);
		}

		static const FrameBuffer* TrackFrameBuffer(const FrameBuffer* buffer)
		{
			return Get().TrackFrameBufferImpl(buffer);
		}
		
		static const ImageMaterial* TrackImageMaterial(const ImageMaterial* material)
		{
			return Get().TrackImageMaterialImpl(material);
		}

		static const ParticleMaterial* TrackParticleMaterial(const ParticleMaterial* material)
		{
			return Get().TrackParticleMaterialImpl(material);
		}

		static const SkyboxMaterial* TrackSkyboxMaterial(const SkyboxMaterial* material)
		{
			return Get().TrackSkyboxMaterialImpl(material);
		}

		static const TerrainMaterial* TrackTerrainMaterial(const TerrainMaterial* material)
		{
			return Get().TrackTerrainMaterialImpl(material);
		}

		static WaterMaterial* TrackWaterMaterial(WaterMaterial* material)
		{
			return Get().TrackWaterMaterialImpl(material);
		}

		static FontType* TrackFontType(FontType* type)
		{
			return Get().TrackFontTypeImpl(type);
		}
		
		template <typename T>
		static void Unload(const T* ptr)
		{
			auto& loaded = Get().m_loaded;
			const auto found = loaded.find(ptr);
			if (found != loaded.end())
			{
				(*found).second--;
				if (!(*found).second)
				{
					loaded.erase(found);
					delete ptr;
				}
			}
		#if _DEBUG
			if (loaded.size() == 0)
			{
				Logger::Log("Resource Manager: Cleaned All Loaded Resources");
			}
		#endif
		}

	private:
		ResourceManager();
		
		const VertexArray* TrackVertexArrayImpl(const VertexArray* vao);
		const Texture* TrackTextureImpl(const Texture* texture);
		
		const ImageMaterial* TrackImageMaterialImpl(const ImageMaterial* material);
		const ParticleMaterial* TrackParticleMaterialImpl(const ParticleMaterial* material);
		const SkyboxMaterial* TrackSkyboxMaterialImpl(const SkyboxMaterial* material);
		const TerrainMaterial* TrackTerrainMaterialImpl(const TerrainMaterial* material);
		WaterMaterial* TrackWaterMaterialImpl(WaterMaterial* material);

		FontType* TrackFontTypeImpl(FontType* type);

		const FrameBuffer* TrackFrameBufferImpl(const FrameBuffer* buffer);
	};
}