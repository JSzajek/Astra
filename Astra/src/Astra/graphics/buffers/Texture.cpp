#include "astra_pch.h"

#include "Texture.h"
#include <GL/glew.h>

namespace Astra::Graphics
{
	const char* const Texture::DefaultTexture = RESOURCE("res/textures/Default.jpg");
	const char* const Texture::DefaultSpecular = RESOURCE("res/textures/DefaultSpecular.jpg");

	Texture::Texture()
		: id(0), width(0), height(0), m_filePath(DefaultTexture)
	{
	}

	Texture::Texture(const char* const filepath)
		: id(0), width(0), height(0), m_filePath(filepath)
	{
	}

	Texture::Texture(unsigned int id, const char* const filepath)
		: id(id), width(0), height(0), m_filePath(filepath)
	{
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &id);
	}
	
	bool Texture::operator==(const Texture& other)
	{
		return id == other.id;
	}
}