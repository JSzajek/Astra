#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class WaterShader : public Shader
	{
	public:
		static constexpr const char* ReflectionTextureTag = "reflectionTexture";
		static constexpr const char* RefractionTextureTag = "refractionTexture";
		static constexpr const char* DuDvMapTextureTag = "dudvMap";
		static constexpr const char* NormalMapTextureTag = "normalMap";
		static constexpr const char* DepthMapTextureTag = "depthMap";

		static constexpr const char* LightPositionTag = "lightPosition";
		static constexpr const char* LightColorTag = "lightColor";

		static constexpr const char* MoveFactorTag = "moveFactor";

		static constexpr const char* CameraPositionTag = "cameraPosition";
	public:
		WaterShader(const char* filepath =
			"C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/shaders/Water.shader")
			: Shader(filepath, ShaderType::Water)
		{
		}
	};
}