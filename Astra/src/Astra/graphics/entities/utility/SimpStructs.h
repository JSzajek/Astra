#pragma once

#include <vector>

#include "Astra/graphics/buffers/Vertex.h"
#include "Astra/math/Vec3.h"
#include "Astra/math/Mat4.h"

namespace Astra::Graphics
{
	#define MAX_VERTEX		500000
	#define MAX_PAIR		4 * MAX_VERTEX
	#define MAX_FACE		4 * MAX_VERTEX
	#define MAX_HEAP_NODE	500000

	struct Vert;

	class Pair
	{
	private:
		Math::Vec3 optPos;
	public:
		float cost;
		float cost1;
		int heapIndex;
		int index;
		int v[2] = {};
	public:
		Pair();
		Pair(int v0, int v1);

		inline friend bool operator<(const Pair& p1, const Pair& p2) 
		{ 
			if (std::fabs(p1.cost - p2.cost) < CMP_EPSILON)
			{
				return p1.cost1 < p2.cost1;
			}
			return p1.cost < p2.cost;
		}

		inline friend bool operator==(const Pair& p1, const Pair& p2)
		{
			return p1.cost == p2.cost || p1.cost1 == p2.cost1;
		}

		inline Math::Vec3 GetOptimalPos() const { return optPos; }

		void Update(std::unordered_map<size_t, std::tuple<Vert, std::vector<std::tuple<Vertex, unsigned int>>, unsigned int>>& vertices,
					std::unordered_map<unsigned int, size_t>& mapper);
	};

	struct Face
	{
	public:
		int indices[3] = {};
		int originIndices[3] = {};
	public:
		Face();
		Face(unsigned int v0, unsigned int v1, unsigned int v2);
		Face(unsigned int v0, unsigned int v1, unsigned int v2,
			 unsigned int origin_v0, unsigned int origin_v1, unsigned int origin_v2);
		Face(const Face& face);

		Face& operator=(const Face& face);

		Math::Vec3 Norm(std::unordered_map<size_t, std::tuple<Vert, std::vector<std::tuple<Vertex, unsigned int>>, unsigned int>>& vertices,
					    std::unordered_map<unsigned int, size_t>& mapper) const;
		
		friend bool operator==(const Face& face1, const Face& face2);
	};

	struct Vert
	{
	public:
		Math::Vec3 p;
		Math::Mat4 q;
		int newIndex;

		std::vector<int> neighbor;
		std::vector<int> pairs;
	public:
		Vert(float x = 0.0f, float y = 0.0f, float z = 0.0f);

		inline void SetPosition(const Math::Vec3& position) { p = position; }
		
		inline void AddNeighbor(int index) { neighbor.push_back(index); }
		inline void AddPair(int index) { pairs.push_back(index); }

		bool IsNeighbor(int index) const;
		void DeleteNeighbor(int index);
		void ComputeQ(std::unordered_map<size_t, std::tuple<Vert, std::vector<std::tuple<Vertex, unsigned int>>, unsigned int>>& vertices,
					  std::unordered_map<unsigned int, size_t>& mapper);
		bool HasPair(int index, const Pair* pairs) const;
		bool HasPair(const Pair& pair, const Pair* pairs) const;
		void DeletePair(int index);
	};

	class VertComp
	{
	public:
		int D;
		std::unordered_map<size_t, std::tuple<Vert, std::vector<std::tuple<Vertex, unsigned int>>, unsigned int>>* vertices;
		std::unordered_map<unsigned int, size_t>* mapper;
		VertComp(int d, std::unordered_map<size_t, std::tuple<Vert, std::vector<std::tuple<Vertex, unsigned int>>, unsigned int>>* vertices,
						std::unordered_map<unsigned int, size_t>* mapper)
			: D(d), vertices(vertices), mapper(mapper)
		{
		}

		bool operator()(int vp1, int vp2) const
		{
			const auto& v1 = std::get<0>((*vertices)[(*mapper)[vp1]]);
			const auto& v2 = std::get<0>((*vertices)[(*mapper)[vp2]]);
			if (D == 0) 
			{
				return v1.p.x < v2.p.x;
			}
			else if (D == 1)
			{
				return v1.p.y < v2.p.y;
			}
			else
			{
				return v1.p.z < v2.p.z;
			}
		}
	};
}
