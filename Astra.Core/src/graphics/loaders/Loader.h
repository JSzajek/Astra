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


		static const Texture& LoadTexture(const char* const filepath, GLint clippingOption = GL_REPEAT)
		{
			return Get().LoadTextureImpl(filepath, clippingOption);
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
		
		const Texture& LoadTextureImpl(const char* const filepath, GLint clippingOption);
		
		const CubeMapTexture* LoadCubeMapImpl(const std::vector<const char*>& filepaths);

		WaterFrameBuffer* LoadWaterFrameBufferImpl(unsigned int reflectionWidth, unsigned int reflectionHeight,
															unsigned int refractionWidth, unsigned int refractionHeight);
		
		const FrameBuffer& CreateFrameBuffer();
		void CreateTextureAttachment(GLuint& id, unsigned int width, unsigned int height);
		void CreateDepthTextureAttachment(GLuint& id, unsigned int width, unsigned int height);
		void CreateDepthBufferAttachment(GLuint& id, unsigned int width, unsigned int height);

		GLuint BindInAttribBuffer(GLuint index, const std::vector<float>& data, int strideSize, GLenum usage = GL_STATIC_DRAW, GLboolean normalized = GL_FALSE);
		GLuint BindIndicesBuffer(const std::vector<int>& data, GLenum usage = GL_STATIC_DRAW);

		const GLuint& GenerateVaoId();
		const GLuint& GenerateVboId();
		
		void UnbindFrameBuffer();
		inline void UnbindVertexArray() { glBindVertexArray(0); }
	};
}