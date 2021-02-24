#pragma once

#include <map>
#include <unordered_map>

#include "Astra/Res.h"
#include "Astra/graphics/buffers/Texture.h"
#include "Astra/graphics/buffers/CubeMapTexture.h"
#include "Astra/graphics/entities/utility/Mesh.h"
#include "Astra/graphics/entities/utility/BoneInfo.h"
//#include "Astra/graphics/buffers/FrameBuffer.h" - later

namespace Astra::Graphics
{
	#define UNLOAD(res) { if (res) { Resource::Unload(res); res = NULL; } }
	#define TRACK(res) { if (res) { Resource::Remark(res); } }

	enum ResourceType
	{
		TextureResource,
		CubeTextureResource,
		MeshResource
	};

	struct ResourceTrack
	{
		unsigned short count;
		unsigned short source;
		size_t hash;

		ResourceTrack()
			: count(0), source(0), hash(0)
		{
		}

		ResourceTrack(size_t hash, unsigned short source)
			: count(1), source(source), hash(hash)
		{
		}

		inline ResourceTrack& operator++() { count++; return *this; }
		inline ResourceTrack& operator--() { count--; return *this; }
		operator int() const { return count; }
	};

	class Resource
	{
	private:
		std::unordered_map<size_t, Texture> m_loadedTextures;
		std::unordered_map<size_t, CubeMapTexture> m_loadedCubeTextures;
		std::unordered_map<size_t, Mesh> m_loadedMeshes;
		// track pointer to amount in use, original listing source, and original hash
		std::map<Res*, ResourceTrack> m_tracker;
	public:
		Resource(const Resource&) = delete;
		void operator=(const Resource&) = delete;

		static Resource& Get()
		{
			static Resource instance;
			return instance;
		}

		static void Unload(Res* ptr)
		{
			Get().UnloadImpl(ptr);
		}

		static void Remark(Res* ptr)
		{
			Get().RemarkImpl(ptr);
		}

		static Texture* LoadTexture(const char* const filepath, bool diffuse = false)
		{
			return Get().LoadTextureImpl(filepath, diffuse);
		}

		static Texture* LoadTexture(std::string filepath, std::string directory, const void* scene, bool diffuse = false)
		{
			return Get().LoadTextureImpl(filepath, directory, scene, diffuse);
		}

		static CubeMapTexture* LoadCubeMap(const std::vector<const char*>& filepaths)
		{
			return Get().LoadCubeMapImpl(filepaths);
		}

		static Mesh* LoadMesh(const std::string& filepath, void* mesh, const void* scene,
							  std::map<std::string, BoneInfo>& map, int& counter, bool normalMapped)
		{
			return Get().LoadMeshImpl(filepath, mesh, scene, map, counter, normalMapped);
		}

		static Mesh* LoadMesh(const char* const name, const std::vector<Vertex>& vertices, const std::vector<int>& indices)
		{
			return Get().LoadMeshImpl(name, vertices, indices);
		}
	private:
		Resource();
		~Resource();
	private:
		void UnloadImpl(Res* ptr);
		void RemarkImpl(Res* ptr);
		Texture* LoadTextureImpl(const char* const filepath, bool diffuse);
		Texture* LoadTextureImpl(std::string filepath, std::string directory, const void* scene, bool diffuse = false);
		
		CubeMapTexture* LoadCubeMapImpl(const std::vector<const char*>& filepaths);

		Mesh* LoadMeshImpl(const std::string& filepath, void* mesh, const void* scene,
						   std::map<std::string, BoneInfo>& map, int& counter, bool normalMapped);
		Mesh* LoadMeshImpl(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<int>& indices);
	private:
		
		// Animation Methods
		template<class Vertex>
		void SetVertexBoneData(Vertex& vertex, int id, float weight);

		template<class Vertex>
		void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, void* mesh, const void* scene,
										  std::map<std::string, BoneInfo>& map, int& counter);
	};
}