#pragma once

#include "Astra/Res.h"

namespace Astra::Graphics
{
	enum TextureType : char
	{
		None = -1,
		DiffuseMap	= 0,
		SpecularMap = 1,
		NormalMap	= 2,
		HeightMap	= 3,
		EmissionMap = 4
	};

	class Texture : public Res
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

		inline bool operator==(const Texture& other) { return id == other.id; }

		virtual void Free() override;
	};
}