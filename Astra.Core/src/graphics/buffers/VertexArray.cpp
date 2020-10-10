#include "VertexArray.h"

namespace Astra::Graphics
{
	VertexArray::VertexArray(const GLuint& vaoId, const unsigned int& vertexCount, const unsigned int& drawType)
		: vaoId(vaoId), vertexCount(vertexCount), drawType(drawType)
	{
		vboIds = new GLuint[TRACKED_VBOS];
		memset(vboIds, 0, TRACKED_VBOS * sizeof(GLuint));
	}

	VertexArray::~VertexArray()
	{
		delete[] vboIds;
	}

	void VertexArray::AddBuffer(const BufferType& type, const GLuint& id)
	{
		vboIds[static_cast<unsigned short>(type)] = id;
	}

	GLuint& const VertexArray::operator()(const BufferType& type)
	{
		return vboIds[static_cast<unsigned short>(type)];
	}
}