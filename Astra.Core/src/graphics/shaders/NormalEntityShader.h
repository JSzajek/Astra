#pragma once

#include "Shader.h"

namespace Astra::Graphics
{
	class NormalEntityShader : public Shader
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

		static constexpr const char* ModelTexture = "u_Texture";
		static constexpr const char* NormalMapTag = "u_NormalMap";


	public:
		NormalEntityShader(const char* filepath =
			"../Astra.Core/src/resources/shaders/NormalEntity.shader")
			: Shader(filepath, ShaderType::NormalMapped)
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

		static const Math::Vec3& ConvertToEyeSpacePosition(const Math::Vec3& position, const Math::Mat4& viewMatrix)
		{
			Math::Vec4 eyeSpacePos = Math::Vec4(position.x, position.y, position.z, 1.0f);
			Math::Vec4 result = viewMatrix * eyeSpacePos;
			return Math::Vec3(result.x, result.y, result.z);
		}
	};
}