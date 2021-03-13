#pragma once

#include "SimpStructs.h"

namespace Astra::Graphics
{
	struct HeapNode
	{
	public:
		int index;
		float cost, cost1;
		bool exist;
	public:
		HeapNode()
			: index(0), exist(0), cost(0), cost1(0)
		{
		}

		inline friend bool operator<(const HeapNode& n1, const HeapNode& n2)
		{
			if (std::fabs(n1.cost - n2.cost) < CMP_EPSILON)
			{
				return n1.cost1 < n2.cost1;
			}
			return n1.cost < n2.cost;
		}
	};

	class Heap
	{
	private:
		Pair* pairs;
		int n, pairNum;
	public:
		HeapNode data[MAX_HEAP_NODE];
	public:
		Heap()
			: pairs(NULL), n(0), pairNum(0)
		{
		}

		void Build(Pair* _pairs, int _n)
		{
			pairs = _pairs;
			n = _n;
			pairNum = _n;

			for (int i = 0; i < _n; ++i)
			{
				data[i].cost = _pairs[i].cost;
				data[i].cost1 = _pairs[i].cost1;
				data[i].index = i;
				data[i].exist = true;
			}
			for (int i = 0; i < _n; ++i)
			{
				pairs[data[i].index].heapIndex = i;
			}
			for (int i = (n - 2) >> 1; i >= 0; --i)
			{
				Down(i);
			}
		}

		int Add(Pair& pair)
		{
			ASTRA_CORE_ASSERT(n > 0 && n < MAX_HEAP_NODE, "Heap: Error in Adding Pair");

			data[n].cost = pair.cost;
			data[n].cost1 = pair.cost1;
			data[n].index = pair.index;
			data[n].exist = true;
			pair.heapIndex = n;
			++n;
			return Up(n - 1);
		}

		void Delete()
		{
			ASTRA_CORE_ASSERT(n - 1 < MAX_HEAP_NODE, "Heap: Error in Deleting");

			if (data[0].exist)
			{
				--pairNum;
			}
			data[0] = data[--n];
			if (data[0].exist)
			{
				ASTRA_CORE_ASSERT(pairs[data[0].index].heapIndex == n, "Heap: Error in Deleting.");
				pairs[data[0].index].heapIndex = 0;
			}
			Down(0);
		}

		void Remove(const Pair& pair)
		{
			ASTRA_CORE_ASSERT(data[pair.heapIndex].exist, "Heap: Can't Remove Non-existent Pair");
			
			data[pair.heapIndex].exist = false;
			--pairNum;
		}

		void Update(Pair& pair)
		{
			data[pair.heapIndex].exist = false;
			Add(pair);
		}

		int Top()
		{
			while (0 < n && !data[0].exist)
			{
				Delete();
			}
			if (0 < n)
			{
				return data[0].index;
			}
			return -1;
		}
	private:
		int Down(int k)
		{
			int i = k;
			int j = k;
			while (true)
			{
				if (((i << 1) + 1 < n) && (data[(i << 1) + 1] < data[j])) {
					j = (i << 1) + 1;
				}
				if (((i << 1) + 2 < n) && (data[(i << 1) + 2] < data[j])) {
					j = (i << 1) + 2;
				}
				if (i == j) {
					return i;
				}
				else
				{
					HeapNode temp = data[i]; data[i] = data[j]; data[j] = temp;
					if (data[i].exist) {
						ASTRA_CORE_ASSERT(pairs[data[i].index].heapIndex == j, "Heap: Down Error.");
						pairs[data[i].index].heapIndex = i;
					}
					if (data[j].exist) {
						/*if (pairs[data[j].index].heapIndex != i) {
							std::cout << i << " " << pairs[data[j].index].heapIndex << std::endl;
							std::cout << data[j].exist << std::endl;
						}*/
						ASTRA_CORE_ASSERT(pairs[data[j].index].heapIndex == i, "Heap: Down Error.");
						pairs[data[j].index].heapIndex = j;
					}
					i = j;
				}
			}
			return i;
		}

		int Up(int k)
		{
			int i = k;
			while (i > 0)
			{
				int j = (i - 1) >> 1;
				if (data[i] < data[j])
				{
					// Swap
					HeapNode temp = data[i];
					data[i] = data[j];
					data[j] = temp;

					if (data[i].exist)
					{
						ASTRA_CORE_ASSERT(pairs[data[i].index].heapIndex == j, "Heap: Up Error.");
						pairs[data[i].index].heapIndex = i;
					}
					if (data[j].exist)
					{
						ASTRA_CORE_ASSERT(pairs[data[j].index].heapIndex == i, "Heap: Up Error.");
						pairs[data[j].index].heapIndex = j;
					}
					i = j;
				}
				else
				{
					return i;
				}
			}
			return i;
		}
	};
}