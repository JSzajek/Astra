#include "astra_pch.h"

#include "ImageMaterial.h"
#include "Astra/graphics/loaders/Loader.h"
#include "Astra/graphics/Resource.h"

namespace Astra::Graphics
{
	ImageMaterial::ImageMaterial()
		: m_height(0), m_reflectivity(1), m_transparent(false), m_fakeLight(false), m_flags(0)
	{
	}

	ImageMaterial::ImageMaterial(const std::vector<Asset<Texture>>& textures)
		: ImageMaterial()
	{
		for (const auto& texture : textures)
		{
			AddTexture(texture);
		}
	}

	void ImageMaterial::AddTexture(const Asset<Texture>& texture)
	{
		m_textures[texture->type] = texture; // Add Pointer
		m_flags |= BIT(texture->type); // Mark as Added
	}
}