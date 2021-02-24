#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "Astra/graphics/buffers/VertexArray.h"
#include "Astra/graphics/buffers/FrameBuffer.h"
#include "Astra/graphics/buffers/WaterFrameBuffer.h"
#include "Astra/graphics/buffers/ShadowFrameBuffer.h"

namespace Astra::Graphics
{
	class Loader
	{
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

		static WaterFrameBuffer* LoadWaterFrameBuffer(unsigned int reflectionWidth, unsigned int reflectionHeight,
													  unsigned int refractionWidth, unsigned int refractionHeight)
		{
			return Get().LoadWaterFrameBufferImpl(reflectionWidth, reflectionHeight, refractionWidth, refractionHeight);
		}

		static ShadowFrameBuffer* LoadShadowFrameBuffer(unsigned int width, unsigned int height)
		{
			return Get().LoadShadowFrameBufferImpl(width, height);
		}

		static FrameBuffer* LoadFrameBuffer(unsigned int width, unsigned int height, unsigned int multisampled = 0, DepthBufferType depthType = DepthBufferType::None, bool floating = false, unsigned int wrapping = GL_REPEAT)
		{
			return Get().LoadFrameBufferImpl(width, height, multisampled, depthType, floating, wrapping);
		}

		static FrameBuffer* LoadMultiTargetFrameBuffer(unsigned int width, unsigned int height, size_t colorAttachments, size_t depthAttachments = 1, bool floating = false)
		{
			return Get().LoadMultiTargetFrameBufferImpl(width, height, colorAttachments, depthAttachments, floating);
		}

		static void UpdateFrameBuffer(FrameBuffer* buffer, unsigned int width, unsigned int height, bool floating, unsigned int multisampled)
		{
			return Get().UpdateFrameBufferImpl(buffer, width, height, floating, multisampled);
		}

	private:
		Loader();

		const VertexArray* LoadImpl(unsigned int drawType, const std::vector<float>& vertices,
									const std::vector<int>& indices, const std::vector<float>& texturesCoords,
									const std::vector<float>& normals);
		
		const VertexArray* LoadImpl(unsigned int drawType, const std::vector<float>& vertices, 
									const std::vector<int>& indices, const std::vector<float>& textureCoords, 
									const std::vector<float>& normals, const std::vector<float>& tangents);

		const VertexArray* LoadImpl(unsigned int drawType, const std::vector<float>& vertices, unsigned int dimensions);
		const GLuint LoadImpl(unsigned int drawType, const std::vector<float>& vertices, const std::vector<float>& textureCoords);

		WaterFrameBuffer* LoadWaterFrameBufferImpl(unsigned int reflectionWidth, unsigned int reflectionHeight,
												   unsigned int refractionWidth, unsigned int refractionHeight);
		ShadowFrameBuffer* LoadShadowFrameBufferImpl(unsigned int width, unsigned int height);
		FrameBuffer* LoadFrameBufferImpl(unsigned int width, unsigned int height, unsigned int multisampled, DepthBufferType depthType, bool floating, unsigned int component);
		FrameBuffer* LoadMultiTargetFrameBufferImpl(unsigned int width, unsigned int height, size_t colorAttachments, size_t depthAttachments, bool floating);


		void UpdateFrameBufferImpl(FrameBuffer* buffer, unsigned int width, unsigned int height, bool floating, unsigned int multisampled);

		FrameBuffer* CreateFrameBuffer(DepthBufferType type, bool multisampled = false, int drawAttachment = GL_NONE, int readAttachment = GL_NONE);
		void CreateTextureAttachment(GLuint& id, unsigned int width, unsigned int height, bool floating, unsigned int wrapping = GL_REPEAT, size_t offset = 0);
		GLuint CreateDepthTextureAttachment(GLuint& id, unsigned int width, unsigned int height, int component = GL_DEPTH24_STENCIL8, int filter = GL_LINEAR, int wrap = GL_REPEAT);
		void CreateDepthBufferAttachment(GLuint& id, unsigned int width, unsigned int height, unsigned int multisampled = 0, bool floating = false);
		void CreateColorBufferAttachment(GLuint& id, unsigned int width, unsigned int height, unsigned int multisampled = 0, bool floating = false);
		GLuint BindInAttribBuffer(GLuint index, const std::vector<float>& data, int strideSize, GLenum usage = GL_STATIC_DRAW, GLboolean normalized = GL_FALSE);
		GLuint BindIndicesBuffer(const std::vector<int>& data, GLenum usage = GL_STATIC_DRAW);

		GLuint GenerateVaoId();
		GLuint GenerateVboId();
		
		void UnbindFrameBuffer();
		inline void UnbindVertexArray() { glBindVertexArray(0); }
	};
}