#include "astra_pch.h"

#include "SimpStructs.h"

namespace Astra::Graphics
{
	Pair::Pair()
		: cost(0.0f), cost1(0.0f), optPos(Math::Vec3::Zero), 
			heapIndex(0), index(0)
	{
		v[0] = v[1] = 0;
	}
	
	Pair::Pair(int v0, int v1)
		: cost(0.0f), cost1(0.0f), optPos(Math::Vec3::Zero),
			heapIndex(0), index(0)
	{
		v[0] = v0; 
		v[1] = v1;
	}

	void Pair::Update(std::unordered_map<size_t, std::tuple<Vert, std::vector<std::tuple<Vertex, unsigned int>>, unsigned int>>& vertices,
					  std::unordered_map<unsigned int, size_t>& mapper)
	{
		const auto& first = std::get<0>(vertices[mapper[v[0]]]);
		const auto& second = std::get<0>(vertices[mapper[v[1]]]);

		optPos = (first.p + second.p) / 2;
		Math::Mat4 a = first.q + second.q;
		a.columns[3].x = 0;
		a.columns[3].y = 0;
		a.columns[3].z = 0;
		a.columns[3].w = 1;

		bool invertible = false;
		auto inv = a.Inverse(&invertible);

		if (invertible)
		{
			// a * [0, 0, 0, 1].T
			optPos.x = inv.columns[0].w;
			optPos.y = inv.columns[1].w;
			optPos.z = inv.columns[2].w;
		}

		Math::Vec4 y(optPos, 1.0f);
		Math::Vec4 a_y = a * y;
		cost = y.Dot(a_y);
		cost1 = first.p.DistanceSquaredTo(second.p);
	}

	Face::Face()
	{
		indices[0] = indices[1] = indices[2] = 0;
		originIndices[0] = originIndices[1] = originIndices[2] = 0;
	}
	
	Face::Face(unsigned int v0, unsigned int v1, unsigned int v2)
	{
		indices[0] = v0;
		indices[1] = v1;
		indices[2] = v2;
		originIndices[0] = originIndices[1] = originIndices[2] = 0;
	}

	Face::Face(unsigned int v0, unsigned int v1, unsigned int v2,
			   unsigned int origin_v0, unsigned int origin_v1, unsigned int origin_v2)
	{
		indices[0] = v0;
		indices[1] = v1;
		indices[2] = v2;
		originIndices[0] = origin_v0;
		originIndices[1] = origin_v1;
		originIndices[2] = origin_v2;
	}
	
	Face::Face(const Face& face)
	{
		indices[0] = face.indices[0];
		indices[1] = face.indices[1];
		indices[2] = face.indices[2];

		originIndices[0] = face.originIndices[0];
		originIndices[1] = face.originIndices[1];
		originIndices[2] = face.originIndices[2];
	}
	
	Face& Face::operator=(const Face& face)
	{
		if (this == &face)
			return *this;

		indices[0] = face.indices[0];
		indices[1] = face.indices[1];
		indices[2] = face.indices[2];
		return *this;
	}

	Math::Vec3 Face::Norm(std::unordered_map<size_t, std::tuple<Vert, std::vector<std::tuple<Vertex, unsigned int>>, unsigned int>>& vertices,
						  std::unordered_map<unsigned int, size_t>& mapper) const
	{
		auto& v0 = std::get<0>(vertices[mapper[indices[0]]]).p;
		return (std::get<0>(vertices[mapper[indices[1]]]).p - v0).Cross(std::get<0>(vertices[mapper[indices[2]]]).p - v0).Normalized();
	}

	bool operator==(const Face& face1, const Face& face2)
	{
		bool b0 = (face1.indices[0] == face2.indices[0]) && (face1.indices[1] == face2.indices[1]) && (face1.indices[2] == face2.indices[2]);
		bool b1 = (face1.indices[0] == face2.indices[0]) && (face1.indices[1] == face2.indices[2]) && (face1.indices[2] == face2.indices[1]);
		bool b2 = (face1.indices[0] == face2.indices[1]) && (face1.indices[1] == face2.indices[0]) && (face1.indices[2] == face2.indices[2]);
		bool b3 = (face1.indices[0] == face2.indices[1]) && (face1.indices[1] == face2.indices[2]) && (face1.indices[2] == face2.indices[0]);
		bool b4 = (face1.indices[0] == face2.indices[2]) && (face1.indices[1] == face2.indices[0]) && (face1.indices[2] == face2.indices[1]);
		bool b5 = (face1.indices[0] == face2.indices[2]) && (face1.indices[1] == face2.indices[1]) && (face1.indices[2] == face2.indices[0]);
		return b0 || b1 || b2 || b3 || b4 || b5;
	}

	Vert::Vert(float x, float y, float z)
		: p(x, y, z), newIndex(0)
	{
	}

	bool Vert::IsNeighbor(int index) const
	{
		for (int i = 0; i < neighbor.size(); ++i)
		{
			if (neighbor[i] == index)
				return true;
		}
		return false;
	}

	void Vert::DeleteNeighbor(int index)
	{
		for (int i = 0; i < neighbor.size(); ++i)
		{
			if (neighbor[i] == index)
			{
				neighbor[i] = neighbor.back();
				neighbor.pop_back();
				return;
			}
		}
		ASTRA_ASSERT(false, "Vert: Couldn't Delete Neighbor");
	}

	void Vert::ComputeQ(std::unordered_map<size_t, std::tuple<Vert, std::vector<std::tuple<Vertex, unsigned int>>, unsigned int>>& vertices,
						std::unordered_map<unsigned int, size_t>& mapper)
	{
		for (int i = 0; i < neighbor.size(); ++i)
		{
			const auto& vert = std::get<0>(vertices[mapper[neighbor[i]]]);
			for (int j = i + 1; j < neighbor.size(); ++j)
			{
				if (vert.IsNeighbor(neighbor[j]))
				{
					Math::Vec3 norm = (vert.p - p).Cross(std::get<0>(vertices[mapper[neighbor[j]]]).p - p).Normalized();
					float w = -(p.Dot(norm));
					Math::Vec4 v4(norm, w);
					q += Math::Mat4(v4);
				}
			}
		}
	}

	bool Vert::HasPair(int index, const Pair* _pairs) const
	{
		for (int i = 0; i < pairs.size(); ++i)
		{
			if (((_pairs[index].v[0] == _pairs[pairs[i]].v[0]) && (_pairs[index].v[1] == _pairs[pairs[i]].v[1]))
		     || ((_pairs[index].v[0] == _pairs[pairs[i]].v[1]) && (_pairs[index].v[1] == _pairs[pairs[i]].v[0])))
			{
				return true;
			}
		}
		return false;
	}

	bool Vert::HasPair(const Pair& pair, const Pair* _pairs) const
	{
		for (int i = 0; i < pairs.size(); ++i)
		{
			if (((pair.v[0] == _pairs[pairs[i]].v[0]) && (pair.v[1] == _pairs[pairs[i]].v[1]))
			 || ((pair.v[0] == _pairs[pairs[i]].v[1]) && (pair.v[1] == _pairs[pairs[i]].v[0])))
			{
				return true;
			}
		}
		return false;
	}

	void Vert::DeletePair(int index)
	{
		for (int i = 0; i < pairs.size(); ++i)
		{
			if (pairs[i] == index)
			{
				pairs[i] = pairs.back();
				pairs.pop_back();
				return;
			}
		}
		ASTRA_ASSERT(false, "Vert: Couldn't Delete Pair");
	}
}