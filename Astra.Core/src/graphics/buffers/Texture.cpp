#include "Texture.h"

namespace Astra::Graphics
{
	Texture::Texture()
		: id(0), width(0), height(0), m_filePath(DefaultTexture)
	{
	}

	Texture::Texture(const char* const filepath)
		: id(0), width(0), height(0), m_filePath(filepath)
	{
	}

	Texture::Texture(GLuint id, const char* const filepath)
		: id(id), width(0), height(0), m_filePath(filepath)
	{
	}

	Texture::Texture(const Texture& other)
		: id(other.id), width(other.width), height(other.height), m_filePath(other.m_filePath)
	{
	}
	
	bool Texture::operator==(const Texture& other)
	{
		return id == other.id;
	}

	//void Texture::operator=(const Texture& other)
	//{
	//	id = other.id;
	//	width = other.width;
	//	height = other.height;
	//	m_filePath = other.m_filePath;
	//}
}