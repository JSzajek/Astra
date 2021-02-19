#include "astra_pch.h"

#include "Model.h"

#include "Astra/Application.h"

#include <stb_image/stb_image.h>
#include <GL/glew.h>

namespace Astra::Graphics
{
	Model::Model(const char* const filepath)
	{
		LoadModel(filepath);
	}

	void Model::LoadModel(std::string filepath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

		//ASTRA_CORE_ASSERT(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode, "Model: Assimp Error Loading");
		
		m_directory = filepath.substr(0, filepath.find_last_of('/'));

		if (scene && scene->mRootNode)
			ProcessNode(scene->mRootNode, scene);
		else
			ASTRA_CORE_ERROR("Model: No root node");
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			auto* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(ProcessMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Tex> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			// Process vertex positions, normals, and texture coordinates
			vertex.Position = Math::Vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertex.Normal = Math::Vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			if (mesh->mTextureCoords[0])
			{
				vertex.TextureCoords = Math::Vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			}
			else
			{
				vertex.TextureCoords = Math::Vec2::Zero;
			}
			vertices.push_back(vertex);
		}

		// Process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			auto face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			auto* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Tex> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			std::vector<Tex> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			std::vector<Tex> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			std::vector<Tex> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");

			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		}
		return Mesh(vertices, indices, textures);
	}

	std::vector<Tex> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<Tex> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString string;
			mat->GetTexture(type, i, &string);
			Tex texture;
			texture.id = TextureFromFile(string.C_Str(), m_directory);
			texture.type = typeName;
			//texture.path = string.C_Str();
			textures.push_back(texture);
		}
		return textures;
	}

	unsigned int Model::TextureFromFile(const char* const filepath, const std::string& directory, bool gamma)
	{
		auto filename = std::string(filepath);
		filename = directory + '/' + filename;

		unsigned int textureId;
		glGenTextures(1, &textureId);

		int width, height, nrComponents;
		auto* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format = nrComponents == 1 ? GL_RED : nrComponents == 3 ? GL_RGB : GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			ASTRA_CORE_ERROR("Model: Texture failed to load at path {0}", filepath);
		}
		return textureId;
	}
}