#include "ImageMaterial.h"

#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	ImageMaterial::ImageMaterial()
		: Texture(Loader::LoadTexture(Texture::DefaultTexture)), reflectivity(0), shineDampener(1), transparent(false), fakeLight(false)
	{
	}

	ImageMaterial::ImageMaterial(const char* const filepath)
		: Texture(Loader::LoadTexture(filepath)), reflectivity(0), shineDampener(1), transparent(false), fakeLight(false)
	{
	}

	ImageMaterial::ImageMaterial(const char* const filepath, float shineDampener, float reflectivity, bool transparent, bool fakeLight)
		: Texture(Loader::LoadTexture(filepath)), shineDampener(shineDampener), reflectivity(reflectivity), transparent(transparent), fakeLight(fakeLight)
	{
	}
}