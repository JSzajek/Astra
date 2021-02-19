#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

namespace Astra::Graphics
{
	class Model
	{
	private:
		std::vector<Mesh> m_meshes;
		std::string m_directory;
	public:
		Model(const char* const filepath);
	private:
		void LoadModel(std::string filepath);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Tex> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

		unsigned int TextureFromFile(const char* const filepath, const std::string& directory, bool gamma = false);
	};
}