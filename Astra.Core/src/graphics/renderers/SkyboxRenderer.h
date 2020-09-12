#pragma once

#include <vector>

#include "Renderer.h"
#include "../buffers/CubeMapTexture.h"
#include "../materials/SkyboxMaterial.h"
#include "../loaders/Loader.h"
#include "../shaders/SkyboxShader.h"

namespace Astra::Graphics
{
	class SkyboxRenderer : public Renderer
	{
	private:
		static const constexpr float Size = 500.0f;
		static const inline std::vector<float> Vertices =
		{
			-Size,  Size, -Size,
			-Size, -Size, -Size,
			 Size, -Size, -Size,
			 Size, -Size, -Size,
			 Size,  Size, -Size,
			-Size,  Size, -Size,

			-Size, -Size,  Size,
			-Size, -Size, -Size,
			-Size,  Size, -Size,
			-Size,  Size, -Size,
			-Size,  Size,  Size,
			-Size, -Size,  Size,

			 Size, -Size, -Size,
			 Size, -Size,  Size,
			 Size,  Size,  Size,
			 Size,  Size,  Size,
			 Size,  Size, -Size,
			 Size, -Size, -Size,

			-Size, -Size,  Size,
			-Size,  Size,  Size,
			 Size,  Size,  Size,
			 Size,  Size,  Size,
			 Size, -Size,  Size,
			-Size, -Size,  Size,

			-Size,  Size, -Size,
			 Size,  Size, -Size,
			 Size,  Size,  Size,
			 Size,  Size,  Size,
			-Size,  Size,  Size,
			-Size,  Size, -Size,

			-Size, -Size, -Size,
			-Size, -Size,  Size,
			 Size, -Size, -Size,
			 Size, -Size, -Size,
			-Size, -Size,  Size,
			 Size, -Size,  Size
		};
	private:
		const VertexArray* m_cube;
		const SkyboxMaterial* m_material;
		const Math::Vec3* m_skyColor;
		float m_blendFactor;
	public:
		SkyboxRenderer(Shader* shader, const Math::Vec3* skyColor);

		inline void SetSkyBox(const SkyboxMaterial* material) { m_material = material; }

		void Draw(const Math::Mat4& viewMatrix) override;
	private:
		void BindTextures();
	};
}
