#include "ImageMaterial.h"

#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	ImageMaterial::ImageMaterial(const char* const filepath)
		: Texture(filepath), reflectivity(0), shineDampener(1), transparent(false), fakeLight(false)
	{
		Loader::BufferTexture(this);
	}

	ImageMaterial::ImageMaterial(const char* const filepath, float shineDampener, float reflectivity, bool transparent, bool fakeLight)
		: Texture(filepath), shineDampener(shineDampener), reflectivity(reflectivity), transparent(transparent), fakeLight(fakeLight)
	{
		Loader::BufferTexture(this);
	}

	ImageMaterial::~ImageMaterial()
	{
		Loader::RemoveTexture(this);
	}
}