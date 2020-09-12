#pragma once

#include <vector>

#include "Renderer.h"
#include "../buffers/CubeMapTexture.h"
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
		std::vector<const char*> m_textureFiles =
		{
			"../Astra.Core/src/resources/textures/Default_Skybox/right.png",
			"../Astra.Core/src/resources/textures/Default_Skybox/left.png",
			"../Astra.Core/src/resources/textures/Default_Skybox/top.png",
			"../Astra.Core/src/resources/textures/Default_Skybox/bottom.png",
			"../Astra.Core/src/resources/textures/Default_Skybox/back.png",
			"../Astra.Core/src/resources/textures/Default_Skybox/front.png",
		};

		std::vector<const char*> m_nightTextureFiles =
		{
			"../Astra.Core/src/resources/textures/Default_Night_Skybox/right.png",
			"../Astra.Core/src/resources/textures/Default_Night_Skybox/left.png",
			"../Astra.Core/src/resources/textures/Default_Night_Skybox/top.png",
			"../Astra.Core/src/resources/textures/Default_Night_Skybox/bottom.png",
			"../Astra.Core/src/resources/textures/Default_Night_Skybox/back.png",
			"../Astra.Core/src/resources/textures/Default_Night_Skybox/front.png",
		};

		const VertexArray* m_cube;
		const CubeMapTexture* m_texture;
		const CubeMapTexture* m_nightTexture;
		const Math::Vec3* m_skyColor;
		float m_blendFactor;
	public:
		SkyboxRenderer(Shader* shader, const Math::Vec3* skyColor);

		void Draw(const Math::Mat4& viewMatrix) override;
	private:
		void BindTextures();
	};
}
