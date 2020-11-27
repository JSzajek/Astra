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
		const Texture* normalMap;
		const Texture* parallaxMap;
		Math::Mat4* selectedModelMatrix;
	private:
		int m_textureIndex;
		bool m_normalMapped;
		bool m_parallaxMapped;
		float m_height;
		bool m_selected;
	public:
		Entity();
		~Entity();
		Entity(const Math::Vec3& position);
		
		Entity(const char* const objpath, const ImageMaterial* material = new ImageMaterial(), int textureIndex = 0,
				const Math::Vec3& position = Math::Vec3::Zero, const Math::Vec3& rotation = Math::Vec3::Zero, const Math::Vec3& scale = Math::Vec3::One);

		Entity(const char* const filepath, const char* const normalMapTexture, const ImageMaterial* material = new ImageMaterial(),
				const Math::Vec3& position = Math::Vec3(0), const Math::Vec3& rotation = Math::Vec3(0), const Math::Vec3& scale = Math::Vec3(1));

		Entity(const char* const filepath, const char* const normalMapTexture, const char* const heightMapTexture, float height,
				const ImageMaterial* material = new ImageMaterial(), const Math::Vec3& position = Math::Vec3(0),
				const Math::Vec3& rotation = Math::Vec3(0), const Math::Vec3& scale = Math::Vec3(1));

		Entity(const Entity& other);

		inline void SetSelected(bool selected) { m_selected = selected; UpdateMatrices();  }
		inline bool IsSelected() const { return m_selected; }

		inline const bool IsNormalMapped() const { return m_normalMapped; }
		inline const bool IsParallaxMapped() const { return m_parallaxMapped; }
		inline const float GetHeightOffset() const { return m_height; }

		inline const Math::Mat4* const GetSelectedModelMatrix() const { return selectedModelMatrix; }

		inline float GetMaterialXOffset() const { return (float)(m_textureIndex % material->GetRowCount()) / (float)material->GetRowCount(); }
		inline float GetMaterialYOffset() const { return (float)(m_textureIndex / material->GetRowCount()) / (float)material->GetRowCount(); }
	protected:
		void UpdateMatrices() override;
	};
}