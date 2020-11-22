#pragma once

#include <cstring>

namespace Astra::Graphics
{
	#define TRACKED_VBOS 4
	
	enum class BufferType : unsigned short
	{
		Vertices		= 0,
		TextureCoords	= 1,
		Normals			= 2,
		Tangents		= 3,
	};

	struct VertexArray
	{
	public:
		unsigned int vertexCount, drawType;
		unsigned int vaoId;
		unsigned int* vboIds;
	public:
		VertexArray(unsigned int vaoId, unsigned int vertexCount, unsigned int drawType);
		VertexArray(const VertexArray& other);
		~VertexArray();
		void AddBuffer(const BufferType& type, unsigned int id);

		unsigned int& operator()(const BufferType& type);
	};
}