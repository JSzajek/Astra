#include "ImageMaterial.h"

#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	ImageMaterial::ImageMaterial()
		: Texture(Loader::LoadTexture(Texture::DefaultTexture)), m_rowCount(1), reflectivity(0), shineDampener(1), transparent(false), fakeLight(false)
	{
	}

	ImageMaterial::ImageMaterial(const char* const filepath)
		: Texture(Loader::LoadTexture(filepath)), m_rowCount(1), reflectivity(0), shineDampener(1), transparent(false), fakeLight(false)
	{
	}

	ImageMaterial::ImageMaterial(const char* const filepath, int rowCount)
		: Texture(Loader::LoadTexture(filepath)), m_rowCount(rowCount), reflectivity(0), shineDampener(1), transparent(false), fakeLight(false)
	{
	}

	ImageMaterial::ImageMaterial(const char* const filepath, int rowCount, float shineDampener, float reflectivity, bool transparent, bool fakeLight)
		: Texture(Loader::LoadTexture(filepath)), m_rowCount(rowCount), shineDampener(shineDampener), reflectivity(reflectivity), transparent(transparent), fakeLight(fakeLight)
	{
	}
}