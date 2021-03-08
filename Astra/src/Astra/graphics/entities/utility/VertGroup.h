#pragma once

#include "SimplifierStructs.h"

namespace Astra::Graphics
{
	#define MAX_POINT_COUNT		1000000

	class VertGroup
	{
	public:
		Vert group[MAX_POINT_COUNT];
		int cntVert;
		bool* isDeleted;
	public:
		VertGroup();
		~VertGroup();

		int Add(Vert& vertex);
		void Delete(int index);
		int GetCommonVertNum(int left, int right);
	};
}