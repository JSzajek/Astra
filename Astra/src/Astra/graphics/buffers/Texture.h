#pragma once

namespace Astra::Graphics
{
	enum TextureType : unsigned char
	{
		DiffuseMap	= 0,
		SpecularMap = 1,
		NormalMap	= 2,
		HeightMap	= 3,
		EmissionMap = 4
	};

	class Texture
	{
	public:
		static const char* const DefaultTexture;
		static const char* const DefaultSpecular;
	public:
		unsigned int id;
		int width, height; // Used in gui textures - TODO
	#if ASTRA_DEBUG
		const char* m_filePath;
	#endif
		bool hdr;
		TextureType type;
	public:
		Texture();
		Texture(const char* const filepath);
		Texture(unsigned int id, const char* const filepath);
		~Texture();

		inline bool operator==(const Texture& other) { return id == other.id; }
	};
}