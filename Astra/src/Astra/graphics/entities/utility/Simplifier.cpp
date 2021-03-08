#include "astra_pch.h"

#include "Astra/Core.h"
#include "Simplifier.h"

namespace Astra::Graphics
{
	Simplifier::Simplifier()
		: vOffset(1), pOffset(0), fOffset(0), faceCount(0),
			vertexCount(0), rate(1.0f)
	{
	}

	Simplifier::~Simplifier()
	{
	}

	void Simplifier::SetupImpl(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		for (const auto& vert : vertices)
		{
			AddVertex(vert.Position);
		}

		unsigned int i = 0;
		while (i < indices.size())
		{
			AddFace(Face(indices[i], indices[i + 1], indices[i + 2]));
			i += 3;
		}
	}

	void Simplifier::SimplifyMeshImpl(float _rate, std::vector<unsigned int>* resultIndices)
	{
		rate = _rate;

		ComputeQ();
		ComputeValidPairs();
		heap.Build(pairs, pOffset);
	
		int nowCount = faceCount;
		int iter = 0;
		while ((double)nowCount > (double)faceCount * rate)
		{
			if (iter % 1000 == 0)
			{
				ASTRA_CORE_INFO("Iter: {0} Count: {1}", iter, nowCount);
			}

			int pairIndex = heap.Top();
			if (pairIndex < 0)
				break;

			Pair minPair = pairs[heap.Top()];
			heap.Delete();
			bool b = Update(minPair);
			if (b)
			{
				nowCount -= 2;
			}
			++iter;
		}
		
		for (int index = 0; index < vOffset; ++index) {
			if (valid[index]) {
				for (int i = 0; i < vertices[index].neighbor.size(); ++i) {
					int neiIndex1 = vertices[index].neighbor[i];
					for (int j = i + 1; j < vertices[index].neighbor.size(); ++j) {
						int neiIndex2 = vertices[index].neighbor[j];
						if (!inFace[neiIndex1] && !inFace[neiIndex2]) {
							if (vertices[neiIndex1].IsNeighbor(neiIndex2)) {
								auto found = faceMap.find(Face(index, neiIndex1, neiIndex2));
								if (found != faceMap.end())
								{
									resultIndices->push_back(found->indices[0]);
									resultIndices->push_back(found->indices[1]);
									resultIndices->push_back(found->indices[2]);
								}
							}
						}
					}
				}
				inFace[index] = true;
			}
		}
	}

	void Simplifier::ComputeQ()
	{
		for (unsigned int i = 0; i < vOffset; ++i)
		{
			vertices[i].ComputeQ(vertices);
		}
	}

	void Simplifier::ComputeValidPairs()
	{
		tree.BuildTree(vertices, vOffset);

		for (int i = 0; i < vOffset; ++i)
		{
			for (int j = 0; j < vertices[i].neighbor.size(); ++j)
			{
				int index = vertices[i].neighbor[j];
				if (!inPair[index])
				{
					int pairIndex = AddPair(i, index);
					pairs[pairIndex].UpdateOptimalPos(vertices);
					pairs[pairIndex].UpdateCost(vertices);
				}
			}

			std::vector<int> v_hit;
			tree.Query(tree.root, vertices[i].p, v_hit);
			for (int k = 0; k < v_hit.size(); ++k)
			{
				if ((v_hit[k] != i) && !inPair[v_hit[k]] && !vertices[i].HasPair(Pair(i, v_hit[k]), pairs))
				{
					int index = AddPair(i, v_hit[k]);
					pairs[index].UpdateOptimalPos(vertices);
					pairs[index].UpdateCost(vertices);
				}
			}
			inPair[i] = true;
		}
	}

