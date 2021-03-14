#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <queue>

#include "Astra/math/Vec3.h"
#include "Astra/math/Vec4.h"
#include "Astra/graphics/buffers/Vertex.h"

#include "SimpStructs.h"
#include "KdTree.h"
#include "Heap.h"
#include "Map.h"

namespace Astra::Graphics
{
	/*template<typename T>
	class custom_priority_queue : public std::priority_queue<T, std::vector<T>>
	{
	public:

		bool remove(const T& value) {
			auto it = std::find(this->c.begin(), this->c.end(), value);
			if (it != this->c.end()) {
				this->c.erase(it);
				std::make_heap(this->c.begin(), this->c.end(), this->comp);
				return true;
			}
			else {
				return false;
			}
		}
	};*/

	struct FaceHash
	{
		std::size_t operator()(const Face& face) const
		{
			unsigned long long int a1 = face.indices[0] + face.indices[1] + face.indices[2];
			unsigned long long int a2 = face.indices[0] * face.indices[1] + face.indices[1] * face.indices[2] + face.indices[2] * face.indices[0];
			unsigned long long int a3 = face.indices[0] * face.indices[1] * face.indices[2];
			unsigned long long int temp = (a1 * 131313 + a2) * 131313 + a3;
			unsigned int result = temp % (int)(MAX_FACE);
			return result; 
		}
	};

	#define VERT(index) std::get<0>(vertices[vertMapper[index]])

	class Simplifier
	{
	public:
		Simplifier(const Simplifier&) = delete;
		void operator=(const Simplifier&) = delete;

		static Simplifier& Get()
		{
			static Simplifier instance;
			return instance;
		}

		static void Setup(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
		{
			Get().SetupImpl(vertices, indices);
		}

		static void SimplifyMesh(float rate, std::vector<Vertex>* result, std::vector<unsigned int>* indices)
		{
			Get().SimplifyMeshImpl(rate, result, indices);
		}
	private:
		Simplifier();
		~Simplifier();

		void SetupImpl(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

		void SimplifyMeshImpl(float rate, std::vector<Vertex>* result, std::vector<unsigned int>* resultIndices);
	private:
		std::unordered_map<unsigned int, size_t> vertMapper;
		std::unordered_map<unsigned int, unsigned int> indexMapper;
		std::unordered_map<size_t, std::tuple<Vert, std::vector<std::tuple<Vertex, unsigned int>>, unsigned int>> vertices;

		Face originFace[MAX_FACE];
		Pair pairs[MAX_PAIR];

		std::unordered_set<Face, FaceHash> faceMap;
		//Map faceMap;
		//custom_priority_queue<Pair> heap;
		Heap heap;
		KdTree tree;

		int fOffset, vOffset, pOffset;
		int faceCount, vertexCount;
		float rate;

		bool valid[MAX_VERTEX] = {};
		bool inPair[MAX_VERTEX] = {};
		bool inFace[MAX_VERTEX] = {};
	private:

		void ComputeQ();
		void ComputeValidPairs();
		int AddVertex(const Vertex& vert);
		int AddPair(int v1, int v2);
		void AddFace(const Face& face);

		void DeleteVertex(int index);
		bool Update(const Pair& pair);
	};
}