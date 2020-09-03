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
	class ObjLoader
	{
	private:
		std::vector<Math::Vec3> vertices;
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

		void ProcessVertex(const std::vector<std::string>& data, std::vector<int>& indices, std::vector<Math::Vec2>& textures, std::vector<Math::Vec3>& normals, std::vector<float>& textureArray, std::vector<float>& normalsArray);
	};
}