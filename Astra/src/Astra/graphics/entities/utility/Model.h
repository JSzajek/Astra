#pragma once

#include <string>
#include <vector>

#include <assimp/scene.h>

#include "Astra/graphics/entities/Spatial.h"
#include "Astra/graphics/entities/utility/Mesh.h"

namespace Astra::Graphics
{
	class Model : public Spatial
	{
	private:
		size_t m_uid;
		bool m_normals;
		Math::Mat4* selectedModelMatrix;
	private:
		std::vector<Mesh> m_meshes;
		unsigned int m_textureIndex;
		unsigned int m_rowCount;
		bool m_selected;
	private:
		std::string m_directory;
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

		inline float GetMaterialXOffset() const { return (float)(m_textureIndex % m_rowCount) / (float)m_rowCount; }
		inline float GetMaterialYOffset() const { return (float)(m_textureIndex / m_rowCount) / (float)m_rowCount; }
	
		inline const Math::Mat4* const GetSelectedModelMatrix() const { return selectedModelMatrix; }
	private:
		void LoadModel(std::string filepath, bool calcTangents);
		void ProcessNode(aiNode* node, const aiScene* scene);
		ImageMaterial* ProcessMaterials(aiMesh* mesh, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::tuple<std::vector<Texture*>, size_t> LoadMaterialTextures(const aiScene* scene, aiMaterial* mat, aiTextureType type, TextureType texType);
	protected:
		void UpdateMatrices() override;
	};
}