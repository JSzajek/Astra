#pragma once

#include <map>
#include <queue>
#include <vector>

#include "SimplifierStructs.h"

namespace Astra::Graphics
{
	#define MAX_EDGE_COUNT		20000000

	struct EdgeComparator
	{
		bool operator() (Edge left, Edge right)
		{
			return left.deltaV > right.deltaV;
		}
	};

	class EdgeHeap
	{
	public:
		std::priority_queue<Edge, std::vector<Edge>, EdgeComparator> queue;
		std::map<std::pair<int, int>, int> mapEdgeToId;
		bool isDeleted[MAX_EDGE_COUNT + 1];
		int cntEdge;
	public:
		EdgeHeap();
		void Add(Edge& edge);
		void Delete(const Edge& edge);
		Edge MinDelta();
	};
}