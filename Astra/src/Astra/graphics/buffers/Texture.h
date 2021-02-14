#pragma once

namespace Astra::Graphics
{
	class Texture
	{
	public:
		static constexpr const char* const DefaultTexture = "../Astra/res/textures/Default.jpg";
		static constexpr const char* const DefaultSpecular = "../Astra/res/textures/DefaultSpecular.jpg";
	public:
		unsigned int id;
		int width, height;
		const char* m_filePath;
	public:
		Texture();
		Texture(const char* const filepath);
		Texture(unsigned int id, const char* const filepath);
		~Texture();

		bool operator==(const Texture& other);
	};
}