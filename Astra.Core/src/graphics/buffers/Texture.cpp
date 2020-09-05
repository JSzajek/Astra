#include "Texture.h"

namespace Astra::Graphics
{
	Texture::Texture(const char* const filepath)
		: id(0), m_filePath(filepath)
	{
		stbi_set_flip_vertically_on_load(1);
		buffer = stbi_load(std::string(filepath).c_str(), &width, &height, &m_bpp, 4);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &id);
	}

	void Texture::ClearBuffer()
	{
		if (buffer)
		{
			stbi_image_free(buffer);
			buffer = nullptr;
		}
	}
}