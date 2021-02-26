#pragma once

#include <map>
#include <unordered_map>

#include "Astra/Core.h"
#include "Astra/graphics/buffers/Texture.h"
#include "Astra/graphics/buffers/CubeMapTexture.h"
#include "Astra/graphics/guis/utility/FontAtlas.h"
#include "Astra/graphics/buffers/Mesh.h"
#include "Astra/graphics/entities/animation/BoneInfo.h"
#include "Astra/graphics/entities/animation/Animation.h"

namespace Astra::Graphics
{
	class Resource
	{
	private:
		std::unordered_map<size_t, Asset<Texture>> m_loadedTextures;
		std::unordered_map<size_t, Asset<CubeMapTexture>> m_loadedCubeTextures;
		std::unordered_map<size_t, Asset<FontAtlas>> m_loadedFontAtlas;

		std::unordered_map<size_t, Asset<Mesh>> m_loadedMeshes;
		std::unordered_map<size_t, Asset<Animation>> m_loadedAnimations;
	public:
		Resource(const Resource&) = delete;
		void operator=(const Resource&) = delete;

		static Resource& Get()
		{
			static Resource instance;
			return instance;
		}

		static void Check()
		{
			Get().CheckImpl();
		}

		static Asset<Texture> LoadTexture(const TextureCreationSpec& specs)
		{
			return Get().LoadTextureImpl(specs);
		}

		static Asset<CubeMapTexture> LoadCubeMap(const std::vector<const char*>& filepaths)
		{
			return Get().LoadCubeMapImpl(filepaths);
		}

		static Asset<FontAtlas> LoadFontAtlas(const char* const filepath, unsigned int fontsize)
		{
			return Get().LoadFontAtlasImpl(filepath, fontsize);
		}

		static Asset<Mesh> LoadMesh(const MeshCreationSpec& specs)
		{
			return Get().LoadMeshImpl(specs);
		}

		static Asset<Mesh> LoadMesh(const char* const name, const std::vector<Vertex>& vertices, const std::vector<int>& indices)
		{
			return Get().LoadMeshImpl(name, vertices, indices);
		}

		static Asset<Animation> LoadAnimation(const AnimationCreationSpec& specs)
		{
			return Get().LoadAnimationImpl(specs);
		}

		static void UpdateDiffuseTextures(bool hdr)
		{
			Get().UpdateDiffuseTexturesImpl(hdr);
		}
	private:
		Resource() = default;
		~Resource();
	private:
		void CheckImpl();
		Asset<Texture> LoadTextureImpl(const TextureCreationSpec& specs);

		Asset<CubeMapTexture> LoadCubeMapImpl(const std::vector<const char*>& filepaths);
		
		Asset<FontAtlas> LoadFontAtlasImpl(const char* const filepath, unsigned int fontsize);

		Asset<Mesh> LoadMeshImpl(const MeshCreationSpec& specs);
		Asset<Mesh> LoadMeshImpl(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<int>& indices);

		Asset<Animation> LoadAnimationImpl(const AnimationCreationSpec& specs);

		void UpdateDiffuseTexturesImpl(bool hdr);
	private:
		// Bone Extraction Methods
		template<class Vertex>
		void SetVertexBoneData(Vertex& vertex, int id, float weight);

		template<class Vertex>
		void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, void* mesh, const void* scene,
										  std::map<std::string, BoneInfo>* map, int* counter);
	};
}