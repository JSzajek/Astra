#include "ImageMaterial.h"

#include "../loaders/Loader.h"

namespace Astra::Graphics
{
	ImageMaterial::ImageMaterial(const char* const filepath, int rowCount, float reflectivity, 
								 bool transparent, bool fakeLight)
		: m_diffuse(Loader::LoadTexture(filepath)), m_rowCount(rowCount), Reflectivity(reflectivity),
			Transparent(transparent), FakeLight(fakeLight), m_glowing(false)
	{
		m_specular = Loader::LoadTexture(Texture::DefaultSpecular);
	}

	ImageMaterial::ImageMaterial(const char* const diffusepath, const char* const specularpath,
								 int rowCount, float reflectivity, bool transparent)
		: m_diffuse(Loader::LoadTexture(diffusepath)), m_rowCount(rowCount), Reflectivity(reflectivity), Transparent(transparent), FakeLight(false), m_glowing(false)
	{
		m_specular = specularpath != NULL ? Loader::LoadTexture(specularpath, false) : Loader::LoadTexture(Texture::DefaultSpecular, false);
	}

	ImageMaterial::ImageMaterial(const char* const diffusepath, const char* const specularpath, const char* const emissionpath,
									int rowCount, float reflectivity, bool transparent)
		: m_diffuse(Loader::LoadTexture(diffusepath)), m_rowCount(rowCount), Reflectivity(reflectivity), Transparent(transparent), FakeLight(false)
	{
		m_specular = specularpath != NULL ? Loader::LoadTexture(specularpath, false) : Loader::LoadTexture(Texture::DefaultSpecular, false);
		m_glowing = emissionpath != NULL;
		m_emission = Loader::LoadTexture(m_glowing ? emissionpath : Texture::DefaultSpecular);
	}

	//ImageMaterial::ImageMaterial(const char* const diffusepath, const char* const specularpath, 
	//							 const char* const heightmappath, int rowCount, float reflectivity, bool transparent)
	//	: m_diffuse(Loader::LoadTexture(diffusepath)), m_rowCount(rowCount), Reflectivity(reflectivity), Transparent(transparent), FakeLight(false)
	//{
	//	m_specular = Loader::LoadTexture(specularpath != NULL ? specularpath : Texture::DefaultSpecular);
	//	m_heightmap = Loader::LoadTexture(heightmappath != NULL ? heightmappath : Texture::DefaultSpecular);
	//}
}