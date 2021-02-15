#pragma once

#include "Spatial.h"
#include "../materials/ImageMaterial.h"
#include "../buffers/VertexArray.h"

namespace Astra::Graphics
{
	class Entity : public Spatial
	{
	public:
		const VertexArray* vertexArray;
		const ImageMaterial* material;
		Math::Mat4* selectedModelMatrix;
	private:
		int m_textureIndex;
		bool m_selected;
	public:
		Entity();
		
		Entity(const char* const filepath,
			   bool calcTangents, 
			   int textureIndex,
			   const Math::Vec3& position,
			   const Math::Vec3& rotation,
			   const Math::Vec3& scale);

		Entity(const Entity& other);
		~Entity();

		inline void SetSelected(bool selected) { m_selected = selected; UpdateMatrices();  }
		inline bool IsSelected() const { return m_selected; }

		inline void SetMaterial(const ImageMaterial* material) { this->material = material; }

		inline const Math::Mat4* const GetSelectedModelMatrix() const { return selectedModelMatrix; }

		inline float GetMaterialXOffset() const { return (float)(m_textureIndex % material->GetRowCount()) / (float)material->GetRowCount(); }
		inline float GetMaterialYOffset() const { return (float)(m_textureIndex / material->GetRowCount()) / (float)material->GetRowCount(); }
	protected:
		void UpdateMatrices() override;
	};
}