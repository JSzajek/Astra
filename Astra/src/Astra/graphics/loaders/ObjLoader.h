#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

#include "../buffers/VertexArray.h"
#include "../materials/ImageMaterial.h"
#include "../../math/Maths.h"
#include "../../utils/StringUtils.h"
#include "Loader.h"

namespace Astra::Graphics
{
	struct Vertex
	{
	public:
		Math::Vec3 position;
		int textureIndex;
		int normalIndex;
		int index;
		float length;

		Vertex* duplicate;

		Vertex(int index, const Math::Vec3& position)
			: index(index), position(position), textureIndex(-1), normalIndex(-1), duplicate(NULL)
		{
			length = position.Length();
		}

		inline bool IsSet() const
		{
			return textureIndex != -1 && normalIndex != -1;
		}
		inline bool SameTextureAndNormal(int otherTextureIndex, int otherNormalIndex) const
		{
			return textureIndex == otherTextureIndex && normalIndex == otherNormalIndex;
		}
	};

	struct NormalVertex : Vertex
	{
	public:
		std::vector<Math::Vec3> tangents;
		Math::Vec3 avgTangent;

		NormalVertex* duplicate;
		
		NormalVertex(int index, const Math::Vec3& position)
			: Vertex(index, position), duplicate(NULL), avgTangent(Math::Vec3::Zero)
		{
			length = position.Length();
		}

		inline void AddTangent(const Math::Vec3& tangent) { tangents.push_back(tangent); }

		void AverageTangents()
		{
			if (tangents.size() == 0) { return; }
			for (const Math::Vec3& tangent : tangents)
			{
				avgTangent += tangent;
			}
			avgTangent.Normalize();
		}
	};

	class ObjLoader
	{
	private:
		std::unordered_map<std::string, const VertexArray*> m_loaded;
	
		std::vector<Vertex*> vertices;
		std::vector<NormalVertex*> normVertices;
		std::vector<Math::Vec2> textures;
		std::vector<Math::Vec3> normals;
		std::vector<int> indices;
	public:
		ObjLoader(const ObjLoader&) = delete;
		void operator=(const ObjLoader&) = delete;

		static ObjLoader& Get()
		{
			static ObjLoader instance;
			return instance;
		}

		static const VertexArray* LoadObjectModel(const char* const filepath)
		{
			return Get().LoadObjectModelImpl(filepath);
		}

		static const VertexArray* LoadNormalMappedObjectModel(const char* const filepath)
		{
			return Get().LoadNormalMappedObjectModelImpl(filepath);
		}

	private:
		ObjLoader();
		const VertexArray* LoadObjectModelImpl(const std::string& filepath);
		const VertexArray* LoadNormalMappedObjectModelImpl(const std::string& filepath);
	private:
		void BasicProcessing(std::ifstream& stream, std::string& line, bool normalMapped);
		float Convert(std::vector<float>& verticesArray, std::vector<float>& texturesArray, std::vector<float>& normalsArray);
		float Convert(std::vector<float>& verticesArray, std::vector<float>& texturesArray, std::vector<float>& normalsArray, std::vector<float>& tangentsArray);
		void CalculateTangents(NormalVertex* vert0, NormalVertex* vert1, NormalVertex* vert2);
		
		Vertex* ProcessVertex(const std::vector<std::string>& data);
		Vertex* AlreadyProcessed(Vertex* previous, int textureIndex, int normalIndex);
		
		NormalVertex* ProcessNormVertex(const std::vector<std::string>& data);
		NormalVertex* AlreadyNormProcessed(NormalVertex* previous, int textureIndex, int normalIndex);

		void RemoveUnused();
		void RemoveNormUnused();
	};
}