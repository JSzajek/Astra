#include "Loader.h"

#include "../../logger/Logger.h"
#include "../Window.h"

namespace Astra::Graphics
{ 
	Loader::Loader()
	{
	}

	Loader::~Loader()
	{
		for (const GLuint& vao : m_vaos)
		{
			glDeleteVertexArrays(1, &vao);
		}

		for (const GLuint& vbo : m_vbos)
		{
			glDeleteBuffers(1, &vbo);
		}

		for (const auto& texture : m_textureDirectory)
		{
			glDeleteTextures(1, &texture.second.id);
		}

		for (const GLuint& textureId : m_textureIds)
		{
			glDeleteTextures(1, &textureId);
		}
	}

	const VertexArray* Loader::LoadImpl(unsigned int drawType, const std::vector<float>& vertices,
		const std::vector<int>& indices, const std::vector<float>& texturesCoords,
		const std::vector<float>& normals)
	{
		GLuint id = GenerateVaoId();
		GLuint verticesID = BindInAttribBuffer(0, vertices, 3, GL_STATIC_DRAW);
		GLuint texturesID = BindInAttribBuffer(1, texturesCoords, 2, GL_STATIC_DRAW);
		GLuint normalsID = BindInAttribBuffer(2, normals, 3, GL_STATIC_DRAW);
		GLuint indicesID = BindIndicesBuffer(indices, GL_STATIC_DRAW);
		UnbindVertexArray();

		VertexArray* vertArray = new VertexArray(id, indices.size(), drawType);
		(*vertArray)(BufferType::Vertices) = verticesID;
		(*vertArray)(BufferType::Normals) = normalsID;
		(*vertArray)(BufferType::TextureCoords) = texturesID;
		(*vertArray)(BufferType::Indices) = indicesID;

		return vertArray;
	}

	const VertexArray* Loader::LoadImpl(unsigned int drawType, const std::vector<float>& vertices, unsigned int dimensions)
	{
		GLuint id = GenerateVaoId();
		GLuint vboId = BindInAttribBuffer(0, vertices, dimensions, GL_STATIC_DRAW);
		UnbindVertexArray();

		VertexArray* vertArray = new VertexArray(id, vertices.size() / dimensions, drawType);
		(*vertArray)(BufferType::Vertices) = vboId;
		return vertArray;
	}

	const Texture& Loader::LoadTextureImpl(const char* const filepath, GLint clippingOption)
	{
		static int m_bpp;
		static unsigned char* buffer;

		auto found = m_textureDirectory.find(filepath);
		if (found != m_textureDirectory.end())
		{
			return found->second;
		}

		Texture texture(filepath);

		stbi_set_flip_vertically_on_load(1);
		buffer = stbi_load(std::string(filepath).c_str(), &texture.width, &texture.height, &m_bpp, 4);
		
		if (buffer)
		{
			glGenTextures(1, &texture.id);
			glBindTexture(GL_TEXTURE_2D, texture.id);


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clippingOption);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clippingOption);
		
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -2);
		
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			glGenerateMipmap(GL_TEXTURE_2D);
		
			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(buffer);

			m_textureDirectory[filepath] = texture;
			return texture;
		}
		Logger::LogWarning(std::string("Texture ") + std::string(filepath) + std::string(" did not load correctly."));
		return NULL;
	}

	const CubeMapTexture* Loader::LoadCubeMapImpl(const std::vector<const char*>& filepaths)
	{
		static int m_bpp;
		static unsigned char* buffer;

		CubeMapTexture* resultTexture = new CubeMapTexture(filepaths);
		CubeMapTexture& cubemapTexture = *resultTexture;

		glGenTextures(1, &cubemapTexture.id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture.id);
		
		stbi_set_flip_vertically_on_load(1);
		for (int i = 0; i < filepaths.size(); i++)
		{
			Texture& texture = cubemapTexture[i];
			buffer = stbi_load(std::string(texture.m_filePath).c_str(), &texture.width, &texture.height, &m_bpp, 4);
			if (buffer)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
				stbi_image_free(buffer);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_textureIds.push_back(cubemapTexture.id);

		return resultTexture;
	}

	const WaterFrameBuffer& Loader::LoadWaterFrameBufferImpl(unsigned int reflectionWidth, unsigned int reflectionHeight,
															  unsigned int refractionWidth, unsigned int refractionHeight)
	{
		static unsigned int hashId = 1;
		FrameBuffer reflection = CreateFrameBuffer();
		CreateTextureAttachment(reflection.ColorAttachment(), reflectionWidth, reflectionHeight);
		CreateDepthBufferAttachment(reflection.DepthAttachment(), reflectionWidth, reflectionHeight);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::LogError("Error Incomplete Reflection FBO.");
		}
		UnbindFrameBuffer();

		FrameBuffer refraction = CreateFrameBuffer();
		CreateTextureAttachment(refraction.ColorAttachment(), refractionWidth, refractionWidth);
		CreateDepthTextureAttachment(refraction.DepthAttachment(), refractionWidth, refractionWidth);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::LogError("Error Incomplete Refraction FBO.");
		}
		UnbindFrameBuffer();

		WaterFrameBuffer waterFrameBuffer(reflection, refraction);
		waterFrameBuffer.id = hashId++;
		return waterFrameBuffer;
	}

	const FrameBuffer& Loader::CreateFrameBuffer()
	{
		FrameBuffer buffer;
		glGenFramebuffers(1, &buffer.Id());
		glBindFramebuffer(GL_FRAMEBUFFER, buffer.Id());
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		return buffer;
	}

	void Loader::CreateTextureAttachment(GLuint& id, unsigned int width, unsigned int height)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
	}

	void Loader::CreateDepthTextureAttachment(GLuint& id, unsigned int width, unsigned int height)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, id, 0);
	}

	void Loader::CreateDepthBufferAttachment(GLuint& id, unsigned int width, unsigned int height)
	{
		glGenRenderbuffers(1, &id);
		glBindRenderbuffer(GL_RENDERBUFFER, id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id);
	}

	GLuint Loader::BindInAttribBuffer(GLuint index, const std::vector<float>& data, int strideSize, GLenum usage, GLboolean normalized)
	{
		GLuint id = GenerateVboId();
		glBindBuffer(GL_ARRAY_BUFFER, id);

		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], usage);
		glVertexAttribPointer(index, strideSize, GL_FLOAT, normalized, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return id;
	}

	GLuint Loader::BindIndicesBuffer(const std::vector<int>& data, GLenum usage)
	{
		GLuint id = GenerateVboId();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(int), data.data(), usage);
		return id;
	}

	const GLuint& Loader::GenerateVaoId()
	{
		GLuint vaoId;
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);
		m_vaos.emplace_back(vaoId);
		return vaoId;
	}

	const GLuint& Loader::GenerateVboId()
	{
		GLuint vboId;
		glGenBuffers(1, &vboId);
		m_vbos.emplace_back(vboId);
		return vboId;
	}
	
	void Loader::UnbindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Window::width, Window::height);
	}
}