#include "astra_pch.h"

#include <stb_image/stb_image.h>
#include <GLFW\glfw3.h>

#include "Loader.h"
#include "Astra/Application.h"
#include "Astra/graphics/ResourceManager.h"

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

	const Texture* Loader::LoadFontAtlasTextureImpl(const char* const filepath, unsigned int fontSize, const std::vector<unsigned char>& data, unsigned int width, unsigned int height)
	{
		Texture* texture = NULL;
		if (ResourceManager::QueryFontAtlasTexture(filepath, fontSize, &texture))
		{
			return texture;
		}
		else if (texture)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glGenTextures(1, &texture->id);
			glBindTexture(GL_TEXTURE_2D, texture->id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, &data[0]);

			texture->width = width;
			texture->height = height;

			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Reset unpacking alignment to default
			
			glBindTexture(GL_TEXTURE_2D, 0);

			return texture;
		}
		ASTRA_CORE_ERROR("Loader Error in Font Atlas Texture Initialization.");
		return NULL;
	}

	Texture* Loader::LoadTextureImpl(const char* const filepath, bool diffuse, GLint clippingOption, bool flip, bool invert)
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
			int width, height;
			buffer = stbi_load(std::string(filepath).c_str(), &width, &height, &m_bpp, !invert ? 4 : 1);

			if (!buffer)
			{
				ASTRA_CORE_WARN("Texture {0} Did Not Load Correctly.", filepath);
				return NULL;
			}
			auto hdr = Application::Get().GetWindow().IsHDR();
			texture->width = width;
			texture->height = height;

			glGenTextures(1, &texture->id);
			glBindTexture(GL_TEXTURE_2D, texture->id);
			glTexImage2D(GL_TEXTURE_2D, 0, hdr && diffuse ? GL_SRGB8_ALPHA8 : GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clippingOption);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clippingOption);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, diffuse ? GL_LINEAR : GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			texture->hdr = hdr;

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
				ASTRA_CORE_WARN("Anisotropic Filtering Not Supported");
			}

			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(buffer);

			return texture;
		}
		ASTRA_CORE_ERROR("Loader Error in Texture Initialization.");
		return NULL;
	}

	void Loader::LoadTextureDataImpl(Texture* texture, unsigned char* data, int width, int height, int nrComponents, bool diffuse)
	{
		ASTRA_CORE_ASSERT(data, "Model: Texture Data Failed to Load.");

		unsigned int textureId;
		glGenTextures(1, &textureId);
		auto hdr = Application::Get().GetWindow().IsHDR();

		GLenum format = nrComponents == 1 ? GL_RED : nrComponents == 3 ? GL_RGB : GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, diffuse ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		
		texture->id = textureId;
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Loader::UpdateDiffuseTextureImpl(Texture* texture, bool hdrEnabled)
	{
		if (texture->hdr == hdrEnabled) { return; } // Already updated format

		int width, height;
		glBindTexture(GL_TEXTURE_2D, texture->id);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

		void* data = malloc(sizeof(float) * width * height * 4 /*RGBA*/); // Allocate Enough Space For Image

		// Gathers Image Data from Texture Buffer based on ID and re-buffers with new internal format
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexImage2D(GL_TEXTURE_2D, 0, hdrEnabled ? GL_SRGB8_ALPHA8 : GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		texture->hdr = hdrEnabled; // Update hdr status

		free(data); // Free Image Data
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	CubeMapTexture* Loader::LoadCubeMapImpl(const std::vector<const char*>& filepaths)
	{
		static int m_bpp;
		static unsigned char* buffer;

		CubeMapTexture* texture = NULL;
		if (ResourceManager::QueryTexture(filepaths, &texture))
		{
			return texture;
		}
		else if (texture)
		{
			glGenTextures(1, &texture->id);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id);
			stbi_set_flip_vertically_on_load(0); // Don't flip?
			
			auto hdr = Application::Get().GetWindow().IsHDR();
			
			int width, height;
			for (size_t i = 0; i < filepaths.size(); i++)
			{
				auto* tex = (*texture)[i];
				buffer = stbi_load(std::string(tex->m_filePath).c_str(), &width, &height, &m_bpp, 4);
				if (!buffer)
				{
					ASTRA_CORE_WARN("Cube Map Texture {0} Did Not Load Correctly.", tex->m_filePath);
					return NULL;
				}

				if (buffer)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, hdr ? GL_SRGB8_ALPHA8 : GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
					stbi_image_free(buffer);
				}
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			return texture;
		}
		ASTRA_CORE_ERROR("Loader Error in Cube Map Texture Initialization.");
		return NULL;
	}

	void Loader::UpdateCubeMapImpl(CubeMapTexture* texture, bool hdrEnabled)
	{
		//glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // Only grabs one level of 3d texture - Investigate
		static int m_bpp;
		static unsigned char* buffer;

		if (texture->hdr == hdrEnabled)
		{
			return;
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id);
		stbi_set_flip_vertically_on_load(false); // Don't flip?

		int width, height;
		for (size_t i = 0; i < texture->GetFiles().size(); i++)
		{
			auto* tex = (*texture)[i];
			buffer = stbi_load(std::string(tex->m_filePath).c_str(), &width, &height, &m_bpp, 4);
			if (!buffer)
			{
				ASTRA_CORE_WARN("Cube Map Texture {0} Did Not Load Correctly.", tex->m_filePath);
				return;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, hdrEnabled ? GL_SRGB8_ALPHA8 : GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			stbi_image_free(buffer);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		texture->hdr = hdrEnabled;
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	WaterFrameBuffer* Loader::LoadWaterFrameBufferImpl(unsigned int reflectionWidth, unsigned int reflectionHeight,
															  unsigned int refractionWidth, unsigned int refractionHeight)
	{
		auto hdr = Application::Get().GetWindow().IsHDR();

		FrameBuffer* reflection = CreateFrameBuffer(DepthBufferType::None, false, GL_COLOR_ATTACHMENT0);
		CreateTextureAttachment(reflection->ColorAttachment(), reflectionWidth, reflectionHeight, hdr);
		CreateDepthBufferAttachment(reflection->DepthAttachment(), reflectionWidth, reflectionHeight);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			ASTRA_CORE_ERROR("WaterFrameBuffer Error Incomplete Reflection FBO.");
		}
		UnbindFrameBuffer();

		FrameBuffer* refraction = CreateFrameBuffer(DepthBufferType::None, false, GL_COLOR_ATTACHMENT0);
		CreateTextureAttachment(refraction->ColorAttachment(), refractionWidth, refractionHeight, hdr);
		static_cast<void>(CreateDepthTextureAttachment(refraction->DepthAttachment(), refractionWidth, refractionHeight));
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			ASTRA_CORE_ERROR("WaterFrameBuffer Error Incomplete Refraction FBO.");
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
			ASTRA_CORE_ERROR("ShadowFrameBuffer Error Incomplete Shadow FBO.");
		}
		UnbindFrameBuffer();

		ShadowFrameBuffer* shadowFrameBuffer = new ShadowFrameBuffer(buffer, id, width, height);
		return shadowFrameBuffer;
	}

	FrameBuffer* Loader::LoadFrameBufferImpl(unsigned int width, unsigned int height, unsigned int multisampled, DepthBufferType depthType, bool floating, unsigned int wrapping)
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
			CreateDepthTextureAttachment(buffer->DepthAttachment(), width, height, GL_DEPTH24_STENCIL8);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			ASTRA_CORE_ERROR("FrameBuffer Error Incomplete FBO.");
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
			ASTRA_CORE_ERROR("FrameBuffer Error Incomplete FBO.");
		}
		UnbindFrameBuffer();

		return buffer;
	}

	void Loader::UpdateFrameBufferImpl(FrameBuffer* buffer, unsigned int width, unsigned int height, bool floating, unsigned int multisampled)
	{
		if (buffer->GetColorAttachment() != 0)
		{
			if (multisampled)
			{
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, buffer->GetColorAttachment());
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, multisampled, floating ? GL_RGBA16F : GL_RGBA, width, height, GL_TRUE);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			}
			else
			{
				for (size_t i = 0; i < buffer->GetColorAttachmentCount(); i++)
				{
					glBindTexture(GL_TEXTURE_2D, buffer->GetColorAttachment(i));
					glTexImage2D(GL_TEXTURE_2D, 0, floating ? GL_RGBA16F : GL_RGBA, width, height, 0, GL_RGBA, floating ? GL_FLOAT : GL_UNSIGNED_BYTE, NULL);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
		}
		if (buffer->GetType() == DepthBufferType::Texture)
		{
			if (buffer->GetDepthAttachment() != 0)
			{
				glBindTexture(GL_TEXTURE_2D, buffer->GetDepthAttachment());
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisampled, GL_DEPTH24_STENCIL8, width, height);
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
		glTexImage2D(GL_TEXTURE_2D, 0, floating ? GL_RGBA16F : GL_RGBA, width, height, 0, GL_RGBA, floating ? GL_FLOAT : GL_UNSIGNED_BYTE, NULL);
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

	void Loader::CreateDepthBufferAttachment(GLuint& id, unsigned int width, unsigned int height, unsigned int multisampled, bool floating)
	{
		glGenRenderbuffers(1, &id);
		glBindRenderbuffer(GL_RENDERBUFFER, id);
		if (!multisampled)
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		}
		else 
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisampled, GL_DEPTH24_STENCIL8, width, height);
		}
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);
	}

	void Loader::CreateColorBufferAttachment(GLuint& id, unsigned int width, unsigned int height, unsigned int multisampled, bool floating)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, multisampled, floating ? GL_RGBA16F : GL_RGBA, width, height, GL_TRUE);
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
		auto [width, height] = Application::Get().GetWindow().GetSize();
		glViewport(0, 0, width, height);
	}
}