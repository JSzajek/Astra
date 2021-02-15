#include "astra_pch.h"

#include "VertexArray.h"
#include <GL/glew.h>

namespace Astra::Graphics
{
	VertexArray::VertexArray(unsigned int vaoId, unsigned int vertexCount, unsigned int drawType)
		: vaoId(vaoId), vertexCount(vertexCount), drawType(drawType)
	{
		vboIds = new unsigned int[TRACKED_VBOS];
		memset(vboIds, 0, TRACKED_VBOS * sizeof(unsigned int));
	}

	VertexArray::VertexArray(const VertexArray& other)
		: vaoId(other.vaoId), vertexCount(other.vertexCount), drawType(other.drawType)
	{
		memcpy(vboIds, other.vboIds, TRACKED_VBOS * sizeof(unsigned int));
	}

	VertexArray::~VertexArray()
	{
		for (size_t i = 0; i < TRACKED_VBOS; i++)
		{
			glDeleteBuffers(1, &vboIds[i]);
		}
		glDeleteVertexArrays(1, &vaoId);
		delete[] vboIds;
	}

	void VertexArray::AddBuffer(const BufferType& type, unsigned int id)
	{
		vboIds[static_cast<unsigned short>(type)] = id;
	}

	unsigned int& VertexArray::operator()(const BufferType& type)
	{
		return vboIds[static_cast<unsigned short>(type)];
	}
}