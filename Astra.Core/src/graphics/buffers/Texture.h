#pragma once

#include <GL/glew.h>

namespace Astra::Graphics
{
	class Texture
	{
	public:
		static constexpr const char* const DefaultTexture = "../Astra.Core/src/resources/textures/Default.jpg";
		static constexpr const char* const DefaultSpecular = "../Astra.Core/src/resources/textures/DefaultSpecular.jpg";
	public:
		GLuint id;
		int width, height;
		const char* m_filePath;
	public:
		Texture();
		Texture(const char* const filepath);
		Texture(GLuint id, const char* const filepath);
		Texture(const Texture& other);

		bool operator==(const Texture& other);
	};
}