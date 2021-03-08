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

	void Pair::UpdateOptimalPos(const Vert* vertices)
	{
		optPos = (vertices[v[0]].p + vertices[v[1]].p) / 2;
		Math::Mat4 a = vertices[v[0]].q + vertices[v[1]].q;
		a.columns[3][0] = 0;
		a.columns[3][1] = 0;
		a.columns[3][2] = 0;
		a.columns[3][3] = 1;

		Math::Vec4(0, 0, 0, 1.0f);
		for (int i = 0; i < 4; ++i)
		{
			a.columns[i][3] = -a.columns[i][3];
		}

		for (int i = 0; i < 3; ++i)
		{
			int j = 0;
			for (; j < 3; ++j)
			{
				if (std::fabs(a.columns[i][j]) >= 1e-6)
					break;
			}
			if (j == 3)
				return;

			for (int k = 0; k < 3; ++k)
			{
				if (k != i)
				{
					float d = a.columns[k][j] / a.columns[i][j];
					for (int l = 0; l < 4; ++l)
					{
						a.columns[k][l] = a.columns[k][l] - a.columns[i][l] * d;
					}
				}
			}
		}

		for (int i = 0; i < 3; ++i)
		{
			int count = 0;
			for (int j = 0; j < 3; ++j)
			{
				if (std::fabs(a.columns[i][j]) < 1e-6)
					count++;
			}
			if (count == 3)
				return;
		}

		float index[3];
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (std::fabs(a.columns[i][j]) > 1e-6)
				{
					index[j] = a.columns[i][3] / a.columns[i][j];
				}
			}
		}

		optPos.x = index[0];
		optPos.y = index[1];
		optPos.z = index[2];
	}

	void Pair::UpdateCost(const Vert* vertices)
	{
		Math::Vec4 y(optPos, 1.0f);
		Math::Mat4 a = vertices[v[0]].q + vertices[v[1]].q;
		Math::Vec4 a_y = a * y;
		cost = y.Dot(a_y);
		cost1 = vertices[v[0]].p.DistanceSquaredTo(vertices[v[1]].p);
	}

	Face::Face()
	{
		indices[0] = indices[1] = indices[2] = 0;
	}
	
	Face::Face(int v0, int v1, int v2)
	{
		indices[0] = v0;
		indices[1] = v1;
		indices[2] = v2;
	}
	
	Face::Face(const Face& face)
	{
		indices[0] = face.indices[0];
		indices[1] = face.indices[1];
		indices[2] = face.indices[2];
	}
	
	Face::Face(const int _indices[])
	{
		indices[0] = _indices[0];
		indices[1] = _indices[1];
		indices[2] = _indices[2];
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

	Math::Vec3 Face::Norm(const Vert* vertices) const
	{
		Math::Vec3 v0 = vertices[indices[0]].p;
		return (vertices[indices[1]].p - v0).Cross(vertices[indices[2]].p - v0).Normalized();
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

	bool Face::InFace(const Math::Vec3& p, const Vert* vertices) const
	{
		Math::Vec3 v0 = vertices[indices[0]].p;
		Math::Vec3 v1 = vertices[indices[1]].p;
		Math::Vec3 v2 = vertices[indices[2]].p;
		
		float a0 = (v1 - v0).Cross(v2 - v0).Length();
		float s0 = (v1 -  p).Cross(v2 -  p).Length();
		float s1 = (v2 -  p).Cross(v0 -  p).Length();
		float s2 = (v0 -  p).Cross(v1 -  p).Length();
		if (s0 + s1 + s2 > a0)
			return false;
		return true;
	}

	/*float Face::Distance(const Math::Vec3& p, const Vert* vertices) const
	{
		Math::Vec3 norm = Norm(vertices);
		float dist = norm.Dot(vertices[indices[0]].p - p);
		Math::Vec3 p_f = p + dist * norm;
		
		if (InFace(p_f, vertices))
		{
			return std::fabs(dist);
		}
		else
		{
			float d = 10000;
			for (int i = 0; i < 3; ++i)
			{
				float newD = vertices[indices[i]].p.DistanceTo(p);
				if (newD < d)
				{
					d = newD;
				}
			}
			return d;
		}
	}*/

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
	}

	void Vert::ComputeQ(const Vert* vertices)
	{
		for (int i = 0; i < neighbor.size(); ++i)
		{
			for (int j = i + 1; j < neighbor.size(); ++j)
			{
				if (vertices[neighbor[i]].IsNeighbor(neighbor[j]))
				{
					Math::Vec3 norm = (vertices[neighbor[i]].p - p).Cross(vertices[neighbor[j]].p - p).Normalized();
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