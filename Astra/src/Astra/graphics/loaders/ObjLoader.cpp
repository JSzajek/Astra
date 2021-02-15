#include "astra_pch.h"

#include "ObjLoader.h"

namespace Astra::Graphics
{
	ObjLoader::ObjLoader()
	{
	}

	void ObjLoader::BasicProcessing(std::ifstream& stream, std::string& line, bool normalMapped)
	{
		bool done = false;
		
		while (getline(stream, line))
		{
			if (line.empty()) { continue; }

			std::vector<std::string> split = StringUtils::Split(line);
			if (split.size() < 1) { continue; }
			switch (StringUtils::str2int(split[0].c_str()))
			{
			case StringUtils::str2int("v"):
				if (!normalMapped) {
					vertices.emplace_back(new Vertex(vertices.size(), Math::Vec3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3]))));
				}
				else {
					normVertices.emplace_back(new NormalVertex(normVertices.size(), Math::Vec3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3]))));
				}
				break;
			case StringUtils::str2int("vt"):
				textures.push_back(Math::Vec2(std::stof(split[1]), std::stof(split[2])));
				break;
			case StringUtils::str2int("vn"):
				normals.push_back(Math::Vec3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3])));
				break;
			case StringUtils::str2int("f"):
				done = true;
				break;
			}
			if (done) { break; }
		}
	}

	const VertexArray* ObjLoader::LoadObjectModelImpl(const std::string& filepath)
	{
		auto temp = m_loaded.find(filepath);
		if (temp != m_loaded.end())
		{
			return temp->second;
		}

		std::vector<float> verticesArray;
		std::vector<float> texturesArray;
		std::vector<float> normalsArray;

		std::ifstream stream(filepath);
		std::string line;

		BasicProcessing(stream, line, false);

		do {
			if (line.find('f') == std::string::npos) { continue; }

			std::vector<std::string> split = StringUtils::Split(line);
			if (line.find('/') != std::string::npos)
			{
				for (int i = 1; i <= 3; i++)
				{
					static_cast<void>(ProcessVertex(StringUtils::Split(split[i], '/')));
				}
			}
			else
			{
				for (int i = 1; i <= 3; i++)
				{
					indices.push_back(std::stoi(split[i]) - 1);
				}
			}
		} while (getline(stream, line));

		stream.close();

		RemoveUnused();
		texturesArray.resize(vertices.size() * 2);
		normalsArray.resize(vertices.size() * 3);
		verticesArray.resize(vertices.size() * 3);
		Convert(verticesArray, texturesArray, normalsArray);

		const VertexArray* result = Loader::Load(GL_TRIANGLES, verticesArray, indices, texturesArray, normalsArray);
		m_loaded[filepath] = result;

		for (Vertex* vertex : vertices)
		{
			delete vertex;
		}
		vertices.clear();
		textures.clear();
		normals.clear();
		indices.clear();

		return result;
	}

	const VertexArray* ObjLoader::LoadNormalMappedObjectModelImpl(const std::string& filepath)
	{
		auto temp = m_loaded.find(filepath);
		if (temp != m_loaded.end())
		{
			return temp->second;
		}

		std::vector<float> verticesArray;
		std::vector<float> texturesArray;
		std::vector<float> normalsArray;
		std::vector<float> tangentsArray;

		std::ifstream stream(filepath);
		std::string line;

		BasicProcessing(stream, line, true);

		do {
			if (line.find('f') == std::string::npos) { continue; }

			std::vector<std::string> split = StringUtils::Split(line);
			if (line.find('/') != std::string::npos)
			{
				// TODO: Figure out why nesting causes strange behavior
				NormalVertex* vert0 = ProcessNormVertex(StringUtils::Split(split[1], '/'));
				NormalVertex* vert1 = ProcessNormVertex(StringUtils::Split(split[2], '/'));
				NormalVertex* vert2 = ProcessNormVertex(StringUtils::Split(split[3], '/'));
				CalculateTangents(vert0, vert1, vert2);
			}
			else
			{
				for (int i = 1; i <= 3; i++)
				{
					indices.push_back(std::stoi(split[i]) - 1);
				}
			}
		} while (getline(stream, line));

		stream.close();

		RemoveNormUnused();
		texturesArray.resize(normVertices.size() * 2);
		normalsArray.resize(normVertices.size() * 3);
		verticesArray.resize(normVertices.size() * 3);
		tangentsArray.resize(normVertices.size() * 3);
		Convert(verticesArray, texturesArray, normalsArray, tangentsArray);

		const VertexArray* result = Loader::Load(GL_TRIANGLES, verticesArray, indices, texturesArray, normalsArray, tangentsArray);
		m_loaded[filepath] = result;

		for (NormalVertex* vertex : normVertices)
		{
			delete vertex;
		}
		normVertices.clear();
		textures.clear();
		normals.clear();
		indices.clear();

		return result;
	}

	float ObjLoader::Convert(std::vector<float>& verticesArray, std::vector<float>& texturesArray, std::vector<float>& normalsArray)
	{
		float furthest = 0;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			Vertex* current = vertices[i];
			if (current == NULL) { continue; }
			if (current->length > furthest)
			{
				furthest = current->length;
			}
			const Math::Vec2& texture = textures[current->textureIndex];
			const Math::Vec3& normal = normals[current->normalIndex];
			verticesArray[i * 3] = current->position.x;
			verticesArray[(i * 3) + 1] = current->position.y;
			verticesArray[(i * 3) + 2] = current->position.z;

			texturesArray[i * 2] = texture.x;
			texturesArray[(i * 2) + 1] = texture.y;

			normalsArray[i * 3] = normal.x;
			normalsArray[(i * 3) + 1] = normal.y;
			normalsArray[(i * 3) + 2] = normal.z;
		}
		return furthest;
	}

	float ObjLoader::Convert(std::vector<float>& verticesArray, std::vector<float>& texturesArray, std::vector<float>& normalsArray, std::vector<float>& tangentsArray)
	{
		float furthest = 0;
		for (size_t i = 0; i < normVertices.size(); i++)
		{
			NormalVertex* current = normVertices[i];
			if (current == NULL) { continue; }
			if (current->length > furthest)
			{
				furthest = current->length;
			}
			const Math::Vec2& texture = textures[current->textureIndex];
			const Math::Vec3& normal = normals[current->normalIndex];
			const Math::Vec3& tangent = current->avgTangent;
			verticesArray[i * 3] = current->position.x;
			verticesArray[(i * 3) + 1] = current->position.y;
			verticesArray[(i * 3) + 2] = current->position.z;

			texturesArray[i * 2] = texture.x;
			texturesArray[(i * 2) + 1] = texture.y;

			normalsArray[i * 3] = normal.x;
			normalsArray[(i * 3) + 1] = normal.y;
			normalsArray[(i * 3) + 2] = normal.z;

			tangentsArray[i * 3] = tangent.x;
			tangentsArray[(i * 3) + 1] = tangent.y;
			tangentsArray[(i * 3) + 2] = tangent.z;
		}
		return furthest;
	}

	void ObjLoader::CalculateTangents(NormalVertex* vert0, NormalVertex* vert1, NormalVertex* vert2)
	{
		Math::Vec3 deltaPos1 = vert1->position;
		deltaPos1 -= vert0->position;
		Math::Vec3 deltaPos2 = vert2->position;
		deltaPos2 -= vert0->position;

		Math::Vec2 uv0 = textures[vert0->textureIndex];
		Math::Vec2 uv1 = textures[vert1->textureIndex];
		Math::Vec2 uv2 = textures[vert2->textureIndex];

		Math::Vec2 deltaUv1 = uv1;
		deltaUv1 -= uv0;
		Math::Vec2 deltaUv2 = uv2;
		deltaUv2 -= uv0;

		float r = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);
		deltaPos1 *= deltaUv2.y;
		deltaPos2 *= deltaUv1.y;
		Math::Vec3 tangent = deltaPos1;
		tangent -= deltaPos2;
		tangent *= r;
		vert0->AddTangent(tangent);
		vert1->AddTangent(tangent);
		vert2->AddTangent(tangent);
	}

	Vertex* ObjLoader::ProcessVertex(const std::vector<std::string>& data)
	{
		int currentVertexPointer = std::stoi(data[0]) - 1;
		Vertex* currentVertex = vertices[currentVertexPointer];
		
		int textureIndex = textures.size() > 0 && !data[1].empty() ? std::stoi(data[1]) - 1 : -1;
		int normalIndex = normals.size() > 0 && !data[2].empty() ? std::stoi(data[2]) - 1 : -1;
		
		if (!currentVertex->IsSet())
		{
			currentVertex->textureIndex = textureIndex;
			currentVertex->normalIndex = normalIndex;
			indices.push_back(currentVertexPointer);
			return currentVertex;
		}
		else
		{
			return AlreadyProcessed(currentVertex, textureIndex, normalIndex);
		}
	}

	NormalVertex* ObjLoader::ProcessNormVertex(const std::vector<std::string>& data)
	{
		int currentVertexPointer = std::stoi(data[0]) - 1;
		NormalVertex* currentVertex = normVertices[currentVertexPointer];

		int textureIndex = textures.size() > 0 && !data[1].empty() ? std::stoi(data[1]) - 1 : -1;
		int normalIndex = normals.size() > 0 && !data[2].empty() ? std::stoi(data[2]) - 1 : -1;

		if (!currentVertex->IsSet())
		{
			currentVertex->textureIndex = textureIndex;
			currentVertex->normalIndex = normalIndex;
			indices.push_back(currentVertexPointer);
			return currentVertex;
		}
		else
		{
			return AlreadyNormProcessed(currentVertex, textureIndex, normalIndex);
		}
	}

	NormalVertex* ObjLoader::AlreadyNormProcessed(NormalVertex* previous, int textureIndex, int normalIndex)
	{
		if (previous->SameTextureAndNormal(textureIndex, normalIndex))
		{
			indices.push_back(previous->index);
			return previous;
		}
		else
		{
			NormalVertex* other = previous->duplicate;
			if (other != NULL)
			{
				return AlreadyNormProcessed(other, textureIndex, normalIndex);
			}
			else
			{
				NormalVertex* duplicate = new NormalVertex(normVertices.size(), previous->position);
				duplicate->tangents = previous->tangents;

				duplicate->textureIndex = textureIndex;
				duplicate->normalIndex = normalIndex;
				previous->duplicate = duplicate;
				normVertices.emplace_back(duplicate);
				indices.push_back(duplicate->index);
				return duplicate;
			}
		}
	}

	Vertex* ObjLoader::AlreadyProcessed(Vertex* previous, int textureIndex, int normalIndex)
	{
		if (previous->SameTextureAndNormal(textureIndex, normalIndex))
		{
			indices.push_back(previous->index);
			return previous;
		}
		else
		{
			Vertex* other = previous->duplicate;
			if (other != NULL)
			{
				return AlreadyProcessed(other, textureIndex, normalIndex);
			}
			else
			{
				Vertex* duplicate = new Vertex(vertices.size(), previous->position);
				duplicate->textureIndex = textureIndex;
				duplicate->normalIndex = normalIndex;
				previous->duplicate = duplicate;
				vertices.emplace_back(duplicate);
				indices.push_back(duplicate->index);
				return duplicate;
			}
		}
	}

	void ObjLoader::RemoveUnused()
	{
		std::vector<Vertex*>::iterator iter = vertices.begin();
		while (iter != vertices.end())
		{
			if (!(*iter)->IsSet())
			{
				void* ptr = *iter;
				iter = vertices.erase(iter);
				delete ptr;
			}
			else
			{
				++iter;
			}
		}
	}

	void ObjLoader::RemoveNormUnused()
	{
		std::vector<NormalVertex*>::iterator iter = normVertices.begin();
		while (iter != normVertices.end())
		{
			(*iter)->AverageTangents();
			if (!(*iter)->IsSet())
			{
				void* ptr = *iter;
				iter = normVertices.erase(iter);
				delete ptr;
			}
			else
			{
				++iter;
			}
		}
	}
}