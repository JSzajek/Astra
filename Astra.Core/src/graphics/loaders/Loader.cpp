#include "Loader.h"

#include "../../logger/Logger.h"
#include "../ResourceManager.h"
#include "../Window.h"

namespace Astra::Graphics
{ 
	Loader::Loader()
	{
	}

	const VertexArray* Loader::LoadImpl(unsigned int drawType, const std::vector<float>& vertices,
										const std::vector<int>& indices, const std::vector<float>& textureCoords,
										const std::vector<float>& normals)
	{
		GLuint id = GenerateVaoId();
		GLuint verticesID = BindInAttribBuffer(0, vertices, 3);
		GLuint texturesID = BindInAttribBuffer(1, textureCoords, 2);
		GLuint normalsID = BindInAttribBuffer(2, normals, 3);
		static_cast<void>(BindIndicesBuffer(indices));
		UnbindVertexArray();

		VertexArray* vertArray = new VertexArray(id, indices.size(), drawType);
		ResourceManager::Track(vertArray);

		(*vertArray)(BufferType::Vertices) = verticesID;
		(*vertArray)(BufferType::Normals) = normalsID;
		(*vertArray)(BufferType::TextureCoords) = texturesID;

		return vertArray;
	}

	const VertexArray* Loader::LoadImpl(unsigned int drawType, const std::vector<float>& vertices, 
										const std::vector<int>& indices, const std::vector<float>& textureCoords, 
										const std::vector<float>& normals, const std::vector<float>& tangents)
	{
		GLuint id = GenerateVaoId();
		GLuint verticesID = BindInAttribBuffer(0, vertices, 3);
		GLuint texturesID = BindInAttribBuffer(1, textureCoords, 2);
		GLuint normalsID = BindInAttribBuffer(2, normals, 3);
		static_cast<void>(BindIndicesBuffer(indices));
		GLuint tangentsID = BindInAttribBuffer(3, tangents, 3);
		UnbindVertexArray();

		VertexArray* vertArray = new VertexArray(id, indices.size(), drawType);
		ResourceManager::Track(vertArray);

		(*vertArray)(BufferType::Vertices) = verticesID;
		(*vertArray)(BufferType::Normals) = normalsID;
		(*vertArray)(BufferType::TextureCoords) = texturesID;
		(*vertArray)(BufferType::Tangents) = tangentsID;

		return vertArray;
	}

	const VertexArray* Loader::LoadImpl(unsigned int drawType, const std::vector<float>& vertices, unsigned int dimensions)
	{
		GLuint id = GenerateVaoId();
		GLuint vboId = BindInAttribBuffer(0, vertices, dimensions);
		UnbindVertexArray();

		VertexArray* vertArray = new VertexArray(id, vertices.size() / dimensions, drawType);
		ResourceManager::Track(vertArray);

		(*vertArray)(BufferType::Vertices) = vboId;
		return vertArray;
	}

	const GLuint Loader::LoadImpl(unsigned int drawType, const std::vector<float>& vertices, const std::vector<float>& textureCoords)
	{
		GLuint id = GenerateVaoId();
		GLuint verticesID = BindInAttribBuffer(0, vertices, 2);
		GLuint texturesID = BindInAttribBuffer(1, textureCoords, 2);
		UnbindVertexArray();
		return id;
	}

	const Texture* Loader::LoadAtlasTextureImpl(const char* const filepath)
	{
		static int m_bpp;
		static unsigned char* buffer;

		Texture* texture = NULL;
		if (ResourceManager::QueryTexture(filepath, &texture))
		{
			return texture;
		}
		else if (texture)
		{
			stbi_set_flip_vertically_on_load(0);
			buffer = stbi_load(std::string(filepath).c_str(), &texture->width, &texture->height, &m_bpp, 4);

			if (!buffer)
			{
				Logger::LogWarning(std::string("Texture ") + std::string(filepath) + std::string(" did not load correctly."));
				return NULL;
			}

			glGenTextures(1, &texture->id);
			glBindTexture(GL_TEXTURE_2D, texture->id);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(buffer);

			return texture;
		}
		Logger::LogError(std::string("Loader Error in Texture Initialization"));
		return NULL;
	}

