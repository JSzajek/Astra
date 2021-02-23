#include "astra_pch.h"

#include "Model.h"

#include "Astra/Application.h"
#include "Astra/graphics/loaders/Loader.h"
#include "Astra/graphics/ResourceManager.h"
#include "Astra/math/Mat4Utils.h"
#include "Astra/graphics/entities/utility/AssimpReader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>
#include <GL/glew.h>

namespace Astra::Graphics
{
	Model::Model(const char* const name, const char* const filepath, bool calcTangents)
		: Spatial(name), m_renderId(std::hash<std::string>{}(filepath)), m_normals(false), selectedModelMatrix(1.0f),
			m_textureIndex(0), m_rowCount(1), m_selected(0), m_boneCounter(0), m_animator(NULL)
	{
		LoadModel(filepath, calcTangents);
	}

	Model::Model(const char* const filepath, bool calcTangents)
		: Model("Model", filepath, calcTangents)
	{
	}

	Model::Model(const Model& other)
		: Spatial(other), m_directory(other.m_directory), m_normals(other.m_normals),
			m_textureIndex(other.m_textureIndex), m_rowCount(other.m_rowCount),
			m_selected(other.m_selected), m_meshes(other.m_meshes),
			selectedModelMatrix(other.selectedModelMatrix), m_boneCounter(other.m_boneCounter),
			m_animator(other.m_animator), m_animations(other.m_animations)
	{
	}

	Model::~Model()
	{
	}

	void Model::Free()
	{
		for (auto& mesh : m_meshes)
		{
			mesh.Unload();
		}
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
		ASTRA_CORE_ASSERT(scene && scene->mFlags | AI_SCENE_FLAGS_INCOMPLETE && scene->mRootNode, "Model: Assimp Error Loading")
		
		m_directory = filepath.substr(0, filepath.find_last_of('/'));

		if (scene->mRootNode)
		{
			ProcessNode(scene->mRootNode, scene);
		}
		else
		{
			ASTRA_CORE_ERROR("Model: No root node");
		}
	
		// Load Animations in Model
		if (scene->HasAnimations())
		{
			m_animations.reserve(scene->mNumAnimations);
			
			// For now only check first animation
			m_animations.push_back(Animation(scene->mAnimations[0], scene->mRootNode, m_boneInfoMap, m_boneCounter));
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

			// Extract Animation Information
			if (mesh->HasBones())
			{
				ExtractBoneWeightForVertices(vertices, mesh, scene);
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

			// Extract Animation Information
			if (mesh->HasBones())
			{
				ExtractBoneWeightForVertices(vertices, mesh, scene);
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
					// Read Texture from Filepath - External Texture
					auto filename = m_directory + '/' + std::string(string.C_Str());
					data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
				}
				texture->type = texType;
				Loader::LoadTexture(texture, data, width, height, nrComponents, texType == TextureType::DiffuseMap);
				stbi_image_free(data);
				
				textures.push_back(texture);
			}
		}
		return { textures, _hash };
	}

	template<class Vertex>
	void Model::SetVertexBoneData(Vertex& vertex, int id, float weight)
	{
		for (unsigned int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			if (vertex.BoneIds[i] < 0)
			{
				vertex.BoneIds[i] = id;
				vertex.Weights[i] = weight;
				break;
			}
		}
	}

	template<class Vertex>
	void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
	{
		for (unsigned int index = 0; index < mesh->mNumBones; ++index)
		{
			int id = -1;
			std::string name = mesh->mBones[index]->mName.C_Str();
			if (m_boneInfoMap.find(name) == m_boneInfoMap.end())
			{
				BoneInfo info(m_boneCounter, ConvertAiMatrix(mesh->mBones[index]->mOffsetMatrix));
				m_boneInfoMap[name] = info;
				id = m_boneCounter++;
			}
			else
			{
				id = m_boneInfoMap[name].Id;
			}
			ASTRA_CORE_ASSERT(id != -1, "Model: Error in Animation Bone Id Matching.");

			auto weights = mesh->mBones[index]->mWeights;
			int numWeights = mesh->mBones[index]->mNumWeights;
			for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
			{
				unsigned int vertexId = weights[weightIndex].mVertexId;
				float weight = weights[weightIndex].mWeight;
				ASTRA_CORE_ASSERT(vertexId <= vertices.size(), "Model: Out of Bounds Vertex Id.");
				SetVertexBoneData(vertices[vertexId], id, weight);
			}
		}
	}

	void Model::UpdateMatrices()
	{
		if (m_selected)
		{
			Math::Vec3 scale = GetScale();
			scale *= 1.1f;
			selectedModelMatrix = Math::Mat4Utils::Transformation(GetTranslation(), GetRotation(), scale);
		}
		Spatial::UpdateMatrices();
	}
}