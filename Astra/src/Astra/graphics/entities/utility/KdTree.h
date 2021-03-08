#pragma once

#include <vector>
#include <algorithm>

#include "Astra/math/Vec3.h"

#include "SimpStructs.h"

namespace Astra::Graphics
{
	struct KdNode
	{
	public:
		int index;
		int left, right;
		Math::Vec3 minBound;
		Math::Vec3 maxBound;
		int dim;
	public:
		KdNode()
			: index(0), left(0), right(0), dim(0),
				minBound(0), maxBound(0)
		{
		}
	};

	class KdTree
	{
	private:
		KdNode* nodes;
		Vert* vB;
		int offset;
		int vpNum;
	public:
		int root;
	public:
		KdTree()
			: offset(1), root(0), nodes(new KdNode[MAX_VERTEX]),
			vB(NULL), vpNum(-1)
		{
		}

		~KdTree()
		{
			delete[] nodes;
		}

		int BuildLayer(int* indices, int l, int r, int dim)
		{
			if (l >= r)
				return 0;
			int mid = (l + r) >> 1;
			std::nth_element(indices + l, indices + mid, indices + r, VertComp(dim, vB));
			int tIndex = offset;
			++offset;
			int midIndex = indices[mid];
			nodes[tIndex].index = midIndex;
			nodes[tIndex].dim = dim;
			nodes[tIndex].maxBound = vB[midIndex].p;
			nodes[tIndex].minBound = vB[midIndex].p;
			nodes[tIndex].left = BuildLayer(indices, l, mid, (dim + 1) % 3);
			if (nodes[tIndex].left)
			{
				nodes[tIndex].maxBound = MIN(nodes[nodes[tIndex].left].maxBound, nodes[tIndex].maxBound);
				nodes[tIndex].minBound = MIN(nodes[nodes[tIndex].left].minBound, nodes[tIndex].minBound);
			}
			
			nodes[tIndex].right = BuildLayer(indices, mid + 1, r, (dim + 1) % 3);
			if (nodes[tIndex].right)
			{
				nodes[tIndex].maxBound = MIN(nodes[nodes[tIndex].right].maxBound, nodes[tIndex].maxBound);
				nodes[tIndex].minBound = MIN(nodes[nodes[tIndex].right].minBound, nodes[tIndex].minBound);
			}
			return tIndex;
		}

		void BuildTree(Vert* _vB, int _vpNum)
		{
			vB = _vB;
			vpNum = _vpNum;

			int* indices = new int[MAX_VERTEX];
			for (int i = 0; i < vpNum; ++i)
			{
				indices[i] = i;
			}
			root = BuildLayer(indices, 0, vpNum, 0);
			delete[] indices;
		}

		void Query(int node, const Math::Vec3& position, std::vector<int>& vHit) const
		{
			if (position.x > nodes[node].maxBound.x || position.x < nodes[node].minBound.x ||
				position.y > nodes[node].maxBound.y || position.y < nodes[node].minBound.y ||
				position.z > nodes[node].maxBound.z || position.z < nodes[node].minBound.z)
			{
				return;
			}

			int vIndex = nodes[node].index;

			if (vB[vIndex].p.IsEqualApprox(position))
			{
				vHit.push_back(vIndex);
			}

			if (nodes[node].left)
			{
				Query(nodes[node].left, position, vHit);
			}
			if (nodes[node].right)
			{
				Query(nodes[node].right, position, vHit);
			}
		}

		void Clear()
		{
			offset = 1;
			root = 0;
		}
	};
}
