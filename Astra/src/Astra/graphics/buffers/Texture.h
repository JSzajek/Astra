#pragma once

namespace Astra::Graphics
{
	class Texture
	{
	public:
		static const char* const DefaultTexture;
		static const char* const DefaultSpecular;
	public:
		unsigned int id;
		int width, height;
		const char* m_filePath;
		bool hdr;
	public:
		Texture();
		Texture(const char* const filepath);
		Texture(unsigned int id, const char* const filepath);
		~Texture();

		bool operator==(const Texture& other);
	};
}