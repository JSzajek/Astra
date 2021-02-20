#pragma once

#include "Renderer.h"

#include "Astra/graphics/Color.h"
#include "Astra/graphics/loaders/Loader.h"

#include "Astra/graphics/entities/utility/Model.h"
//#include "Astra/graphics/entities/Entity.h" 

namespace Astra::Graphics
{
	class SelectionRenderer : public Renderer
	{
	private:
		Shader* m_entityShader;
		static const Color DefaultColor;
	public:
		SelectionRenderer();
		void SetSelectionColor(const Color& color);
		
		void SetShader(Shader* shader) override;
		void UpdateProjectionMatrix(const Math::Mat4* projectionMatrix) override;

		void Draw(float delta,
				  const std::unordered_map<unsigned int, std::vector<const Model*>>& models,
				  const Math::Mat4* viewMatrix = NULL,
				  const Math::Vec4& inverseViewVector = NULL,
				  const Math::Vec4& clipPlane = DefaultClipPlane);
		void DrawSelected(const std::unordered_map<unsigned int, std::vector<const Model*>>& models, 
						  const Math::Mat4* viewMatrix);
	private:
		void PrepareMesh(const Mesh& mesh);
	};
}
