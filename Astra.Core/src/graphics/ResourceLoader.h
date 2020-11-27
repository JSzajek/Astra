#pragma once

#include <unordered_map>
#include "entities/Entity.h"
#include "materials/Materials.h"

namespace Astra::Graphics
{
	class ResourceLoader
	{
	private:
		std::unordered_map<size_t, ImageMaterial*> m_loadedImageMaterials;
	public:
		ResourceLoader(const ResourceLoader&) = delete;
		void operator=(const ResourceLoader&) = delete;

		static ResourceLoader& Get()
		{
			static ResourceLoader instance;
			return instance;
		}

		static ImageMaterial* LoadMaterial(const char* diffuse, const char* specular, const char* emission = NULL, size_t rowCount = 1, float reflectivity = 16.f, bool transparent = false)
		{
			return Get().LoadMaterialImpl(diffuse, specular, emission, rowCount, reflectivity, transparent);
		}

		static ImageMaterial* LoadMaterial(const char* diffuse, const char* specular, const char* normalMap, const char* parallaxMap = NULL, float heightOffset = 0.f, const char* emission = NULL, size_t rowCount = 1, float reflectivity = 16.f, bool transparent = false)
		{
			return Get().LoadMaterialImpl(diffuse, specular, normalMap, parallaxMap, heightOffset, emission, rowCount, reflectivity, transparent);
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

	private:
		ResourceLoader();
		Entity* LoadEntityImpl(const char* filepath, bool calcTangents, int textureIndex, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale);
		ImageMaterial* LoadMaterialImpl(const char* diffuse, const char* specular, const char* emission, size_t rowCount, float reflectivity, bool transparent);
		ImageMaterial* LoadMaterialImpl(const char* diffuse, const char* specular, const char* normalMap, const char* parallaxMap, float heightOffset, const char* emission, size_t rowCount, float reflectivity, bool transparent);
	};
}