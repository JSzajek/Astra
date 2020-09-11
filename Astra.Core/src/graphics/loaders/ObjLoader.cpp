#include "ObjLoader.h"

namespace Astra::Graphics
{
	ObjLoader::ObjLoader()
	{
	}

	const VertexArray* ObjLoader::LoadObjectModelImpl(const std::string& filepath)
	{
		bool done = false;

		std::vector<float> verticesArray;
		std::vector<float> texturesArray;
		std::vector<float> normalsArray;

		unsigned int vertexCount, textureCount, normalsCount;

		std::ifstream stream(filepath);
		std::string line;
		while (getline(stream, line))
		{
			if (line.empty()) { continue; }

			std::vector<std::string> split = StringUtils::Split(line);
			if (split.size() < 1) { continue; }
			switch (StringUtils::str2int(split[0].c_str()))
			{
			case StringUtils::str2int("v"):
				vertices.emplace_back(new TempVertex(vertices.size(), Math::Vec3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3]))));
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

		do 
		{
			if (line.find('f') == std::string::npos) { continue; }

			std::vector<std::string> split = StringUtils::Split(line);
			if (line.find('/') != std::string::npos)
			{
				for (int i = 1; i <= 3; i++)
				{
					ProcessVertex(StringUtils::Split(split[i], '/'));
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

		for (TempVertex* vertex : vertices)
		{
			delete vertex;
		}
		vertices.clear();
		textures.clear();
		normals.clear();
		indices.clear();

		return result;
	}

	float ObjLoader::Convert(std::vector<float>& verticesArray, std::vector<float>& texturesArray, std::vector<float>& normalsArray)
	{
		float furthest = 0;
		for (int i = 0; i < vertices.size(); i++)
		{
			TempVertex* current = vertices[i];
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

	void ObjLoader::ProcessVertex(const std::vector<std::string>& data)
	{
		int currentVertexPointer = std::stoi(data[0]) - 1;
		TempVertex* currentVertex = vertices[currentVertexPointer];
		
		int textureIndex = textures.size() > 0 && !data[1].empty() ? std::stoi(data[1]) - 1 : -1;
		int normalIndex = normals.size() > 0 && !data[2].empty() ? std::stoi(data[2]) - 1 : -1;
		
		if (!currentVertex->IsSet())
		{
			currentVertex->textureIndex = textureIndex;
			currentVertex->normalIndex = normalIndex;
			indices.push_back(currentVertexPointer);
		}
		else
		{
			AlreadyProcessed(currentVertex, textureIndex, normalIndex);
		}
	}

	void ObjLoader::AlreadyProcessed(TempVertex* previous, int textureIndex, int normalIndex)
	{
		if (previous->SameTextureAndNormal(textureIndex, normalIndex))
		{
			indices.push_back(previous->index);
		}
		else
		{
			TempVertex* other = previous->duplicate;
			if (other != NULL)
			{
				AlreadyProcessed(other, textureIndex, normalIndex);
			}
			else
			{
				TempVertex* duplicate = new TempVertex(vertices.size(), previous->position);
				duplicate->textureIndex = textureIndex;
				duplicate->normalIndex = normalIndex;
				previous->duplicate = duplicate;
				vertices.emplace_back(duplicate);
				indices.push_back(duplicate->index);
			}
		}
	}

	void ObjLoader::RemoveUnused()
	{
		std::vector<TempVertex*>::iterator iter = vertices.begin();
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
}