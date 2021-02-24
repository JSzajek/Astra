#include "astra_pch.h"

#include "ImageMaterial.h"
#include "Astra/graphics/loaders/Loader.h"
#include "Astra/graphics/ResourceManager.h"
#include "Astra/graphics/Resource.h"

namespace Astra::Graphics
{
	ImageMaterial::ImageMaterial()
		: m_height(0), m_reflectivity(1), m_transparent(false), m_fakeLight(false), m_flags(0)
	{
	}

	ImageMaterial::ImageMaterial(const std::vector<Texture*>& textures)
		: ImageMaterial()
	{
		for (auto* texture : textures)
		{
			AddTexture(texture);
		}
	}

	ImageMaterial::ImageMaterial(const ImageMaterial& other)
		: m_height(other.m_height), m_reflectivity(other.m_reflectivity), m_fakeLight(other.m_fakeLight),
			m_transparent(other.m_transparent), m_flags(other.m_flags)
	{
		memcpy(m_textures, other.m_textures, sizeof(m_textures));
		for (auto* texture : m_textures)
		{
			TRACK(texture);
		}
	}

	void ImageMaterial::operator=(const ImageMaterial& other)
	{
		m_height = other.m_height;
		m_reflectivity = other.m_reflectivity;
		m_fakeLight = other.m_fakeLight;
		m_transparent = other.m_transparent;
		m_flags = other.m_flags;

		memcpy(m_textures, other.m_textures, sizeof(m_textures));
		for (auto* texture : m_textures)
		{
			TRACK(texture);
		}
	}

	ImageMaterial::~ImageMaterial()
	{
		for (auto* texture : m_textures)
		{
			UNLOAD(texture);
		}
	}

	void ImageMaterial::AddTexture(Texture* texture)
	{
		m_textures[texture->type] = texture; // Add Pointer
		m_flags |= BIT(texture->type); // Mark as Added
	}
}