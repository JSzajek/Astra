#pragma once

#include "Spatial.h"
#include "../materials/ImageMaterial.h"
#include "../buffers/VertexArray.h"

namespace Astra::Graphics
{
	class Entity : public Spatial
	{
	public:
		const VertexArray* const vertexArray;
		const ImageMaterial* const material;
	public:
		Entity();
		~Entity();
		Entity(const Math::Vec3& position);
		Entity(const VertexArray* vertArray, const Math::Vec3& position);
		Entity(const VertexArray* vertArray, const Math::Vec3& position, const Math::Vec3& rotation, const Math::Vec3& scale);
		Entity(const VertexArray* vertArray, const ImageMaterial* material = new ImageMaterial(), const Math::Vec3& position = Math::Vec3(0), const Math::Vec3& rotation = Math::Vec3(0), const Math::Vec3& scale = Math::Vec3(1));
		
		Entity(const Entity& other);
		//Entity(const Math::Vec3& position, float xRotation, float yRotation, float zRotation, float scale);
	};
}