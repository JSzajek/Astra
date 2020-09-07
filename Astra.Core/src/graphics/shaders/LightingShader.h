#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class LightingShader : public Shader
	{
	public:
		static constexpr const char* LightPositionTag = "lightPosition";
		static constexpr const char* LightColorTag = "lightColor";
		static constexpr const char* ShineDampenerTag = "shineDampener";
		static constexpr const char* ReflectivityTag = "reflectivity";

		static constexpr const char* SkyColorTag = "skyColor";

		static constexpr const char* UseFakeLightingTag = "useFakeLighting";

		static constexpr const char* NumberOfRowsTag = "numberOfRows";
		static constexpr const char* OffsetTag = "offset";
	public:
		LightingShader(const char* filepath =
			"C:/Users/Justin/Documents/Cpp Projects/Astra/Astra.Core/src/resources/shaders/Lighting.shader")
			: Shader(filepath, ShaderType::Lighting)
		{
		}
	};
}