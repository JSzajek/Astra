#pragma once

#include <GL/glew.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

#include "../buffers/VertexArray.h"
#include "../../math/Maths.h"
#include "../../utils/StringUtils.h"
#include "Loader.h"

namespace Astra::Graphics
{
	struct TempVertex
	{
		Math::Vec3 position;
		int textureIndex;
		int normalIndex;
		int index;
		float length;

		TempVertex* duplicate;

		TempVertex(int index, const Math::Vec3& position)
			: index(index), position(position), textureIndex(-1), normalIndex(-1), duplicate(NULL)
		{
			length = position.Magnitude();
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

	class ObjLoader
	{
	private:
		std::vector<TempVertex*> vertices;
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

	private:
		ObjLoader();
		const VertexArray* LoadObjectModelImpl(const std::string& filepath);

		float Convert(std::vector<float>& verticesArray, std::vector<float>& texturesArray, std::vector<float>& normalsArray);

		void ProcessVertex(const std::vector<std::string>& data);
		void AlreadyProcessed(TempVertex* previous, int textureIndex, int normalIndex);
		void RemoveUnused();
	};
}