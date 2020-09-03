#pragma once

#include <GL/glew.h>
#include <cstring>

namespace Astra::Graphics
{
	#define TRACKED_VBOS 4
	
	enum BufferType
	{
		Vertices		= 0,
		Indices			= 1,
		TextureCoords	= 2,
		Normals			= 3
	};

	struct VertexArray
	{
	public:
		unsigned int vertexCount, drawType;
		GLuint vaoId;
		GLuint* vboIds;
	public:
		VertexArray(const GLuint& vaoId, const unsigned int& vertexCount, const unsigned int& drawType);
		~VertexArray();
		void AddBuffer(const BufferType& type, const GLuint& id);

		GLuint& const operator()(const BufferType& type);
	};
}