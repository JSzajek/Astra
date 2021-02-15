workspace "Astra"
	architecture "x86"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	startproject "TestGame"

outputdir = "%{cfg.system}-%{cfg.buildcfg}-%{cfg.architecture}"

project "Astra"
	location "Astra"
	kind "StaticLib"

	language "C++"
	cppdialect "C++17"
	
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "astra_pch.h"
	pchsource "%{prj.name}/src/astra_pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**",
		"%{prj.name}/vendor/dr_lib/**",
		"%{prj.name}/res/**"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLEW/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/OpenALSoft/include",
		"%{prj.name}/vendor/FreeType/include",
		
		-- Header Only
		"%{prj.name}/vendor/stb_image",
		"%{prj.name}/vendor/dr_lib",
		"%{prj.name}/vendor/rectpack2D"
	}
	
	libdirs
	{
		"%{prj.name}/vendor/GLEW/lib",
		"%{prj.name}/vendor/GLFW/lib",
		"%{prj.name}/vendor/OpenALSoft/lib",
		"%{prj.name}/vendor/FreeType/lib",
	}
	
	links
	{
		"glfw3",
		"opengl32",
		"glew32s",
		"OpenAL32.lib",
		"freetype28MT.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ASTRA_PLATFORM_WINDOWS",
			"ASTRA_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"GLEW_STATIC",
			"_CRT_SECURE_NO_WARNINGS"
		}

		postbuildcommands
		{
			-- Post build copy of required OpenAL dll
			("{COPY} vendor/OpenALSoft/lib/OpenAL32.dll ../bin/" .. outputdir .."/TestGame")
		}

	filter "configurations:Debug"
		defines
		{
			"ASTRA_DEBUG",
			"ASTRA_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		defines "ASTRA_RELEASE"
		defines
		{
			"ASTRA_RELEASE"
		}
		runtime "Release"
		optimize "on"

		postbuildcommands
		{
			-- Post build copy of required OpenAL dll
			("{COPY} vendor/OpenALSoft/lib/OpenAL32.dll ../bin/" .. outputdir .."/TestGame")
		}

	filter "configurations:Dist"
		defines "ASTRA_DIST"
		runtime "Release"
		optimize "on"

		postbuildcommands
		{
			-- Post build copy of required OpenAL dll
			("{COPY} vendor/OpenALSoft/lib/OpenAL32.dll ../bin/" .. outputdir .."/TestGame")
		}

		postbuildcommands
		{
			-- Post build copy of resource files
			("{COPY} res/shaders/**.shader ../bin/" .. outputdir .."/TestGame/res/shaders"),
			("{COPY} res/textures/**.png ../bin/" .. outputdir .."/TestGame/res/textures"),
			("{COPY} res/textures/**.jpg ../bin/" .. outputdir .."/TestGame/res/textures"),
			("{COPY} res/fonts/**.ttf ../bin/" .. outputdir .."/TestGame/res/fonts")
		}

project "TestGame"
	location "TestGame"
	kind "ConsoleApp"

	language "C++"
	cppdialect "C++17"

	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Astra/src",

		"Astra/vendor/GLEW/include",
		"Astra/vendor/GLFW/include",
		"Astra/vendor/OpenALSoft/include",
		"Astra/vendor/FreeType/include",
		
		-- Header Only
		"Astra/vendor/spdlog/include",
		"Astra/vendor/stb_image",
		"Astra/vendor/dr_lib",
		"Astra/vendor/rectpack2D"
	}
	
	libdirs
	{
		"Astra/vendor/GLEW/lib",
		"Astra/vendor/GLFW/lib",
		"Astra/vendor/OpenALSoft/lib",
		"Astra/vendor/FreeType/lib",
	}

	links
	{
		"Astra",
		"glfw3",
		"opengl32",
		"glew32s",
		"OpenAL32.lib",
		"freetype28MT.lib"
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"ASTRA_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
			"GLEW_STATIC",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "ASTRA_DEBUG"
		runtime "Debug"
		symbols "On"
	filter "configurations:Release"
		defines "ASTRA_RELEASE"
		runtime "Release"
		optimize "On"
	filter "configurations:Dist"
		defines "ASTRA_DIST"
		runtime "Release"
		optimize "On"

		postbuildcommands
		{
			-- Post build copy of resource files
			("{COPY} res/**.obj ../bin/" .. outputdir .."/TestGame/res/"),
			("{COPY} res/textures/**.png ../bin/" .. outputdir .."/TestGame/res/textures"),
			("{COPY} res/textures/**.jpg ../bin/" .. outputdir .."/TestGame/res/textures"),
			("{COPY} res/fonts/**.ttf ../bin/" .. outputdir .."/TestGame/res/fonts"),
			("{COPY} res/audio/**.wav ../bin/" .. outputdir .."/TestGame/res/audio")
		}