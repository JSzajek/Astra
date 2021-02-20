#include "astra_pch.h"

#include "Model.h"

#include "Astra/Application.h"
//#include "Astra/utils/Unique.h"
#include "Astra/graphics/loaders/Loader.h"
#include "Astra/graphics/ResourceManager.h"
#include "Astra/math/Mat4Utils.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>
//#include <stb_image/stb_image_write.h>
#include <GL/glew.h>

namespace Astra::Graphics
{
	Model::Model(const char* const filepath, bool calcTangents)
		: m_normals(false), m_uid(std::hash<std::string>{}(filepath)),
			selectedModelMatrix(new Math::Mat4()),
			m_textureIndex(0), m_rowCount(1),
			m_selected(0)
	{
		LoadModel(filepath, calcTangents);
	}

	Model::Model(const Model& other)
		: Spatial(other), m_directory(other.m_directory), m_normals(other.m_normals),
			m_textureIndex(other.m_textureIndex), m_rowCount(other.m_rowCount),
			m_selected(other.m_selected), m_meshes(other.m_meshes), m_uid(other.m_uid),
			selectedModelMatrix(new Math::Mat4(*other.selectedModelMatrix))
	{
	}

	Model::~Model()
	{
		delete selectedModelMatrix;
	}

	void Model::LoadModel(std::string filepath, bool calcTangents)
	{
		Assimp::Importer importer;
		unsigned int pFlag = aiProcess_OptimizeMeshes | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;
		if (calcTangents)
		{
			pFlag |= aiProcess_CalcTangentSpace;
		}
		const aiScene* scene = importer.ReadFile(filepath, pFlag);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			ASTRA_CORE_ERROR("Model: Assimp Error Loading");
		}
		
		m_directory = filepath.substr(0, filepath.find_last_of('/'));

		if (scene && scene->mRootNode)
		{
			ProcessNode(scene->mRootNode, scene);
		}
		else
		{
			ASTRA_CORE_ERROR("Model: No root node");
		}
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			auto* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(ProcessMesh(mesh, scene));
		}

		// Add to Children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<int> indices;
		auto* material = ProcessMaterials(mesh, scene);

		if (mesh->HasTangentsAndBitangents() && (material->HasTexture(TextureType::NormalMap) || material->HasTexture(TextureType::HeightMap)))
		{
			std::vector<NormalVertex> vertices;

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				NormalVertex vertex;
				// Process vertex positions, normals, tangents, bitangents and texture coordinates
				vertex.Position = Math::Vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				vertex.Normal = Math::Vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				vertex.Tangent = Math::Vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

				vertex.TextureCoords = mesh->mTextureCoords[0] ? Math::Vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
																	: Math::Vec2::Zero;
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
			return Mesh(vertices, indices, material);
		}
		else
		{
			std::vector<Vertex> vertices;
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				// Process vertex positions, normals, and texture coordinates
				vertex.Position = Math::Vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				vertex.Normal = Math::Vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				vertex.TextureCoords = mesh->mTextureCoords[0] ? Math::Vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
																	: Math::Vec2::Zero;
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
			return Mesh(vertices, indices, material);
		}
	}

	ImageMaterial* Model::ProcessMaterials(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Texture*> textures;
		size_t textureHash = 0;

		// Process materials
		ASTRA_CORE_ASSERT(mesh->mMaterialIndex >= 0, "Model: No Materials Found");

		auto* material = scene->mMaterials[mesh->mMaterialIndex];
		for (int i = aiTextureType_DIFFUSE; i < 12; i++)
		{
			auto t = material->GetTextureCount((aiTextureType)i);
		}

		auto [diffuseMaps, diffuseHash] = LoadMaterialTextures(scene, material, aiTextureType_DIFFUSE, TextureType::DiffuseMap);
		auto [specularMaps, specularHash] = LoadMaterialTextures(scene, material, aiTextureType_SPECULAR, TextureType::SpecularMap);
		auto [normalMaps, normalHash] = LoadMaterialTextures(scene, material, aiTextureType_NORMALS, TextureType::NormalMap);
		auto [heightMaps, heightHash] = LoadMaterialTextures(scene, material, aiTextureType_DISPLACEMENT, TextureType::HeightMap);
		auto [emissionMaps, emissionHash] = LoadMaterialTextures(scene, material, aiTextureType_EMISSIVE, TextureType::EmissionMap);

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());

		textureHash += diffuseHash + specularHash + normalHash + heightHash + emissionHash;
		m_normals = normalMaps.size() > 0 || heightMaps.size() > 0;

		return ResourceManager::LoadMaterial(textures, textureHash);
	}

	std::tuple<std::vector<Texture*>, size_t> Model::LoadMaterialTextures(const aiScene* scene, aiMaterial* mat, aiTextureType type, TextureType texType)
	{
		stbi_set_flip_vertically_on_load(false);

		std::vector<Texture*> textures;
		textures.reserve(mat->GetTextureCount(type));
		size_t _hash = 0;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString string;
			mat->Get(AI_MATKEY_TEXTURE(type, i), string);

			_hash += std::hash<std::string>{}(string.C_Str());
			Texture* texture = NULL;
			if (ResourceManager::QueryTexture(string.C_Str(), &texture))
			{
				textures.push_back(texture);
			}
			else if (texture)
			{
				unsigned char* data;
				int width, height, nrComponents;
				if (const auto* tex = scene->GetEmbeddedTexture(string.C_Str()))
				{
					// Read Texture from Memory - Embedded Texture
					data = !tex->mHeight ? stbi_load_from_memory(reinterpret_cast<unsigned char*>(tex->pcData), tex->mWidth, &width, &height, &nrComponents, 0)
											: stbi_load_from_memory(reinterpret_cast<unsigned char*>(tex->pcData), tex->mWidth * tex->mHeight, &width, &height, &nrComponents, 0);
				}
				else
				{
					//// Read Texture from Filepath - External Texture
					auto filename = m_directory + '/' + std::string(string.C_Str());
					data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
				}
				texture->type = texType;
				Loader::LoadTexture(texture, data, width, height, nrComponents, texType == TextureType::DiffuseMap);
				stbi_image_free(data);
				
				//glBindTexture(GL_TEXTURE_2D, texture->id);
				//glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
				//glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
				//
				//GLenum format = nrComponents == 1 ? GL_RED : nrComponents == 3 ? GL_RGB : GL_RGBA;

				//void* pixels = malloc(sizeof(float) * width * height * nrComponents /*RGBA*/); // Allocate Enough Space For Image

				//// Gathers Image Data from Texture Buffer based on ID and re-buffers with new internal format
				//glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, pixels);
				//glBindTexture(GL_TEXTURE_2D, 0);
				//stbi_write_jpg("res/test.jpg", width, height, nrComponents, pixels, 80);
				//free(pixels);

				textures.push_back(texture);
			}
		}
		return { textures, _hash };
	}

	void Model::UpdateMatrices()
	{
		if (m_selected)
		{
			Math::Vec3 scale = GetScale();
			scale *= 1.1f;
			*selectedModelMatrix = Math::Mat4Utils::Transformation(GetTranslation(), GetRotation(), scale);
		}
		Spatial::UpdateMatrices();
	}
}