#include "astra_pch.h"

#include <stb_image/stb_image.h>
#include <GLFW\glfw3.h>

#include "Loader.h"
#include "Astra/Application.h"

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
		glEnableVertexAttribArray(index);
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