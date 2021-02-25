#include "astra_pch.h"

#include "Resource.h"
#include "Astra/Application.h"
#include "Astra/graphics/entities/utility/AssimpReader.h"

#include <assimp/scene.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

namespace Astra::Graphics
{
	Resource::~Resource()
	{
	#if ASTRA_DEBUG
		for (const auto& dir : m_loadedMeshes)
		{
			ASTRA_CORE_ASSERT(dir.second.use_count() == 1, "Resource: Dangling Mesh Pointer at Application closure.");
		}
		for (const auto& dir : m_loadedTextures)
		{
			ASTRA_CORE_ASSERT(dir.second.use_count() == 1, "Resource: Dangling Texture Pointer at Application closure.");
		}
		for (const auto& dir : m_loadedCubeTextures)
		{
			ASTRA_CORE_ASSERT(dir.second.use_count() == 1, "Resource: Dangling CubeMapTexture Pointer at Application closure.");
		}
		for (const auto& dir : m_loadedFontAtlas)
		{
			ASTRA_CORE_ASSERT(dir.second.use_count() == 1, "Resource: Dangling FontAtlas Pointer at Application closure.");
		}
	#endif
		
		// Clean Up All Remaining References
		m_loadedMeshes.clear(); 
		m_loadedTextures.clear();
		m_loadedCubeTextures.clear();
		m_loadedFontAtlas.clear();
	}

	void Resource::CheckImpl()
	{
		// Loop through and determine if any models can be unloaded and are extraneous
		for (auto iter = m_loadedMeshes.begin(); iter != m_loadedMeshes.end();)
		{
			if (iter->second.use_count() == 1) 
				iter = m_loadedMeshes.erase(iter);
			else 
				++iter;
		}
	}

	/*Asset<Texture> Resource::LoadTextureImpl(const TextureCreationSpec& specs)
	{
		// Check if filepath exists within loaded textures.
		size_t hash = std::hash<std::string>{}(specs.filepath);
		if (m_loadedTextures.find(hash) != m_loadedTextures.end())
		{
			return m_loadedTextures[hash];
		}

		Asset<Texture> texture = CreateAsset<Texture>(specs.filepath);

		stbi_set_flip_vertically_on_load(specs.flipped);
		int m_bpp;
		unsigned char* buffer;
		buffer = stbi_load(std::string(specs.filepath).c_str(), &texture->width, &texture->height, &m_bpp, 4);

		if (!buffer)
		{
			ASTRA_CORE_WARN("Texture {0} Did Not Load Correctly.", specs.filepath);
			return NULL;
		}
		auto hdr = Application::Get().GetWindow().IsHDR();

		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_2D, texture->id);
		glTexImage2D(GL_TEXTURE_2D, 0, hdr && specs.diffuse ? GL_SRGB8_ALPHA8 : GL_RGBA8, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		glGenerateMipmap(GL_TEXTURE_2D);
		texture->hdr = hdr;
		if (specs.diffuse)
			texture->type = TextureType::DiffuseMap;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, specs.diffuse ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(buffer);

		return m_loadedTextures[hash] = texture;
	}*/

