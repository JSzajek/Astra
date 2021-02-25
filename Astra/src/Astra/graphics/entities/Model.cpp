#include "astra_pch.h"

#include "Model.h"

#include "Astra/Application.h"
#include "Astra/graphics/loaders/Loader.h"
#include "Astra/graphics/Resource.h"
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
			m_textureIndex(0), m_rowCount(1), m_selected(0), m_boneCounter(0), m_animator(NULL), m_mesh(NULL)
	{
		LoadModel(filepath, calcTangents);
	}

	Model::Model(const char* const filepath, bool calcTangents)
		: Model("", filepath, calcTangents)
	{
	}

	Model::Model(const Model& other)
		: Spatial(other), m_renderId(other.m_renderId), m_directory(other.m_directory), m_normals(other.m_normals),
			m_textureIndex(other.m_textureIndex), m_rowCount(other.m_rowCount),
			m_selected(other.m_selected), m_mesh(other.m_mesh),
			selectedModelMatrix(other.selectedModelMatrix), m_boneCounter(other.m_boneCounter),
			m_animator(other.m_animator), m_animations(other.m_animations), m_material(other.m_material)
	{
		//Resource::Remark(m_mesh);
	}

	void Model::operator=(const Model& other)
	{
		Name = other.Name;
		m_uid = other.m_uid;

		m_modelMatrix = new Math::Mat4(*other.m_modelMatrix);
		m_normalMatrix = new Math::Mat4(*other.m_normalMatrix);
		memcpy(m_data, other.m_data, sizeof(m_data));

		m_renderId = other.m_renderId; 
		m_directory = other.m_directory;
		m_normals = other.m_normals;

		m_textureIndex = other.m_textureIndex; 
		m_rowCount = other.m_rowCount;

		m_selected = other.m_selected; 
		selectedModelMatrix = other.selectedModelMatrix;
		m_boneCounter = other.m_boneCounter;
		m_animator = other.m_animator; 
		m_animations = other.m_animations; 
		m_material = other.m_material;

		m_mesh = other.m_mesh;
		//TRACK(m_mesh);
	}

	Model::~Model()
	{
		//UNLOAD(m_mesh);
	}

	void Model::LoadModel(std::string filepath, bool calcTangents)
	{
		// TODO: Look into using global scale tag: aiProcess_GlobalScale

		Assimp::Importer importer;
		unsigned int pFlag = aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
		if (calcTangents)
		{
			pFlag |= aiProcess_CalcTangentSpace;
		}
		const aiScene* scene = importer.ReadFile(filepath, pFlag);
		ASTRA_CORE_ASSERT(scene && scene->mFlags | AI_SCENE_FLAGS_INCOMPLETE && scene->mRootNode, "Model: Assimp Error Loading")
		
		m_directory = filepath.substr(0, filepath.find_last_of('/'));

		if (scene->mRootNode)
		{
			ProcessNode(scene->mRootNode, scene, filepath);
		}
		else
		{
			ASTRA_CORE_ERROR("Model: No root node");
		}
	
		// Load Animations in Model
		if (scene->HasAnimations())
		{
			m_animations.reserve(scene->mNumAnimations);
			
			for (unsigned int i = 0; i < scene->mNumAnimations; ++i)
			{
				m_animations[scene->mAnimations[i]->mName.C_Str()] = Animation(scene->mAnimations[i], scene->mRootNode, m_boneInfoMap, m_boneCounter);
			}
		}
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene, const std::string& filepath)
	{
		// Model can only have one mesh - rest should be branched off and put as children of "parent" Model.
		if (!m_mesh && node->mNumMeshes > 0)
		{
			std::tie(m_mesh, m_material) = ProcessMesh(scene->mMeshes[node->mMeshes[0]], scene, filepath);
		}

		// Model wasn't in meshes for some reason check first child
		if (!m_mesh && node->mNumChildren > 0)
		{
			ProcessNode(node->mChildren[0], scene, filepath);
		}

		// Add Children
		/*if (node->mNumChildren > 0)
		{
		}*/
	}

	std::tuple<Asset<Mesh>, ImageMaterial> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& filepath)
	{
		const auto& material = ProcessMaterials(mesh, scene);

		auto normalMapped = mesh->HasTangentsAndBitangents() && (material.HasTexture(TextureType::NormalMap) || material.HasTexture(TextureType::HeightMap));
		return { Resource::LoadMesh(MeshCreationSpec(filepath, mesh, scene, &m_boneInfoMap, &m_boneCounter, normalMapped)), material };
	}

	ImageMaterial Model::ProcessMaterials(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Asset<Texture>> textures;
		size_t textureHash = 0;

		// Process materials
		ASTRA_CORE_ASSERT(mesh->mMaterialIndex >= 0, "Model: No Materials Found");

		auto* material = scene->mMaterials[mesh->mMaterialIndex];
		auto diffuseMaps = LoadMaterialTextures(scene, material, aiTextureType_DIFFUSE, TextureType::DiffuseMap);
		auto specularMaps = LoadMaterialTextures(scene, material, aiTextureType_SPECULAR, TextureType::SpecularMap);
		auto normalMaps = LoadMaterialTextures(scene, material, aiTextureType_NORMALS, TextureType::NormalMap);
		auto heightMaps = LoadMaterialTextures(scene, material, aiTextureType_DISPLACEMENT, TextureType::HeightMap);
		auto emissionMaps = LoadMaterialTextures(scene, material, aiTextureType_EMISSIVE, TextureType::EmissionMap);

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());

		m_normals = normalMaps.size() > 0 || heightMaps.size() > 0;
		return ImageMaterial(textures);
	}

	std::vector<Asset<Texture>> Model::LoadMaterialTextures(const aiScene* scene, aiMaterial* mat, aiTextureType type, TextureType texType)
	{
		stbi_set_flip_vertically_on_load(false);

		std::vector<Asset<Texture>> textures;
		textures.reserve(mat->GetTextureCount(type));
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString string;
			mat->Get(AI_MATKEY_TEXTURE(type, i), string);
			auto texture = Resource::LoadTexture(TextureCreationSpec(string.C_Str(), m_directory, scene, texType == TextureType::DiffuseMap, false));
			texture->type = texType;
			textures.push_back(texture);
		}
		return textures;
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