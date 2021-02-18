#pragma once

#include <vector>

#include "Renderer.h"
#include "Astra/graphics/Color.h"
#include "../buffers/CubeMapTexture.h"
#include "../materials/SkyboxMaterial.h"
#include "../loaders/Loader.h"
#include "../shaders/SkyboxShader.h"

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
		const VertexArray* m_cube;
		const SkyboxMaterial* m_material;
		const Color* m_fogColor;
		Math::Mat4* m_fixedViewMatrix;
		float m_blendFactor;
		float m_rotation;
	public:
		SkyboxRenderer(Shader* shader, const Color* fogColor);
		~SkyboxRenderer();

		//inline void Clear() override { m_material = NULL; }
		inline void SetSkyBox(const SkyboxMaterial* material) { m_material = material; }

		void Draw(float delta = 0, const Math::Mat4* viewMatrix = NULL, const Math::Vec4& inverseViewVector = NULL, const Math::Vec4& clipPlane = DefaultClipPlane);
	private:
		void BindTextures();
	};
}
