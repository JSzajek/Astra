#include "ObjLoader.h"

namespace Astra::Graphics
{
	ObjLoader::ObjLoader()
	{
	}

	const VertexArray* ObjLoader::LoadObjectModelImpl(const std::string& filepath)
	{
		vertices.clear();
		textures.clear();
		normals.clear();
		indices.clear();

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
				vertices.push_back(Math::Vec3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3])));
				break;
			case StringUtils::str2int("vt"):
				textures.push_back(Math::Vec2(std::stof(split[1]), std::stof(split[2])));
				break;
			case StringUtils::str2int("vn"):
				normals.push_back(Math::Vec3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3])));
				break;
			case StringUtils::str2int("f"):
				textureCount = vertices.size() * 2;
				normalsCount = vertices.size() * 3;
				texturesArray.resize(textureCount);
				normalsArray.resize(normalsCount);
				done = true;
				break;
			}
			if (done) { break; }
		}

		while (getline(stream, line))
		{
			if (line.find('f') == std::string::npos) { continue; }

			std::vector<std::string> split = StringUtils::Split(line);
			if (line.find('/') != std::string::npos)
			{
				for (int i = 1; i <= 3; i++)
				{
					ProcessVertex(StringUtils::Split(split[i], '/'), indices, textures, normals, texturesArray, normalsArray);
				}
			}
			else
			{
				split.erase(split.begin());
				for (int i = 1; i <= 3; i++)
				{
					ProcessVertex(split, indices, textures, normals, texturesArray, normalsArray);
				}
			}
		}
		stream.close();
		
		vertexCount = vertices.size() * 3;
		verticesArray.reserve(vertexCount);
		
		for (const Math::Vec3& vertex : vertices)
		{
			verticesArray.push_back(vertex.x);
			verticesArray.push_back(vertex.y);
			verticesArray.push_back(vertex.z);
		}

		return Loader::Load(GL_TRIANGLES,
							verticesArray,
							indices,
							texturesArray,
							normalsArray);
	}

	void ObjLoader::ProcessVertex(const std::vector<std::string>& data, std::vector<int>& indices, std::vector<Math::Vec2>& textures, std::vector<Math::Vec3>& normals, std::vector<float>& textureArray, std::vector<float>& normalsArray)
	{
		int currentVertexPointer = std::stoi(data[0]) - 1;
		indices.push_back(currentVertexPointer);
		if (textures.size() > 0 && !data[1].empty())
		{
			const Math::Vec2& current = textures[std::stoi(data[1]) - 1];
			textureArray[currentVertexPointer * 2] = current.x;
			textureArray[(currentVertexPointer * 2) + 1] = current.y;
		}

		if (normals.size() > 0)
		{
			const Math::Vec3& current = normals[std::stoi(data[2]) - 1];
			normalsArray[currentVertexPointer * 3] = current.x;
			normalsArray[(currentVertexPointer * 3) + 1] = current.y;
			normalsArray[(currentVertexPointer * 3) + 2] = current.z;
		}
	}
}