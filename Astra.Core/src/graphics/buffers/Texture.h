#pragma once

#include <GL/glew.h>
#include "../../utils/vendor/stb_image/stb_image.h"
#include <string>

namespace Astra::Graphics
{
	class Texture
	{
	public:
		GLuint id;
		int width, height;
		unsigned char* buffer;
		
	private:
		const char* const m_filePath;
		int m_bpp;
	public:
		Texture(const char* const filepath);
		~Texture();
		void ClearBuffer();
	};
}