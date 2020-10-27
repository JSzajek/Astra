#pragma once

#include <vector>
#include <unordered_map>
#include "../../utils/vendor/stb_image/stb_image.h"
#include <string>

#include "../buffers/VertexArray.h"
#include "../buffers/Texture.h"
#include "../buffers/CubeMapTexture.h"
#include "../buffers/FrameBuffer.h"
#include "../buffers/WaterFrameBuffer.h"
#include "../buffers/ShadowFrameBuffer.h"

namespace Astra::Graphics
{
	class Loader
	{
	private:
		std::vector<GLuint> m_vaos;
		std::vector<GLuint> m_vbos;
		std::unordered_map<std::string, Texture> m_textureDirectory;
		std::vector<GLuint> m_textureIds;
	public:
		Loader(const Loader&) = delete;
		void operator=(const Loader&) = delete;

		static Loader& Get()
		{
			static Loader instance;
			return instance;
		}
		
		static const VertexArray* Load(unsigned int drawType, const std::vector<float>& vertices, const std::vector<int>& indices, 
										const std::vector<float>& texturesCoords, const std::vector<float>& normals)
		{
			return Get().LoadImpl(drawType, vertices, indices, texturesCoords, normals);
		}

		static const VertexArray* Load(unsigned int drawType, const std::vector<float>& vertices, const std::vector<int>& indices,
										const std::vector<float>& textureCoords, const std::vector<float>& normals, const std::vector<float>& tangents)
		{
			return Get().LoadImpl(drawType, vertices, indices, textureCoords, normals, tangents);
		}

		static const VertexArray* Load(unsigned int drawType, const std::vector<float>& vertices, unsigned int dimensions)
		{
			return Get().LoadImpl(drawType, vertices , dimensions);
		}

		static const unsigned int Load(unsigned int drawType, const std::vector<float>& vertices, const std::vector<float>& textureCoords)
		{
			return Get().LoadImpl(drawType, vertices, textureCoords);
		}

		static const Texture& LoadTexture(const char* const filepath, GLint clippingOption = GL_REPEAT, bool flip = true)
		{
			return Get().LoadTextureImpl(filepath, clippingOption, flip);
		}

		static const Texture& LoadAtlasTexture(const char* const filepath)
		{
			return Get().LoadAtlasTextureImpl(filepath);
		}

		static const CubeMapTexture* LoadCubeMap(const std::vector<const char*>& filepaths)
		{
			return Get().LoadCubeMapImpl(filepaths);
		}

		static WaterFrameBuffer* LoadWaterFrameBuffer(unsigned int reflectionWidth, unsigned int reflectionHeight,
													  unsigned int refractionWidth, unsigned int refractionHeight)
		{
			return Get().LoadWaterFrameBufferImpl(reflectionWidth, reflectionHeight, refractionWidth, refractionHeight);
		}

		static ShadowFrameBuffer* LoadShadowFrameBuffer(unsigned int width, unsigned int height)
		{
			return Get().LoadShadowFrameBufferImpl(width, height);
		}

	private:
		Loader();
		~Loader();

		const VertexArray* LoadImpl(unsigned int drawType, const std::vector<float>& vertices,
									const std::vector<int>& indices, const std::vector<float>& texturesCoords,
									const std::vector<float>& normals);
		
		const VertexArray* LoadImpl(unsigned int drawType, const std::vector<float>& vertices, 
									const std::vector<int>& indices, const std::vector<float>& textureCoords, 
									const std::vector<float>& normals, const std::vector<float>& tangents);

		const VertexArray* LoadImpl(unsigned int drawType, const std::vector<float>& vertices, unsigned int dimensions);
		const GLuint LoadImpl(unsigned int drawType, const std::vector<float>& vertices, const std::vector<float>& textureCoords);
		
		const Texture& LoadAtlasTextureImpl(const char* const filepath);
		const Texture& LoadTextureImpl(const char* const filepath, GLint clippingOption, bool flip);
		
		const CubeMapTexture* LoadCubeMapImpl(const std::vector<const char*>& filepaths);

		WaterFrameBuffer* LoadWaterFrameBufferImpl(unsigned int reflectionWidth, unsigned int reflectionHeight,
												   unsigned int refractionWidth, unsigned int refractionHeight);
		ShadowFrameBuffer* LoadShadowFrameBufferImpl(unsigned int width, unsigned int height);

		const FrameBuffer& CreateFrameBuffer(int drawAttachment = GL_NONE, int readAttachment = GL_NONE);
		void CreateTextureAttachment(GLuint& id, unsigned int width, unsigned int height);
		GLuint CreateDepthTextureAttachment(GLuint& id, unsigned int width, unsigned int height, int filter = GL_LINEAR, int wrap = GL_REPEAT);
		void CreateDepthBufferAttachment(GLuint& id, unsigned int width, unsigned int height);

		GLuint BindInAttribBuffer(GLuint index, const std::vector<float>& data, int strideSize, GLenum usage = GL_STATIC_DRAW, GLboolean normalized = GL_FALSE);
		GLuint BindIndicesBuffer(const std::vector<int>& data, GLenum usage = GL_STATIC_DRAW);

		const GLuint& GenerateVaoId();
		const GLuint& GenerateVboId();
		
		void UnbindFrameBuffer();
		inline void UnbindVertexArray() { glBindVertexArray(0); }
	};
}