#include "astra_pch.h"

#include "ImageMaterial.h"
#include "Astra/graphics/loaders/Loader.h"
#include "Astra/graphics/ResourceManager.h"

namespace Astra::Graphics
{
	ImageMaterial::ImageMaterial(const std::vector<Texture*>& textures)
		: m_height(0), m_reflectivity(1), m_transparent(false), m_fakeLight(false),
			m_flags(0)
	{
		for (auto* texture : textures)
		{
			AddTexture(texture);
		}
	}

	ImageMaterial::~ImageMaterial()
	{
		for (auto* texture : m_textures)
		{
			RESOURCE_UNLOAD(texture);
		}
	}

	void ImageMaterial::UpdateDiffuseMap(bool hdr)
	{
		Loader::UpdateDiffuseTexture(m_textures[TextureType::DiffuseMap], hdr);
	}

	void ImageMaterial::AddTexture(Texture* texture)
	{
		m_textures[texture->type] = texture; // Add Pointer
		m_flags |= BIT(texture->type); // Mark as Added
	}
}