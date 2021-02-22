#pragma once

#include <string>
#include <vector>
#include <map>

#include <assimp/scene.h>

#include "Astra/graphics/entities/Spatial.h"
#include "Astra/graphics/entities/utility/Mesh.h"
#include "Astra/graphics/entities/utility/BoneInfo.h"

#include "Astra/graphics/entities/utility/Animation.h"
#include "Astra/graphics/entities/utility/Animator.h"

namespace Astra::Graphics
{
	class Model : public Spatial
	{
	private:
		// In Game Properties
		size_t m_uid;
		bool m_normals; // Contains a Normal Mapped Mesh
		Math::Mat4* selectedModelMatrix;
		unsigned int m_textureIndex;
		unsigned int m_rowCount;
		bool m_selected;
	private:
		// Base Information
		std::string m_directory;
		std::vector<Mesh> m_meshes;
		
		// Animation Information
		std::vector<Animation> m_animations;
		Animator* m_animator;
		Math::Mat4 m_globalInverseTransform;
		std::map<std::string, BoneInfo> m_boneInfoMap;
		int m_boneCounter;
	public:
		Model(const char* const filepath, bool calcTangents);
		Model(const Model& other);
		~Model();

		inline bool HasNormals() const { return m_normals; }
		inline bool IsSelected() const { return m_selected; }

		inline const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
		inline Mesh& GetMesh(size_t index) { return m_meshes[index]; }
		inline size_t GetMeshesCount() const { return m_meshes.size(); }

		inline unsigned int GetTextureIndex() const { return m_textureIndex; }
		inline unsigned int GetRowCount() const { return m_rowCount; }
		inline size_t GetUID() const { return m_uid; }

		inline void SetSelected(bool enabled) { m_selected = enabled; }
		inline void SetTextureIndex(unsigned int index) { m_textureIndex = index; }
		inline void SetRowCount(unsigned int count) { m_rowCount = count; }

		inline void SetAnimator(Animator* animator) 
		{ 
			m_animator = animator; 
			m_animator->SetGlobalInverseTransform(m_globalInverseTransform);
		}
		inline bool HasAnimator() const { return m_animator; }
		inline Animator* GetAnimator() const { return m_animator; }

		inline void PlayAnimation(const std::string& name) 
		{
			m_animator->SetAnimation(&m_animations[0]);
			m_animator->Play();
		}

		inline float GetMaterialXOffset() const { return (float)(m_textureIndex % m_rowCount) / (float)m_rowCount; }
		inline float GetMaterialYOffset() const { return (float)(m_textureIndex / m_rowCount) / (float)m_rowCount; }
	
		inline const Math::Mat4* const GetSelectedModelMatrix() const { return selectedModelMatrix; }
	private:
		// Base Methods
		void LoadModel(std::string filepath, bool calcTangents);
		void ProcessNode(aiNode* node, const aiScene* scene);
		ImageMaterial* ProcessMaterials(aiMesh* mesh, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::tuple<std::vector<Texture*>, size_t> LoadMaterialTextures(const aiScene* scene, aiMaterial* mat, aiTextureType type, TextureType texType);

		// Animation Methods
		template<class Vertex>
		void SetVertexBoneData(Vertex& vertex, int id, float weight);

		template<class Vertex>
		void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
	protected:
		void UpdateMatrices() override;
	};
}