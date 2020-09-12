#pragma once

#include "Renderer.h"
#include "../entities/WaterTile.h"

namespace Astra::Graphics
{
	class WaterRenderer : public Renderer
	{
	private:
		const VertexArray* m_defaultQuad;
		std::vector<WaterTile> m_waterTiles;
	public:
		WaterRenderer(Shader* shader);
		void Draw(const Math::Mat4& viewMatrix) override;
		inline void AddTile(const WaterTile& tile) { m_waterTiles.push_back(tile); }
	};
}