	const Texture* Loader::LoadTextureImpl(const char* const filepath, bool diffuse, GLint clippingOption, bool flip, bool invert)
	{
		static int m_bpp;
		static unsigned char* buffer;

		Texture* texture = NULL;
		if (ResourceManager::QueryTexture(filepath, &texture))
		{
			return texture;
		}
		else if (texture)
		{
			stbi_set_flip_vertically_on_load(flip);
			buffer = stbi_load(std::string(filepath).c_str(), &texture->width, &texture->height, &m_bpp, !invert ? 4 : 1);

			if (!buffer)
			{
				Logger::LogWarning(std::string("Texture ") + std::string(filepath) + std::string(" did not load correctly."));
				return NULL;
			}
			glGenTextures(1, &texture->id);
			glBindTexture(GL_TEXTURE_2D, texture->id);
		#if HDR
			glTexImage2D(GL_TEXTURE_2D, 0, diffuse ? GL_SRGB8_ALPHA8 : GL_RGBA8, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		#else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		#endif
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clippingOption);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clippingOption);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			if (glfwExtensionSupported("GL_EXT_texture_filter_anisotropic"))
			{
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
				float maxValue;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxValue);
				float amount = fminf(4.0f, maxValue);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);
			}
			else
			{
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.2f);
				Logger::LogWarning("Anisotropic Filtering Not Supported");
			}

			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(buffer);

			return texture;
		}
		Logger::LogError(std::string("Loader Error in Texture Initialization"));
		return NULL;
	}

	const CubeMapTexture* Loader::LoadCubeMapImpl(const std::vector<const char*>& filepaths)
	{
		static int m_bpp;
		static unsigned char* buffer;

		CubeMapTexture* resultTexture = new CubeMapTexture(filepaths);

		glGenTextures(1, &resultTexture->id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, resultTexture->id);
		stbi_set_flip_vertically_on_load(0); // Don't flip?

		for (size_t i = 0; i < filepaths.size(); i++)
		{
			auto* texture = (*resultTexture)[i];
			buffer = stbi_load(std::string(texture->m_filePath).c_str(), &texture->width, &texture->height, &m_bpp, 4);
			if (buffer)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
				stbi_image_free(buffer);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		return resultTexture;
	}

	WaterFrameBuffer* Loader::LoadWaterFrameBufferImpl(unsigned int reflectionWidth, unsigned int reflectionHeight,
															  unsigned int refractionWidth, unsigned int refractionHeight)
	{
		FrameBuffer* reflection = CreateFrameBuffer(DepthBufferType::None, false, GL_COLOR_ATTACHMENT0);
		CreateTextureAttachment(reflection->ColorAttachment(), reflectionWidth, reflectionHeight, HDR);
		CreateDepthBufferAttachment(reflection->DepthAttachment(), reflectionWidth, reflectionHeight);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::LogError("Error Incomplete Reflection FBO.");
		}
		UnbindFrameBuffer();

		FrameBuffer* refraction = CreateFrameBuffer(DepthBufferType::None, false, GL_COLOR_ATTACHMENT0);
		CreateTextureAttachment(refraction->ColorAttachment(), refractionWidth, refractionHeight, HDR);
		static_cast<void>(CreateDepthTextureAttachment(refraction->DepthAttachment(), refractionWidth, refractionHeight));
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::LogError("Error Incomplete Refraction FBO.");
		}
		UnbindFrameBuffer();

		WaterFrameBuffer* waterFrameBuffer = new WaterFrameBuffer(reflection, refraction, 
																  reflectionWidth, reflectionHeight,
																  refractionWidth, refractionHeight);
		return waterFrameBuffer;
	}

	ShadowFrameBuffer* Loader::LoadShadowFrameBufferImpl(unsigned int width, unsigned int height)
	{
		FrameBuffer* buffer = CreateFrameBuffer(DepthBufferType::None);
		GLuint id = CreateDepthTextureAttachment(buffer->DepthAttachment(), width, height, GL_DEPTH_COMPONENT16, GL_NEAREST, GL_CLAMP_TO_EDGE);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::LogError("Error Incomplete Shadow FBO.");
		}
		UnbindFrameBuffer();

		ShadowFrameBuffer* shadowFrameBuffer = new ShadowFrameBuffer(buffer, id, width, height);
		return shadowFrameBuffer;
	}

	FrameBuffer* Loader::LoadFrameBufferImpl(unsigned int width, unsigned int height, bool multisampled, DepthBufferType depthType, bool floating, unsigned int wrapping)
	{
		FrameBuffer* buffer = CreateFrameBuffer(depthType, multisampled, GL_COLOR_ATTACHMENT0, multisampled ? GL_COLOR_ATTACHMENT0 : GL_NONE);
		if (!multisampled)
		{
			CreateTextureAttachment(buffer->ColorAttachment(), width, height, floating, wrapping);
		}
		else
		{
			CreateColorBufferAttachment(buffer->ColorAttachment(), width, height, multisampled, floating);
		}

		if (depthType == DepthBufferType::Render)
		{
			CreateDepthBufferAttachment(buffer->DepthAttachment(), width, height, multisampled, floating);
		}
		else if (depthType == DepthBufferType::Texture)
		{
			CreateDepthTextureAttachment(buffer->DepthAttachment(), width, height, GL_DEPTH_COMPONENT24);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::LogError("Error Incomplete FBO.");
		}
		UnbindFrameBuffer();

		return buffer;
	}

	FrameBuffer* Loader::LoadMultiTargetFrameBufferImpl(unsigned int width, unsigned int height, size_t colorAttachments, size_t depthAttachments, bool floating)
	{
		FrameBuffer* buffer = new FrameBuffer(DepthBufferType::None, false, colorAttachments, depthAttachments);
		ResourceManager::Track(buffer);
		glGenFramebuffers(1, &buffer->Id());
		glBindFramebuffer(GL_FRAMEBUFFER, buffer->Id());
		
		std::vector<unsigned int> attachments;
		for (size_t i = 0; i < colorAttachments; i++)
		{
			CreateTextureAttachment(buffer->ColorAttachment(i), width, height, floating, GL_CLAMP_TO_EDGE, i);
			attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
		glDrawBuffers(colorAttachments, &attachments[0]);
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::LogError("Error Incomplete FBO.");
		}
		UnbindFrameBuffer();

		return buffer;
	}

	void Loader::UpdateFrameBufferImpl(FrameBuffer* buffer, unsigned int width, unsigned int height, bool floating, bool multisampled)
	{
		if (buffer->GetColorAttachment() != 0)
		{
			if (multisampled)
			{
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, buffer->GetColorAttachment());
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MULTI_SAMPLE_SIZE, floating ? GL_RGBA16F : GL_RGBA, width, height, GL_TRUE);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			}
			else
			{
				for (size_t i = 0; i < buffer->GetColorAttachmentCount(); i++)
				{
					glBindTexture(GL_TEXTURE_2D, buffer->GetColorAttachment(i));
				#if HDR
					glTexImage2D(GL_TEXTURE_2D, 0, floating ? GL_RGBA16F : GL_RGBA, width, height, 0, GL_RGBA, floating ? GL_FLOAT : GL_UNSIGNED_BYTE, NULL);
				#else
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, floating ? GL_FLOAT : GL_UNSIGNED_BYTE, NULL);
				#endif
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
		}
		if (buffer->GetType() == DepthBufferType::Texture)
		{
			if (buffer->GetDepthAttachment() != 0)
			{
				glBindTexture(GL_TEXTURE_2D, buffer->GetDepthAttachment());
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		else if (buffer->GetType() == DepthBufferType::Render)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, buffer->GetDepthAttachment());
			if (!multisampled)
			{
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			}
			else
			{
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, MULTI_SAMPLE_SIZE, GL_DEPTH24_STENCIL8, width, height);
			}
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
	}

	FrameBuffer* Loader::CreateFrameBuffer(DepthBufferType type, bool multisampled, int drawAttachment, int readAttachment)
	{
		FrameBuffer* buffer = new FrameBuffer(type, multisampled);
		ResourceManager::Track(buffer);
		glGenFramebuffers(1, &buffer->Id());
		glBindFramebuffer(GL_FRAMEBUFFER, buffer->Id());
		glDrawBuffer(drawAttachment);
		glReadBuffer(readAttachment);
		return buffer;
	}

	void Loader::CreateTextureAttachment(GLuint& id, unsigned int width, unsigned int height, bool floating, unsigned int wrapping, size_t offset)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
	#if HDR
		glTexImage2D(GL_TEXTURE_2D, 0, floating ? GL_RGBA16F : GL_RGBA, width, height, 0, GL_RGBA, floating ? GL_FLOAT : GL_UNSIGNED_BYTE, NULL);
	#else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, floating ? GL_FLOAT : GL_UNSIGNED_BYTE, NULL);
	#endif
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + offset, GL_TEXTURE_2D, id, 0);
	}

	GLuint Loader::CreateDepthTextureAttachment(GLuint& id, unsigned int width, unsigned int height, int component, int filter, int wrap)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, component, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, id, 0);
		return id;
	}

	void Loader::CreateDepthBufferAttachment(GLuint& id, unsigned int width, unsigned int height, bool multisampled, bool floating)
	{
		glGenRenderbuffers(1, &id);
		glBindRenderbuffer(GL_RENDERBUFFER, id);
		if (!multisampled)
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		}
		else 
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, MULTI_SAMPLE_SIZE, GL_DEPTH24_STENCIL8, width, height);
		}
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);
	}

	void Loader::CreateColorBufferAttachment(GLuint& id, unsigned int width, unsigned int height, bool multisampled, bool floating)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
	#if HDR
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MULTI_SAMPLE_SIZE, floating ? GL_RGBA16F : GL_RGBA, width, height, GL_TRUE);
	#else
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MULTI_SAMPLE_SIZE, GL_RGBA, width, height, GL_TRUE);
	#endif
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, id, 0);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
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

	GLuint Loader::GenerateVaoId()
	{
		GLuint vaoId;
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);
		return vaoId;
	}

	GLuint Loader::GenerateVboId()
	{
		GLuint vboId;
		glGenBuffers(1, &vboId);
		return vboId;
	}
	
	void Loader::UnbindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
	}
}