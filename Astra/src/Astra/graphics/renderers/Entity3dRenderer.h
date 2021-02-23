#pragma once

#include <vector>
#include <stack>

#include "Renderer.h"

#include "Astra/graphics/Color.h"
#include "Astra/graphics/loaders/Loader.h"

#include "Astra/graphics/entities/Model.h"
#include "Astra/graphics/entities/Light.h"

namespace Astra::Graphics
{
	class Entity3dRenderer : public Renderer
	{
	private:
		const Math::Mat4* m_toShadowSpaceMatrix;
		const Color* m_fogColor;
		
	#if ASTRA_DEBUG
		bool m_wireframe;
	#endif
	public:
		Entity3dRenderer(const Color* fogColor);

	#if ASTRA_DEBUG
		void SetWireframe(bool wireframe) { m_wireframe = wireframe; };
	#endif
		void SetShader(Shader* shader) override;
		inline void SetShadowMatrix(const Math::Mat4* shadowMatrix) { m_toShadowSpaceMatrix = shadowMatrix; }

		void Draw(float delta, 
				  const std::unordered_map<unsigned int, std::vector<const Model*>>& models,
				  const Math::Mat4* viewMatrix = NULL, 
				  const Math::Vec4& inverseViewVector = NULL, 
				  const Math::Vec4& clipPlane = DefaultClipPlane);

		inline void AddLight(Light* light) { AddLight(0, light); }
		void AddLight(unsigned int index, Light* light);
	private:
		void PrepareMesh(const Mesh& mesh);
	};
}