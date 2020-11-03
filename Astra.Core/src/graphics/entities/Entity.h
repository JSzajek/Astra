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
		const ImageMaterial* const material;
		const Texture const normalMap;
		const Texture const parallaxMap;
	private:
		int m_textureIndex;
		bool m_normalMapped;
		bool m_parallaxMapped;
		float m_height;
	public:
		Entity();
		~Entity();
		Entity(const Math::Vec3& position);
		Entity(const VertexArray* vertArray, const Math::Vec3& position);
		Entity(const VertexArray* vertArray, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale);
		Entity(const VertexArray* vertArray, const ImageMaterial* material = new ImageMaterial(), 
				const Math::Vec3& position = Math::Vec3(0), const Math::Vec3& rotation = Math::Vec3(0), const Math::Vec3& scale = Math::Vec3(1));

		Entity(const VertexArray* vertArray, const ImageMaterial* material = new ImageMaterial(), int textureIndex = 0, 
				const Math::Vec3& position = Math::Vec3(0), const Math::Vec3& rotation = Math::Vec3(0), const Math::Vec3& scale = Math::Vec3(1));
		
		Entity(const char* const filepath, const char* const normalMapTexture, const ImageMaterial* material = new ImageMaterial(),
				const Math::Vec3& position = Math::Vec3(0), const Math::Vec3& rotation = Math::Vec3(0), const Math::Vec3& scale = Math::Vec3(1));

		Entity(const char* const filepath, const char* const normalMapTexture, const char* const heightMapTexture, float height,
				const ImageMaterial* material = new ImageMaterial(), const Math::Vec3& position = Math::Vec3(0),
				const Math::Vec3& rotation = Math::Vec3(0), const Math::Vec3& scale = Math::Vec3(1));

		Entity(const Entity& other);

		inline const bool IsNormalMapped() const { return m_normalMapped; }
		inline const bool IsParallaxMapped() const { return m_parallaxMapped; }
		inline const float GetHeightOffset() const { return m_height; }

		inline float GetMaterialXOffset() const { return (float)(m_textureIndex % material->GetRowCount()) / (float)material->GetRowCount(); }
		inline float GetMaterialYOffset() const { return (float)(m_textureIndex / material->GetRowCount()) / (float)material->GetRowCount(); }
	};
}