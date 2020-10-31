#include "ImageMaterial.h"

#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	ImageMaterial::ImageMaterial(const char* const filepath, int rowCount, float reflectivity, 
								 bool transparent, bool fakeLight, const Math::Vec3& specular)
		: m_diffuse(Loader::LoadTexture(filepath)), m_rowCount(rowCount), Reflectivity(reflectivity), /*Ambient(ambient), Diffuse(diffuse),*/ Specular(specular),
			Transparent(transparent), FakeLight(fakeLight)
	{
	}

	ImageMaterial::ImageMaterial(const char* const diffusepath, const char* const specularpath,
								 int rowCount, float reflectivity, bool transparent, bool fakeLight)
		: m_diffuse(Loader::LoadTexture(diffusepath)), m_rowCount(rowCount), Reflectivity(reflectivity), Transparent(transparent), FakeLight(fakeLight), Specular(0)
	{
		m_specular = specularpath != NULL ? Loader::LoadTexture(specularpath) : Loader::LoadTexture(diffusepath, GL_REPEAT, true, true);
	}
}