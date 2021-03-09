#pragma once

#include <vector>

#include "Astra/math/Vec3.h"
#include "Astra/math/Mat4.h"

namespace Astra::Graphics
{
	#define MAX_VERTEX		50000
	#define MAX_PAIR		4 * MAX_VERTEX
	#define MAX_FACE		3 * MAX_VERTEX
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

		inline friend bool operator<(const Pair& p1, const Pair& p2) { return p1.cost < p2.cost; }

		inline Math::Vec3 GetOptimalPos() const { return optPos; }

		void UpdateOptimalPos(const Vert* vertices);
		void UpdateCost(const Vert* vertices);
	};

	struct Face
	{
	public:
		int indices[3] = {};
	public:
		Face();
		Face(int v0, int v1, int v2);
		Face(const Face& face);
		Face(const int indices[]);

		Face& operator=(const Face& face);

		Math::Vec3 Norm(const Vert* vertices) const;
		
		friend bool operator==(const Face& face1, const Face& face2);

		bool InFace(const Math::Vec3& p, const Vert* vertices) const;
	};

	struct Vert
	{
	public:
		Math::Vec3 p;
		Math::Vec2 TextureCoords;
		Math::Vec3 Normal;
		Math::Mat4 q;
		int newIndex;

		std::vector<int> neighbor;
		std::vector<int> pairs;
	public:
		Vert(float x = 0.0f, float y = 0.0f, float z = 0.0f);

		inline void SetPosition(const Math::Vec3& position) { p = position; }
		inline void SetTextureCoord(const Math::Vec2& coord) { TextureCoords = coord; }
		inline void SetNormal(const Math::Vec3& normal) { Normal = normal; }
		
		inline void AddNeighbor(int index) { neighbor.push_back(index); }
		inline void AddPair(int index) { pairs.push_back(index); }
		
		bool IsNeighbor(int index) const;
		void DeleteNeighbor(int index);
		void ComputeQ(const Vert* vertices);
		bool HasPair(int index, const Pair* pairs) const;
		bool HasPair(const Pair& pair, const Pair* pairs) const;
		void DeletePair(int index);
	};

	class VertComp
	{
	public:
		int D;
		Vert* vB;
		VertComp(int d, Vert* vB)
			: D(d), vB(vB)
		{
		}

		bool operator()(int vp1, int vp2) const
		{
			if (D == 0) 
			{
				return vB[vp1].p.x < vB[vp2].p.x;
			}
			else if (D == 1)
			{
				return vB[vp1].p.y < vB[vp2].p.y;
			}
			else
			{
				return vB[vp1].p.z < vB[vp2].p.z;
			}
		}
	};
}
