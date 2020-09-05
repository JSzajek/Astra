#include "Loader.h"

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
		for (const Texture* texture : m_textures)
		{
			delete texture;
		}
		m_textures.clear();
	}

	void Loader::RemoveTextureImpl(Texture* texture)
	{
		auto found = std::find(m_textures.begin(), m_textures.end(), texture);
		if (found != m_textures.end())
		{
			m_textures.erase(found);
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

	const VertexArray* Loader::LoadImpl(unsigned int drawType, const std::vector<float>& vertices)
	{
		GLuint id = GenerateVaoId();
		GLuint vboId = BindInAttribBuffer(0, vertices, 2, GL_STATIC_DRAW);
		UnbindVertexArray();

		VertexArray* vertArray = new VertexArray(id, vertices.size() / 2, drawType);
		(*vertArray)(BufferType::Vertices) = vboId;
		return vertArray;
	}

	Texture* Loader::LoadTextureImpl(const char* const filepath, GLint clippingOption)
	{
		Texture* texture = new Texture(filepath);
		return BufferTextureImpl(texture, clippingOption);
	}

	Texture* Loader::BufferTextureImpl(Texture* initializedTexture, GLint clippingOption)
	{
		glGenTextures(1, &initializedTexture->id);
		glBindTexture(GL_TEXTURE_2D, initializedTexture->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clippingOption);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clippingOption);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, initializedTexture->width, initializedTexture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, initializedTexture->buffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_textures.emplace_back(initializedTexture);

		initializedTexture->ClearBuffer();
		return initializedTexture;
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
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
}