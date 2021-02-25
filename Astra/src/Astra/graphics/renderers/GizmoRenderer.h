#pragma once

#include <unordered_map>

#include "Renderer.h"

#include "Astra/graphics/gizmos/Gizmo.h"
#include "Astra/graphics/shaders/GizmoShader.h"
#include "Astra/graphics/entities/utility/Mesh.h"

namespace Astra::Graphics
{
#if ASTRA_DEBUG
	class GizmoRenderer : public Renderer
	{
	private:
		Asset<Mesh> m_defaultQuad;
		const Math::Mat4* m_viewMatrix;
		Math::Mat4* m_modelViewMatrix;
	public:
		GizmoRenderer(GizmoShader* shader);
		~GizmoRenderer();

		void Draw(const std::unordered_map<unsigned int, std::vector<const Graphics::Gizmo*>>& gizmos, 
				  const Math::Mat4* viewMatrix = NULL);
	private:
		void UpdateModelViewMatrix(const Math::Vec3& position, float scale);
	};
#endif
}