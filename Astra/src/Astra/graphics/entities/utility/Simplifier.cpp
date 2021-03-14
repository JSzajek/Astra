#include "astra_pch.h"

#include "Astra/Core.h"
#include "Simplifier.h"

namespace Astra::Graphics
{
	Simplifier::Simplifier()
		: vOffset(0), pOffset(0), fOffset(0), faceCount(0),
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
			AddVertex(vert);
		}

		faceMap.reserve(indices.size() / 3);
		
		unsigned int i = 0;
		while (i < indices.size())
		{
			AddFace(Face(indexMapper[indices[i]], indexMapper[indices[i + 1]], indexMapper[indices[i + 2]],
						 indices[i], indices[i + 1], indices[i + 2]));
			i += 3;
		}
	}

	void Simplifier::SimplifyMeshImpl(float _rate, std::vector<Vertex>* result, std::vector<unsigned int>* resultIndices)
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
			if (Update(minPair))
			{
				nowCount -= 2;
			}
			++iter;
		}
		
		int vNum = 0;
		auto mapper = std::unordered_map<unsigned int, unsigned int>();
		for (int index = 0; index < vOffset; ++index) 
		{
			if (valid[index]) 
			{
				// Currently averaging the original vertices into the new position
				// instead should find a way to remap it back to its original vertex causing re-expansion of face mapping
				auto& list = std::get<1>(vertices[vertMapper[index]]);
				auto& first = VERT(index);
				Math::Vec2 avgTexCoords;
				Math::Vec3 avgNormCoords;
				for (const auto& v : list)
				{
					const auto& vert = std::get<0>(v);
					avgTexCoords += vert.TextureCoords;
					avgNormCoords += vert.Normal;
				}
				avgTexCoords /= list.size();
				avgNormCoords /= list.size();
				first.newIndex = vNum;
				result->push_back(Vertex(first.p.x, first.p.y, first.p.z,
										 avgTexCoords.x, avgTexCoords.y,
										 avgNormCoords.x, avgNormCoords.y, avgNormCoords.z));
				++vNum;
			}
		}

		for (int index = 0; index < vOffset; ++index) {
			if (valid[index]) {
				const auto& indexVert = VERT(index);
				for (int i = 0; i < indexVert.neighbor.size(); ++i) 
				{
					int neiIndex1 = indexVert.neighbor[i];
					for (int j = i + 1; j < indexVert.neighbor.size(); ++j) 
					{
						int neiIndex2 = indexVert.neighbor[j];
						if (!inFace[neiIndex1] && !inFace[neiIndex2]) 
						{
							if (std::get<0>(vertices[vertMapper[neiIndex1]]).IsNeighbor(neiIndex2))
							{
								/*Face realFace;
								int b = faceMap.Get(Face(index, neiIndex1, neiIndex2), realFace);
								if (b) 
								{*/
								auto found = faceMap.find(Face(index, neiIndex1, neiIndex2));
								if (found != faceMap.end())
								{
									resultIndices->push_back(VERT(found->indices[0]).newIndex);
									resultIndices->push_back(VERT(found->indices[1]).newIndex);
									resultIndices->push_back(VERT(found->indices[2]).newIndex);
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
		for (auto& v : vertices)
		{
			std::get<0>(v.second).ComputeQ(vertices, vertMapper);
		}
	}

	void Simplifier::ComputeValidPairs()
	{
		tree.BuildTree(&vertices, &vertMapper, vOffset);

		for (int i = 0; i < vOffset; ++i)
		{
			const auto& vert = VERT(i);
			for (int j = 0; j < vert.neighbor.size(); ++j)
			{
				int index = vert.neighbor[j];
				if (!inPair[index])
				{
					int pairIndex = AddPair(i, index);
					pairs[pairIndex].Update(vertices, vertMapper);
				}
			}

			std::vector<int> v_hit;
			tree.Query(tree.root, vert.p, v_hit);
			for (int k = 0; k < v_hit.size(); ++k)
			{
				if ((v_hit[k] != i) && !inPair[v_hit[k]] && !vert.HasPair(Pair(i, v_hit[k]), pairs))
				{
					int index = AddPair(i, v_hit[k]);
					pairs[index].Update(vertices, vertMapper);
				}
			}
			inPair[i] = true;
		}
	}

	int Simplifier::AddVertex(const Vertex& vert)
	{
		static unsigned int count = 0;
		int index = vOffset;
		
		size_t hash = std::hash<std::string>{}(vert.Position.ToString());
		auto found = vertices.find(hash);
		if (found == vertices.end())
		{
			auto v = Vert();
			v.SetPosition(vert.Position);

			vertices[hash] = std::make_tuple(v, std::vector<std::tuple<Vertex, unsigned int>>(), index);

			valid[index] = true;
			++vOffset;
			++vertexCount;
			ASTRA_ASSERT(vOffset < MAX_VERTEX, "Simplifier: Error in Adding Vertex.");
		}

		std::get<1>(vertices[hash]).push_back(std::make_tuple(vert, count));
		vertMapper[index] = hash;
		indexMapper[count] = std::get<2>(vertices[hash]); // Map back original index
		
		count++;
		return index;
	}
	
	int Simplifier::AddPair(int v1, int v2)
	{
		int index = pOffset;

		pairs[pOffset].v[0] = v1;
		pairs[pOffset].v[1] = v2;
		pairs[pOffset].index = index;

		VERT(v1).AddPair(index);
		VERT(v2).AddPair(index);
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
				if (!VERT(face.indices[i]).IsNeighbor(face.indices[j]))
				{
					VERT(face.indices[i]).AddNeighbor(face.indices[j]);
					VERT(face.indices[j]).AddNeighbor(face.indices[i]);
				}
			}
		}

		faceMap.insert(face);
		//faceMap.Insert(face);
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
		auto& v0Vert = VERT(pair.v[0]);
		auto& v1Vert = VERT(pair.v[1]);
		for (int i = 0; i < v0Vert.neighbor.size(); ++i)
		{
			for (int j = i + 1; j < v0Vert.neighbor.size(); ++j)
			{
				int neiIndex1 = v0Vert.neighbor[i];
				int neiIndex2 = v0Vert.neighbor[j];
				
				/*Face realFace;
				int b = faceMap.Get(Face(pair.v[0], neiIndex1, neiIndex2), realFace);
				if (b) 
				{*/
				auto found = faceMap.find(Face(pair.v[0], neiIndex1, neiIndex2));
				if (found != faceMap.end())
				{
					auto originNorm = found->Norm(vertices, vertMapper);
					auto p0 = VERT(found->indices[0]).p;
					auto p1 = VERT(found->indices[1]).p;
					auto p2 = VERT(found->indices[2]).p;

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
						VERT(pair.v[0]).DeletePair(pair.index);
						VERT(pair.v[1]).DeletePair(pair.index);
						return false;
					}
				}
			}
		}

		int newIndex = pair.v[0];
		auto originPos = VERT(newIndex).p;
		VERT(newIndex).SetPosition(newPos);

		// Update Faces
		std::vector<Face> realFaceV;
		std::vector<Face> newFaceV;
		for (int i = 0; i < v1Vert.neighbor.size(); ++i) {
			for (int j = i + 1; j < v1Vert.neighbor.size(); ++j) {
				int neiIndex1 = v1Vert.neighbor[i];
				int neiIndex2 = v1Vert.neighbor[j];

				/*Face realFace;
				int b = faceMap.Get(Face(pair.v[1], neiIndex1, neiIndex2), realFace);
				if (b) 
				{*/
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

					auto n0 = found->Norm(vertices, vertMapper);
					auto n = newFace.Norm(vertices, vertMapper);
					if (n.Dot(n0) > -0.1f) 
					{
						realFaceV.push_back(*found);
						newFaceV.push_back(newFace);
					}
					else 
					{
						VERT(pair.v[0]).SetPosition(originPos);
						VERT(pair.v[0]).DeletePair(pair.index);
						VERT(pair.v[1]).DeletePair(pair.index);
						return false;
					}
				}
			}
		}
		for (int i = 0; i < realFaceV.size(); ++i) 
		{
			faceMap.erase(realFaceV[i]);
			faceMap.insert(newFaceV[i]);
			//faceMap.Remove(realFaceV[i]);
			//faceMap.Insert(newFaceV[i]);
		}
		realFaceV.clear();
		newFaceV.clear();

		// Get New Neighbor
		for (int i = 0; i < v1Vert.neighbor.size(); ++i)
		{
			int neighborIndex = v1Vert.neighbor[i];
			if (neighborIndex != pair.v[0])
			{
				if (!VERT(newIndex).IsNeighbor(neighborIndex))
				{
					VERT(newIndex).AddNeighbor(neighborIndex);
					VERT(neighborIndex).AddNeighbor(newIndex);
				}
				VERT(neighborIndex).DeleteNeighbor(pair.v[1]);
			}
		}
		VERT(newIndex).DeleteNeighbor(pair.v[1]);
		
		// Update Pairs
		for (int i = 0; i < v1Vert.pairs.size(); ++i) {
			int pairIndex = v1Vert.pairs[i];

			if (pairs[pairIndex].v[0] == pair.v[1]) {
				if (pairs[pairIndex].v[1] == pair.v[0]) {
					//pair between v[0] and v[1]
					ASTRA_CORE_ASSERT(pairIndex == pair.index, "Simplifier: Update Error.");
					VERT(newIndex).DeletePair(pairIndex);
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
					VERT(newIndex).DeletePair(pairIndex);
					continue;
				}
				else 
				{
					pairs[pairIndex].v[1] = newIndex;
				}
			}
			if (VERT(newIndex).HasPair(pairIndex, pairs))
			{
				heap.Remove(pairs[pairIndex]);
				if (pairs[pairIndex].v[0] == pair.v[0]) {
					VERT(pairs[pairIndex].v[1]).DeletePair(pairIndex);
				}
				else {
					VERT(pairs[pairIndex].v[0]).DeletePair(pairIndex);
				}
			}
			else {
				VERT(newIndex).AddPair(pairIndex);
			}
		}

		// Update Cost and Optimal Position
		VERT(newIndex).q += VERT(pair.v[1]).q;
		auto newIndexPairs = VERT(newIndex).pairs;
		for (int i = 0; i < newIndexPairs.size(); ++i)
		{
			int index = newIndexPairs[i];
			pairs[index].Update(vertices, vertMapper);
			heap.Update(pairs[index]);
		}
		DeleteVertex(pair.v[1]);
		return true;
	}
}