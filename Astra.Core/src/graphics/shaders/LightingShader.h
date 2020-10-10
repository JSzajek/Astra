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

		static constexpr const char* SkyColorTag = "fogColor";

		static constexpr const char* UseFakeLightingTag = "useFakeLighting";

		static constexpr const char* NumberOfRowsTag = "numberOfRows";
		static constexpr const char* OffsetTag = "offset";

		static constexpr const char* ClipPaneTag = "clipPlane";
	public:
		LightingShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/Lighting.shader")
			: Shader(filepath, ShaderType::Lighting)
		{
		}

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