	int Simplifier::AddVertex(const Math::Vec3& position)
	{
		int index = vOffset;
		vertices[vOffset].SetPosition(position);
		originVertices[vOffset] = position;
		valid[vOffset] = true;
		++vOffset;
		++vertexCount;
		ASTRA_ASSERT(vOffset < MAX_VERTEX, "Simplifier: Error in Adding Vertex.");
		return index;
	}
	
	int Simplifier::AddPair(int v1, int v2)
	{
		int index = pOffset;
		pairs[pOffset].v[0] = v1;
		pairs[pOffset].v[1] = v2;
		pairs[pOffset].index = index;
		vertices[v1].AddPair(index);
		vertices[v2].AddPair(index);
		++pOffset;
		ASTRA_ASSERT(pOffset < MAX_PAIR, "Simplifier: Error in Adding Pair.");
		return index;
	}

	void Simplifier::AddFace(const Face& face)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = i + 1; j < 3; ++j)
			{
				if (!vertices[face.indices[i]].IsNeighbor(face.indices[j]))
				{
					vertices[face.indices[i]].AddNeighbor(face.indices[j]);
					vertices[face.indices[j]].AddNeighbor(face.indices[i]);
				}
			}
		}

		faceMap.insert(face);
		originFace[fOffset] = face;
		++fOffset;
		++faceCount;
	}

	void Simplifier::DeleteVertex(int index)
	{
		ASTRA_CORE_ASSERT(valid[index] == true, "Simplifier: Delete Vertex Error.");
		valid[index] = false;
		--vertexCount;
	}

	bool Simplifier::Update(const Pair& pair)
	{
		auto newPos = pair.GetOptimalPos();
		for (int i = 0; i < vertices[pair.v[0]].neighbor.size(); ++i)
		{
			for (int j = i + 1; j < vertices[pair.v[0]].neighbor.size(); ++j)
			{
				int neiIndex1 = vertices[pair.v[0]].neighbor[i];
				int neiIndex2 = vertices[pair.v[0]].neighbor[j];
				auto found = faceMap.find(Face(pair.v[0], neiIndex1, neiIndex2));
				if (found != faceMap.end())
				{
					auto originNorm = (*found).Norm(vertices);
					auto p0 = vertices[found->indices[0]].p;
					auto p1 = vertices[found->indices[1]].p;
					auto p2 = vertices[found->indices[2]].p;

					if (found->indices[0] == pair.v[0])
					{
						p0 = newPos;
					}
					else if (found->indices[1] == pair.v[0])
					{
						p1 = newPos;
					}
					else
					{
						p2 = newPos;
					}
					
					auto newNorm = (p1 - p0).Cross(p2 - p0).Normalized();
					if (originNorm.Dot(newNorm) < -0.1f)
					{
						vertices[pair.v[0]].DeletePair(pair.index);
						vertices[pair.v[1]].DeletePair(pair.index);
						return false;
					}
				}
			}
		}

		int newIndex = pair.v[0];
		auto originPos = vertices[newIndex].p;
		vertices[newIndex].SetPosition(newPos);

		// Update Faces
		std::vector<Face> realFaceV;
		std::vector<Face> newFaceV;
		for (int i = 0; i < vertices[pair.v[1]].neighbor.size(); ++i) {
			for (int j = i + 1; j < vertices[pair.v[1]].neighbor.size(); ++j) {
				int neiIndex1 = vertices[pair.v[1]].neighbor[i];
				int neiIndex2 = vertices[pair.v[1]].neighbor[j];

				auto found = faceMap.find(Face(pair.v[1], neiIndex1, neiIndex2));
				if (found != faceMap.end())
				{
					Face newFace = *found;
					if (found->indices[0] == pair.v[1])
					{
						newFace.indices[0] = pair.v[0];
					}
					else if (found->indices[1] == pair.v[1])
					{
						newFace.indices[1] = pair.v[0];
					}
					else if (found->indices[2] == pair.v[1])
					{
						newFace.indices[2] = pair.v[0];
					}

					auto n0 = found->Norm(vertices);
					auto n = newFace.Norm(vertices);
					if (n.Dot(n0) > -0.1f) 
					{
						realFaceV.push_back(*found);
						newFaceV.push_back(newFace);
					}
					else 
					{
						vertices[pair.v[0]].SetPosition(originPos);
						vertices[pair.v[0]].DeletePair(pair.index);
						vertices[pair.v[1]].DeletePair(pair.index);
						return false;
					}
				}
			}
		}
		for (int i = 0; i < realFaceV.size(); ++i) 
		{
			faceMap.erase(realFaceV[i]);
			faceMap.insert(newFaceV[i]);
		}
		realFaceV.clear();
		newFaceV.clear();

		// Get New Neighbor
		for (int i = 0; i < vertices[pair.v[1]].neighbor.size(); ++i)
		{
			int index = vertices[pair.v[1]].neighbor[i];
			if (index != pair.v[0])
			{
				if (!vertices[newIndex].IsNeighbor(index))
				{
					vertices[newIndex].AddNeighbor(index);
					vertices[index].AddNeighbor(newIndex);
				}
				vertices[index].DeleteNeighbor(pair.v[1]);
			}
		}
		vertices[newIndex].DeleteNeighbor(pair.v[1]);
		
		// Update Pairs
		for (int i = 0; i < vertices[pair.v[1]].pairs.size(); ++i) {
			int pairIndex = vertices[pair.v[1]].pairs[i];

			if (pairs[pairIndex].v[0] == pair.v[1]) {
				if (pairs[pairIndex].v[1] == pair.v[0]) {
					//pair between v[0] and v[1]
					ASTRA_CORE_ASSERT(pairIndex == pair.index, "Simplifier: Update Error.");
					vertices[newIndex].DeletePair(pairIndex);
					continue;
				}
				else {
					pairs[pairIndex].v[0] = newIndex;
				}
			}
			else {
				ASTRA_CORE_ASSERT(pairs[pairIndex].v[1] == pair.v[1], "Simplifier: Update Error.");
				if (pairs[pairIndex].v[0] == pair.v[0]) 
				{
					//pair between v[0] and v[1]
					ASTRA_CORE_ASSERT(pairIndex == pair.index, "Simplifier: Update Error.");
					vertices[newIndex].DeletePair(pairIndex);
					continue;
				}
				else 
				{
					pairs[pairIndex].v[1] = newIndex;
				}
			}
			if (vertices[newIndex].HasPair(pairIndex, pairs))
			{
				heap.Remove(pairs[pairIndex]);
				if (pairs[pairIndex].v[0] == pair.v[0]) {
					vertices[pairs[pairIndex].v[1]].DeletePair(pairIndex);
				}
				else {
					vertices[pairs[pairIndex].v[0]].DeletePair(pairIndex);
				}
			}
			else {
				vertices[newIndex].AddPair(pairIndex);
			}
		}

		// Update Cost and Optimal Position
		vertices[newIndex].q += vertices[pair.v[1]].q;
		for (int i = 0; i < vertices[newIndex].pairs.size(); ++i)
		{
			int index = vertices[newIndex].pairs[i];
			pairs[index].UpdateOptimalPos(vertices);
			pairs[index].UpdateCost(vertices);
			heap.Update(pairs[index]);
		}
		DeleteVertex(pair.v[1]);
		return true;
	}

	/*void Simplifier::SetupImpl(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		m_vertices.clear();
		m_triangles.clear();
		m_refs.clear();

		m_vertices.resize(vertices.size());
		m_triangles.resize(indices.size() / 3);

		for (int i = 0; i < vertices.size(); ++i)
		{
			m_vertices[i].p = vertices[i].Position;
		}

		unsigned int x = 0;
		for (int i = 0; i < m_triangles.size(); i++)
		{
			m_triangles[i].v[0] = indices[x++];
			m_triangles[i].v[1] = indices[x++];
			m_triangles[i].v[2] = indices[x++];
		}
	}*/

	/*void Simplifier::SimplifyMeshImpl(int target, float aggressive)
	{
		int deleted_triangles = 0;
		std::vector<int> deleted0, deleted1;
		int triangle_count = m_triangles.size();

		for (int iter = 0; iter < 100; iter++)
		{
			printf("iteration %d - triangles %d\n", iter, triangle_count - deleted_triangles);

			if (triangle_count - deleted_triangles <= target)
				break;

			UpdateMesh(iter);

			for (int i = 0; i < m_triangles.size(); ++i)
			{
				m_triangles[i].dirty = 0;
			}

			double threshold = 0.000000001 * pow(double(iter + 3), aggressive);
			for (int i = 0; i < m_triangles.size(); ++i)
			{
				auto& t = m_triangles[i];
				if (t.err[3] > threshold)
					continue;
				if (t.deleted)
					continue;
				if (t.dirty)
					continue;

				for (int j = 0; j < 3; ++j)
				{
					if (t.err[j] < threshold)
					{
						int i0 = t.v[j];
						auto& v0 = m_vertices[i0];
						int i1 = t.v[(j + 1) % 3];
						auto& v1 = m_vertices[i1];

						if (v0.border != v1.border)
							continue;

						Math::Vec3 p(0);
						CalculateError(i0, i1, p);
						deleted0.resize(v0.tcount);
						deleted1.resize(v1.tcount);

						if (Flipped(p, i0, i1, v0, v1, deleted0))
							continue;
						if (Flipped(p, i1, i0, v1, v0, deleted1))
							continue;

						v0.p = p;
						v0.q = v1.q + v0.q;
						int tstart = m_refs.size();

						UpdateTriangles(i0, v0, deleted0, deleted_triangles);
						UpdateTriangles(i0, v1, deleted1, deleted_triangles);

						int tcount = m_refs.size() - tstart;
						if (tcount <= v0.tcount)
						{
							if (tcount)
								memcpy(&m_refs[v0.tstart], &m_refs[tstart], tcount * sizeof(Ref));
						}
						else
						{
							v0.tstart = tstart;
						}

						v0.tcount = tcount;
						break;
					}
				}

				if (triangle_count - deleted_triangles <= target)
					break;
			}
		}

		auto t = 2;
	}

	double Simplifier::VertexError(const SymMatrix& q, double x, double y, double z)
	{
		return  q[0] * x * x + 2 * q[1] * x * y + 2 * q[2] * x * z + 2 * q[3] * x + q[4] * y * y
			+ 2 * q[5] * y * z + 2 * q[6] * y + q[7] * z * z + 2 * q[8] * z + q[9];
	}

	double Simplifier::CalculateError(int id_v1, int id_v2, Math::Vec3& result)
	{
		auto q = m_vertices[id_v1].q + m_vertices[id_v2].q;
		auto border = m_vertices[id_v1].border & m_vertices[id_v2].border;
		double error = 0;
		auto determinant = q.Determinant(0, 1, 2, 1, 4, 5, 2, 5, 7);
		if (determinant != 0 && !border)
		{
			result.x = -1 / determinant * (q.Determinant(1, 2, 3, 4, 5, 6, 5, 7, 8));
			result.y =  1 / determinant * (q.Determinant(0, 2, 3, 1, 5, 6, 2, 7, 8));
			result.z = -1 / determinant * (q.Determinant(0, 1, 3, 1, 4, 6, 2, 5, 8));
			error = VertexError(q, result.x, result.y, result.z);
		}
		else
		{
			auto p1 = m_vertices[id_v1].p;
			auto p2 = m_vertices[id_v2].p;
			auto p3 = (p1 + p2) / 2;
			double error1 = VertexError(q, p1.x, p1.y, p1.z);
			double error2 = VertexError(q, p2.x, p2.y, p2.z);
			double error3 = VertexError(q, p3.x, p3.y, p3.z);
			error = MIN(error1, MIN(error2, error3));
			if (error1 == error)
				result = p1;
			if (error2 == error)
				result = p2;
			if (error3 == error)
				result = p3;
		}
		return error;
	}

	bool Simplifier::Flipped(const Math::Vec3& p, int i0, int i1, Vert& v0, Vert& v1, std::vector<int>& deleted)
	{
		for (int i = 0; i < v0.tcount; ++i)
		{
			auto& t = m_triangles[m_refs[v0.tstart + i].tid];
			if (t.deleted)
				continue;

			int s = m_refs[v0.tstart + i].tvertex;
			int id1 = t.v[(s + 1) % 3];
			int id2 = t.v[(s + 2) % 3];

			if (id1 == i1 || id2 == i1)
			{
				deleted[i] = true;
				continue;
			}

			auto d1 = m_vertices[id1].p - p;
			d1.Normalize();
			auto d2 = m_vertices[id2].p - p;
			d2.Normalize();

			if (std::fabs(d1.Dot(d2)) > 0.999f)
				return true;

			auto cross = d1.Cross(d2);
			cross.Normalize();
			deleted[i] = false;
			if (cross.Dot(t.n) < 0.2f)
				return true;
		}
		return false;
	}

	void Simplifier::UpdateUVs(int i0, const Vert& v, const Math::Vec3& p, std::vector<int>& deleted)
	{
		for (int i = 0; i < v.tcount; ++i)
		{
			auto& r = m_refs[v.tstart + i];
			auto& t = m_triangles[r.tid];
			if (t.deleted)
				continue;
			if (deleted[i])
				continue;

			auto p1 = m_vertices[t.v[0]].p;
			auto p2 = m_vertices[t.v[1]].p;
			auto p3 = m_vertices[t.v[2]].p;
			t.uvs[r.tvertex] = Interpolate(p, p1, p2, p3, t.uvs);
		}
	}

	void Simplifier::UpdateTriangles(int i0, const Vert& v, std::vector<int>& deleted, int& deleted_triangles)
	{
		Math::Vec3 p(0);
		for (int i = 0; i < v.tcount; ++i)
		{
			auto& r = m_refs[v.tstart + i];
			auto& t = m_triangles[r.tid];
			if (t.deleted)
				continue;
			if (deleted[i])
			{
				t.deleted = true;
				++deleted_triangles;
				continue;
			}
			t.v[r.tvertex] = i0;
			t.dirty = true;
			t.err[0] = CalculateError(t.v[0], t.v[1], p);
			t.err[1] = CalculateError(t.v[1], t.v[2], p);
			t.err[2] = CalculateError(t.v[2], t.v[0], p);
			t.err[3] = MIN(t.err[0], MIN(t.err[1], t.err[2]));
			m_refs.push_back(r);
		}
	}

	void Simplifier::UpdateMesh(int iter)
	{
		if (iter > 0)
		{
			int dst = 0;
			for (int i = 0; i < m_triangles.size(); ++i)
			{
				if (!m_triangles[i].deleted)
				{
					m_triangles[dst++] = m_triangles[i];
				}
			}
			m_triangles.resize(dst);
		}

		if (iter == 0)
		{
			for (int i = 0; i < m_vertices.size(); ++i)
			{
				m_vertices[i].q = SymMatrix(0.0f);
			}

			for (int i = 0; i < m_triangles.size(); ++i)
			{
				auto& t = m_triangles[i];
				Math::Vec3 verts[3];
				for (int j = 0; j < 3; ++j)
				{
					verts[j] = m_vertices[t.v[j]].p;
				}
				
				auto cross = (verts[1] - verts[0]).Cross(verts[2] - verts[0]);
				cross.Normalize();
				t.n = cross;
				for (int j = 0; j < 3; ++j)
				{
					//if (t.v[j] == 0)
					//{
					//	auto tris = 2;
					//}
					m_vertices[t.v[j]].q = m_vertices[t.v[j]].q + SymMatrix(cross.x, cross.y, cross.z, -cross.Dot(verts[0]));
				}
			}

			for (int i = 0; i < m_triangles.size(); ++i)
			{
				// Calculate Edge Error
				auto& t = m_triangles[i];
				Math::Vec3 p(0);

				for (int j = 0; j < 3; ++j)
				{
					t.err[j] = CalculateError(t.v[j], t.v[(j + 1) % 3], p);
				}
				t.err[3] = MIN(t.err[0], MIN(t.err[1], t.err[2]));
			}
		}

		// Init Reference ID List
		for (int i = 0; i < m_vertices.size(); ++i)
		{
			m_vertices[i].tstart = 0;
			m_vertices[i].tcount = 0;
		}

		for (int i = 0; i < m_triangles.size(); ++i)
		{
			auto& t = m_triangles[i];
			for (int j = 0; j < 3; ++j)
			{
				m_vertices[t.v[j]].tcount++;
			}
		}

		int tstart = 0;
		for (int i = 0; i < m_vertices.size(); ++i)
		{
			auto& v = m_vertices[i];
			v.tstart = tstart;
			tstart += v.tcount;
			v.tcount = 0;
		}

		// Write References
		m_refs.resize(m_triangles.size() * 3);
		for (int i = 0; i < m_triangles.size(); ++i)
		{
			auto& t = m_triangles[i];
			for (int j = 0; j < 3; ++j)
			{
				auto& v = m_vertices[t.v[j]];
				m_refs[v.tstart + v.tcount].tid = i;
				m_refs[v.tstart + v.tcount].tvertex = j;
				v.tcount++;
			}
		}

		// Identify Boundaries
		if (iter == 0)
		{
			std::vector<int> vcount, vids;

			for (int i = 0; i < m_vertices.size(); ++i)
			{
				m_vertices[i].border = 0;
			}

			for (int i = 0; i < m_vertices.size(); ++i)
			{
				auto& v = m_vertices[i];
				vcount.clear();
				vids.clear();

				for (int j = 0; j < v.tcount; ++j)
				{
					int k = m_refs[v.tstart + j].tid;
					auto& t = m_triangles[k];
					for (int k = 0; k < 3; ++k)
					{
						int ofs = 0;
						int id = t.v[k];

						while (ofs < vcount.size())
						{
							if (vids[ofs] == id)
							{
								break;
							}
							ofs++;
						}

						if (ofs == vcount.size())
						{
							vcount.push_back(1);
							vids.push_back(id);
						}
						else
						{
							vcount[ofs]++;
						}
					}
				}

				for (int j = 0; j < vcount.size(); ++j)
				{
					if (vcount[j] == 1)
					{
						m_vertices[vids[j]].border = 1;
					}
				}
			}
		}
	}

	Math::Vec3 Simplifier::Interpolate(const Math::Vec3& p, const Math::Vec3& a, const Math::Vec3& b, const Math::Vec3& c, const Math::Vec3 attrs[3])
	{
		// Calculate bary centric
		auto v0 = b - a;
		auto v1 = c - a;
		auto v2 = p - a;

		float d00 = v0.Dot(v0);
		float d01 = v0.Dot(v1);
		float d11 = v1.Dot(v1);
		float d20 = v2.Dot(v0);
		float d21 = v2.Dot(v1);

		float denom = d00 * d11 - d01 * d01;
		float v = (d11 * d20 - d01 * d21) / denom;
		float w = (d00 * d21 - d01 * d20) / denom;
		float u = 1.0f - v - w;
		
		Math::Vec3 result(0);
		result = result + attrs[0] * u;
		result = result + attrs[1] * v;
		result = result + attrs[2] * w;
		return result;
	}*/
}