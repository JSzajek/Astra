#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class LightingShader : public Shader
	{
	#define MAX_LIGHTS 4

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

		/* TODO: Add optimizations:
					- Only update light positions and colors when they change or when necessary
					- Sort lights based on position to camera and only grab top 4
		*/
		static const char* GetLightPositionTag(int index)
		{
			static std::string base("lightPosition[x]");
			return base.replace(base.end() - 2, base.end() - 1, std::to_string(index)).c_str();
		}

		static const char* GetLightColorTag(int index)
		{
			static std::string base("lightColor[x]");
			return base.replace(base.end() - 2, base.end() - 1, std::to_string(index)).c_str();
		}

		static const char* GetAttenuationTag(int index)
		{
			static std::string base("attenuation[x]");
			return base.replace(base.end() - 2, base.end() - 1, std::to_string(index)).c_str();
		}
	};
}