#include "ImageMaterial.h"

#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	ImageMaterial::ImageMaterial(const char* const filepath, int rowCount, float reflectivity, 
								 bool transparent, bool fakeLight)
		: m_diffuse(Loader::LoadTexture(filepath)), m_rowCount(rowCount), Reflectivity(reflectivity),
			Transparent(transparent), FakeLight(fakeLight)
	{
		m_specular = Loader::LoadTexture(Texture::DefaultSpecular);
	}

	ImageMaterial::ImageMaterial(const char* const diffusepath, const char* const specularpath,
								 int rowCount, float reflectivity, bool transparent, bool fakeLight)
		: m_diffuse(Loader::LoadTexture(diffusepath)), m_rowCount(rowCount), Reflectivity(reflectivity), Transparent(transparent), FakeLight(fakeLight)
	{
		m_specular = specularpath != NULL ? Loader::LoadTexture(specularpath) : Loader::LoadTexture(Texture::DefaultSpecular);
	}
}