#pragma once

#include <vector>

#include "Renderer.h"
#include "Astra/graphics/Color.h"
#include "Astra/graphics/buffers/CubeMapTexture.h"
#include "Astra/graphics/materials/SkyboxMaterial.h"
#include "Astra/graphics/shaders/SkyboxShader.h"
#include "Astra/graphics/entities/utility/Mesh.h"

namespace Astra::Graphics
{
	#define RotationSpeed			1.0f

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
		Asset<Mesh> m_cube;
		SkyboxMaterial m_material;
		bool m_set;
		const Color* m_fogColor;
		Math::Mat4* m_fixedViewMatrix;
		float m_blendFactor;
		float m_rotation;
	public:
		SkyboxRenderer(Shader* shader, const Color* fogColor);

		void SetSkyBox(const SkyboxMaterial& material);

		void Draw(float delta = 0, const Math::Mat4* viewMatrix = NULL, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane);
	private:
		void BindTextures();
	};
}
