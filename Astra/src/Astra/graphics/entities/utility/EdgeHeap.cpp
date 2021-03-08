#include "astra_pch.h"

#include "EdgeHeap.h"
#include "Astra/math/MathsDefines.h"

namespace Astra::Graphics
{
	EdgeHeap::EdgeHeap()
		: cntEdge(0)
	{
		for (int i = 0; i < MAX_EDGE_COUNT; ++i)
		{
			isDeleted[false];
		}
	}

	void EdgeHeap::Add(Edge& edge)
	{
		++cntEdge;
		edge.id = cntEdge;
		auto u = MIN(edge.v1, edge.v2);
		auto v = MAX(edge.v1, edge.v2);
		mapEdgeToId[std::make_pair(u, v)] = cntEdge;
		queue.push(edge);
	}

	void EdgeHeap::Delete(const Edge& edge)
	{
		auto u = MIN(edge.v1, edge.v2);
		auto v = MAX(edge.v1, edge.v2);

		int id = mapEdgeToId[std::make_pair(u, v)];
		isDeleted[id] = true;
	}

	Edge EdgeHeap::MinDelta()
	{
		if (queue.size() == 0)
		{
			return Edge(0, 0);
		}
		while (isDeleted[queue.top().id])
		{
			queue.pop();
		}
		Edge min = queue.top();
		queue.pop();
		return min;
	}
}