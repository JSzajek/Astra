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
	enum ModelType : unsigned char
	{
		Default, NormalMapped, Selected, ShadowCaster
	};

	class Model : public Spatial
	{
	private:
		// In Game Properties
		size_t m_renderId;
		bool m_normals; // Contains a Normal Mapped Mesh
		Math::Mat4 selectedModelMatrix;
		unsigned int m_textureIndex;
		unsigned int m_rowCount;
		bool m_selected;
	private:
		// Base Information
		std::string m_directory;
		Mesh* m_mesh;
		ImageMaterial m_material;

		// Animation Information
		std::unordered_map<std::string, Animation> m_animations;
		Animator* m_animator;
		std::map<std::string, BoneInfo> m_boneInfoMap;
		int m_boneCounter;
	public:
		Model() = default;
		Model(const char* const name, const char* const filepath, bool calcTangents = false);
		Model(const char* const filepath, bool calcTangents = false);
		Model(const Model& other);
		void operator=(const Model& other);
		~Model();

		virtual void Free() override {};
		inline virtual std::string ToString() const override { return !Name.length() ? ("Model_&" + std::to_string(m_uid)) : Name; }
		
		inline size_t GetRenderID() const { return m_renderId; }

		inline bool HasNormals() const { return m_normals; }
		inline bool IsSelected() const { return m_selected; }

		inline Mesh* GetMesh() const { return m_mesh; }
		inline const ImageMaterial& GetMaterial() const { return m_material; }
		inline ImageMaterial& GetMaterial() { return m_material; }

		inline unsigned int GetTextureIndex() const { return m_textureIndex; }
		inline unsigned int GetRowCount() const { return m_rowCount; }

		inline void SetSelected(bool enabled) { m_selected = enabled; UpdateMatrices(); }
		inline void SetTextureIndex(unsigned int index) { m_textureIndex = index; }
		inline void SetRowCount(unsigned int count) { m_rowCount = count; }

		inline void AddAnimator() { m_animator = new Animator(); }
		inline bool HasAnimator() const { return m_animator; }
		inline Animator* GetAnimator() const { return m_animator; }

		inline void PlayAnimation(const std::string& name) 
		{
			if (m_animations.find(name) == m_animations.end())
			{
				ASTRA_CORE_INFO("Model: Could Not Find Animation: {0}", name);
				return;
			}
			m_animator->SetAnimation(m_animations[name]);
			m_animator->Play();
		}

		inline float GetMaterialXOffset() const { return (float)(m_textureIndex % m_rowCount) / (float)m_rowCount; }
		inline float GetMaterialYOffset() const { return (float)(m_textureIndex / m_rowCount) / (float)m_rowCount; }
	
		inline const Math::Mat4& GetSelectedModelMatrix() const { return selectedModelMatrix; }
	private:
		// Base Methods
		void LoadModel(std::string filepath, bool calcTangents);
		void ProcessNode(aiNode* node, const aiScene* scene, const std::string& filepath);
		std::tuple<Mesh*, ImageMaterial> ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& filepath);
		ImageMaterial ProcessMaterials(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture*> LoadMaterialTextures(const aiScene* scene, aiMaterial* mat, aiTextureType type, TextureType texType);
	protected:
		void UpdateMatrices() override;
	};
}