#include "astra_pch.h"

#include "VertGroup.h"

namespace Astra::Graphics
{
	VertGroup::VertGroup()
		: cntVert(0)
	{
		isDeleted = new bool[MAX_POINT_COUNT];
		for (int i = 0; i < MAX_POINT_COUNT; ++i)
			isDeleted[i] = false;
	}

	VertGroup::~VertGroup()
	{
		delete[] isDeleted;
	}

	int VertGroup::Add(Vert& vertex)
	{
		++cntVert;
		vertex.id = cntVert;
		group[cntVert] = vertex;
		return cntVert;
	}

	void VertGroup::Delete(int index)
	{
		if (index >= MAX_POINT_COUNT)
			return;

		isDeleted[index] = true;
		for (auto iter = group[index].connectVerts.begin(); iter != group[index].connectVerts.end(); ++iter)
		{
			group[(*iter)].Delete(index);
		}
	}
	
	int VertGroup::GetCommonVertNum(int u, int v)
	{
		int count = 0;
		for (auto iter = group[u].connectVerts.begin(); iter != group[u].connectVerts.end(); ++iter)
		{
			if (group[v].IsExistConnectVert(*iter))
			{
				++count;
			}
		}
		return count;
	}
}