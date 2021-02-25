#include "astra_pch.h"

#include "CubeMapTexture.h"
#include <GL/glew.h>

namespace Astra::Graphics
{
	CubeMapTexture::CubeMapTexture()
		: m_filepaths(), id(0), type(TextureType::None)
	{
	}

	CubeMapTexture::CubeMapTexture(const std::vector<const char*>& filepaths)
		: m_filepaths(filepaths), id(0), type(TextureType::DiffuseMap)
	{
	}

	CubeMapTexture::~CubeMapTexture()
	{
		glDeleteTextures(1, &id);
	}
}