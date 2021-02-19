#include "astra_pch.h"

#include "ImageMaterial.h"
#include "../loaders/Loader.h"
#include "../ResourceManager.h"

namespace Astra::Graphics
{
	ImageMaterial::ImageMaterial(const char* diffuse, const char* specular, const char* emission, size_t rowCount, float reflectivity, bool transparent)
		: m_diffuse(Loader::LoadTexture(diffuse)), m_rowCount(rowCount), Reflectivity(reflectivity), Transparent(transparent), FakeLight(false),
			m_specular(Loader::LoadTexture(specular, false)), m_normalMap(NULL), m_parallaxMap(NULL), m_height(0)
	{
		m_emission = emission ? Loader::LoadTexture(emission) : NULL;
	}

	ImageMaterial::ImageMaterial(const char* diffuse, const char* specular, const char* normalMap, const char* parallaxMap, float heightOffset, const char* emission, size_t rowCount, float reflectivity, bool transparent)
		: m_diffuse(Loader::LoadTexture(diffuse)), m_rowCount(rowCount), Reflectivity(reflectivity), Transparent(transparent), FakeLight(false),
			m_specular(Loader::LoadTexture(specular, false)), m_normalMap(Loader::LoadTexture(normalMap, false))
	{
		m_parallaxMap = parallaxMap ? Loader::LoadTexture(parallaxMap) : NULL;
		m_height = heightOffset;
		m_emission = emission ? Loader::LoadTexture(emission) : NULL;
	}

	ImageMaterial::~ImageMaterial()
	{
		RESOURCE_UNLOAD(m_diffuse);
		RESOURCE_UNLOAD(m_specular);
		RESOURCE_UNLOAD(m_emission);
		RESOURCE_UNLOAD(m_normalMap);
		RESOURCE_UNLOAD(m_parallaxMap);
	}

	void ImageMaterial::UpdateDiffuseMap(bool hdr)
	{
		Loader::UpdateDiffuseTexture(m_diffuse, hdr);
	}
}