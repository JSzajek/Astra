#pragma once

namespace Astra::Graphics
{
	enum TextureType : char
	{
		None = -1,
		DiffuseMap = 0,
		SpecularMap = 1,
		NormalMap = 2,
		HeightMap = 3,
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
		std::string m_filePath;
#endif
		bool hdr;
		TextureType type = TextureType::None;
	public:
		Texture();
		Texture(const char* const filepath);
		Texture(std::string filepath);
		Texture(unsigned int id, const char* const filepath);
		~Texture();

		inline bool operator==(const Texture& other) { return id == other.id; }
	};

	struct TextureCreationSpec
	{
		std::string filepath;
		bool diffuse = false;
		bool flipped = true;

		std::string directory;
		const void* scene = NULL;

		TextureCreationSpec(const char* const filepath, bool diffuse = false, bool flipped = true)
			: filepath(filepath), diffuse(diffuse), flipped(flipped)
		{
		}

		TextureCreationSpec(const char* const filepath, std::string directory, const void* scene, bool diffuse = false, bool flipped = true)
			: filepath(filepath), directory(directory), diffuse(diffuse), scene(scene), flipped(flipped)
		{
		}
	};
}