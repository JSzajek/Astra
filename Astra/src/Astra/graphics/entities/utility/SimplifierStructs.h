#pragma once

#include <set>

#include "Astra/math/Vec3.h"

namespace Astra::Graphics
{
	struct Mat5
	{
	public:
		float data[5][5];
	public:
		Mat5()
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					data[i][j] = 0;
				}
			}
		}

		friend Mat5 operator+(const Mat5& left, const Mat5& right)
		{
			Mat5 result;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					result.data[i][j] = left.data[i][j] + left.data[i][j];
				}
			}
			return result;
		}
	};

	struct Edge
	{
	public:
		int id;
		int v1, v2;
		Math::Vec3 v;
		double deltaV;
	public:
		Edge(int u = -99, int v = -99)
			: id(-999), v1(u), v2(v), deltaV(123456789 * 1.0)
		{
		}
	};

	struct Vert
	{
	public:
		int id;
		Math::Vec3 pos;
		std::set<int> connectVerts;
	public:
		Vert()
			: id(-99)
		{
		}

		Vert(const Math::Vec3& position)
			: id (-98), pos(position)
		{
		}

		Vert(float x, float y, float z)
			: id(-97), pos(x,y,z)
		{
		}

		inline void Add(int vert) { connectVerts.insert(vert); }
		inline void Delete(int vert) { connectVerts.erase(vert); }
		inline bool IsExistConnectVert(int vert) { return connectVerts.count(vert) > 0; }
	};
}