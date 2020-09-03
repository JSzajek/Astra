#pragma once

#include <vector>
#include "../buffers/VertexArray.h"
#include "../buffers/Texture.h"

namespace Astra::Graphics
{
	class Loader
	{
	private:
		std::vector<GLuint> m_vaos;
		std::vector<GLuint> m_vbos;
		std::vector<Texture*> m_textures;
	public:
		Loader(const Loader&) = delete;
		void operator=(const Loader&) = delete;

		static Loader& Get()
		{
			static Loader instance;
			return instance;
		}
		
		static const VertexArray* Load(unsigned int drawType, const std::vector<float>& vertices,
			const std::vector<int>& indices, const std::vector<float>& texturesCoords,
			const std::vector<float>& normals)
		{
			return Get().LoadImpl(drawType, vertices, indices, texturesCoords, normals);
		}

		static const VertexArray* Load(unsigned int drawType, const std::vector<float>& vertices)
		{
			return Get().LoadImpl(drawType, vertices);
		}

		static Texture* LoadTexture(const char* const filepath)
		{
			return Get().LoadTextureImpl(filepath);
		}

	private:
		Loader();
		~Loader();

		const VertexArray* LoadImpl(unsigned int drawType, const std::vector<float>& vertices,
									const std::vector<int>& indices, const std::vector<float>& texturesCoords,
									const std::vector<float>& normals);

		const VertexArray* LoadImpl(unsigned int drawType, const std::vector<float>& vertices);
		
		Texture* LoadTextureImpl(const char* const filepath);

		GLuint BindInAttribBuffer(GLuint index, const std::vector<float>& data, int strideSize, GLenum usage, GLboolean normalized = GL_FALSE);

		GLuint BindIndicesBuffer(const std::vector<int>& data, GLenum usage);

		inline void UnbindVertexArray() { glBindVertexArray(0); }

		const GLuint& GenerateVaoId();

		const GLuint& GenerateVboId();
	};
}