	Asset<Texture> Resource::LoadTextureImpl(const TextureCreationSpec& specs)
	{
		// Check if filepath exists within loaded textures.
		size_t hash = std::hash<std::string>{}(specs.filepath);
		if (m_loadedTextures.find(hash) != m_loadedTextures.end())
		{
			return m_loadedTextures[hash];
		}

		Asset<Texture> texture = CreateAsset<Texture>(specs.filepath);
		stbi_set_flip_vertically_on_load(specs.flipped);

		unsigned char* data;
		int nrComponents;

		if (specs.scene)
		{
			const auto* scene = reinterpret_cast<const aiScene*>(specs.scene);
			if (const auto* tex = scene->GetEmbeddedTexture(specs.filepath.c_str()))
			{
				// Read Texture from Memory - Embedded Texture
				data = !tex->mHeight ? stbi_load_from_memory(reinterpret_cast<unsigned char*>(tex->pcData), tex->mWidth, &texture->width, &texture->height, &nrComponents, 0)
										: stbi_load_from_memory(reinterpret_cast<unsigned char*>(tex->pcData), tex->mWidth * tex->mHeight, &texture->width, &texture->height, &nrComponents, 0);
			}
			else
			{
				// Read Texture from Filepath - External Texture
				auto filename = specs.directory + '/' + std::string(specs.filepath);
				data = stbi_load(filename.c_str(), &texture->width, &texture->height, &nrComponents, 0);
			}
		}
		else
		{
			data = stbi_load(std::string(specs.filepath).c_str(), &texture->width, &texture->height, &nrComponents, 0);
		}
		ASTRA_CORE_ASSERT(data, "Model: Texture Data Failed to Load.");

		glGenTextures(1, &texture->id);
		auto hdr = Application::Get().GetWindow().IsHDR();

		GLenum format = nrComponents == 1 ? GL_RED : nrComponents == 3 ? GL_RGB : GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture->id);
		glTexImage2D(GL_TEXTURE_2D, 0, hdr && specs.diffuse ? GL_SRGB8_ALPHA8 : GL_RGBA8, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (specs.diffuse)
			texture->type = TextureType::DiffuseMap;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, specs.diffuse ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		if (glfwExtensionSupported("GL_EXT_texture_filter_anisotropic"))
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
			float maxValue;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxValue);
			float amount = fminf(4.0f, maxValue);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);
		}
		else
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.2f);
			ASTRA_CORE_WARN("Anisotropic Filtering Not Supported");
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);

		return m_loadedTextures[hash] = texture;
	}

	Asset<CubeMapTexture> Resource::LoadCubeMapImpl(const std::vector<const char*>& filepaths)
	{
		// Check if filepath exists within loaded textures.
		std::string combinded;
		for (const auto* file : filepaths)
		{
			combinded += file;
		}
		size_t hash = std::hash<std::string>{}(combinded);

		if (m_loadedCubeTextures.find(hash) != m_loadedCubeTextures.end())
		{
			return m_loadedCubeTextures[hash];
		}

		Asset<CubeMapTexture> texture = CreateAsset<CubeMapTexture>(filepaths);

		glGenTextures(1, &texture->id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id);
		stbi_set_flip_vertically_on_load(false); // Don't flip?
		
		auto hdr = Application::Get().GetWindow().IsHDR();
		
		int m_bpp;
		unsigned char* buffer;
		int width, height;

		for (size_t i = 0; i < filepaths.size(); i++)
		{
			buffer = stbi_load(filepaths[i], &width, &height, &m_bpp, 4);
			if (!buffer)
			{
				ASTRA_CORE_WARN("Cube Map Texture {0} Did Not Load Correctly.", filepaths[i]);
				return NULL;
			}

			if (buffer)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, hdr ? GL_SRGB8_ALPHA8 : GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
				stbi_image_free(buffer);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return m_loadedCubeTextures[hash] = texture;
	}

	Asset<FontAtlas> Resource::LoadFontAtlasImpl(const char* const filepath, unsigned int fontsize)
	{
		size_t hash = std::hash<std::string>{}(filepath);
		hash += std::hash<unsigned int>{}(fontsize);

		if (m_loadedFontAtlas.find(hash) != m_loadedFontAtlas.end())
		{
			return m_loadedFontAtlas[hash];
		}

		return m_loadedFontAtlas[hash] = CreateAsset<FontAtlas>(filepath, fontsize);
	}

	void Resource::UpdateDiffuseTexturesImpl(bool hdr)
	{
		for (auto iter = m_loadedTextures.begin(); iter != m_loadedTextures.end(); ++iter)
		{
			if (iter->second->type != TextureType::DiffuseMap) // Not diffuse so don't consider
				continue;

			if (iter->second->hdr == hdr) // Already Updated
				continue;

			int width, height;
			glBindTexture(GL_TEXTURE_2D, iter->second->id);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

			void* data = malloc(sizeof(float) * width * height * 4 /*RGBA*/); // Allocate Enough Space For Image

			// Gathers Image Data from Texture Buffer based on ID and re-buffers with new internal format
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glTexImage2D(GL_TEXTURE_2D, 0, hdr ? GL_SRGB8_ALPHA8 : GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			iter->second->hdr = hdr; // Update hdr status

			free(data); // Free Image Data
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		for (auto iter = m_loadedCubeTextures.begin(); iter != m_loadedCubeTextures.end(); ++iter)
		{
			if (iter->second->hdr == hdr) // Already Updated
				continue;

			int width, height;
			glBindTexture(GL_TEXTURE_CUBE_MAP, iter->second->id);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
		
			stbi_set_flip_vertically_on_load(false); // Don't flip?

			int m_bpp, i = 0;
			unsigned char* buffer;
			for (const auto& file : iter->second->GetFiles())
			{
				buffer = stbi_load(file, &width, &height, &m_bpp, 4);
				if (!buffer)
				{
					ASTRA_CORE_WARN("Cube Map Texture {0} Did Not Load Correctly.", file);
					return;
				}

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, hdr ? GL_SRGB8_ALPHA8 : GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
				stbi_image_free(buffer);
				++i;
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			iter->second->hdr = hdr; // Update hdr status

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	}

	std::shared_ptr<Mesh> Resource::LoadMeshImpl(const MeshCreationSpec& specs)
	{
		// Check if filepath exists within loaded textures.
		size_t hash = std::hash<std::string>{}(specs.filepath);
		if (m_loadedMeshes.find(hash) != m_loadedMeshes.end())
		{
			auto result = m_loadedMeshes[hash];
			//++m_tracker[result];
			return result;
		}
		
		auto* mesh = reinterpret_cast<aiMesh*>(specs.mesh);
		const auto* scene = reinterpret_cast<const aiScene*>(specs.scene);
		
		std::vector<int> indices;
		if (specs.normalMapped)
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
				ExtractBoneWeightForVertices(vertices, mesh, scene, specs.map, specs.counter);
			}
			return m_loadedMeshes[hash] = std::make_shared<Mesh>(vertices, indices);
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
				ExtractBoneWeightForVertices(vertices, mesh, scene, specs.map, specs.counter);
			}
			return m_loadedMeshes[hash] = CreateAsset<Mesh>(vertices, indices);
		}
	}

	Asset<Mesh> Resource::LoadMeshImpl(const std::string& string, const std::vector<Vertex>& vertices, const std::vector<int>& indices)
	{
		// Check if filepath exists within loaded textures.
		size_t hash = std::hash<std::string>{}(string);
		if (m_loadedMeshes.find(hash) != m_loadedMeshes.end())
		{
			return m_loadedMeshes[hash];
		}

		return m_loadedMeshes[hash] = CreateAsset<Mesh>(vertices, indices);
	}

	template<class Vertex>
	void Resource::SetVertexBoneData(Vertex& vertex, int id, float weight)
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
	void Resource::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, void* _mesh, const void* _scene,
												std::map<std::string, BoneInfo>* map, int* counter)
	{
		auto* mesh = reinterpret_cast<aiMesh*>(_mesh);
		const auto* scene = reinterpret_cast<const aiScene*>(_scene);

		for (unsigned int index = 0; index < mesh->mNumBones; ++index)
		{
			int id = -1;
			std::string name = mesh->mBones[index]->mName.C_Str();
			if (map->find(name) == map->end())
			{
				BoneInfo info(*counter, ConvertAiMatrix(mesh->mBones[index]->mOffsetMatrix));
				(*map)[name] = info;
				id = (*counter)++;
			}
			else
			{
				id = (*map)[name].Id;
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
}