workspace "ChernoHazel"
    architecture "x64"
    
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "ChernoHazel/vendor/GLFW/include"
IncludeDir["Glad"] = "ChernoHazel/vendor/Glad/include"
IncludeDir["ImGui"] = "ChernoHazel/vendor/imgui"
IncludeDir["glm"] = "ChernoHazel/vendor/glm"
IncludeDir["stb_image"] = "ChernoHazel/vendor/stb_image"

group "Dependencies"
	include "ChernoHazel/vendor/GLFW"
	include "ChernoHazel/vendor/Glad"
	include "ChernoHazel/vendor/imgui"

group ""
    
project "ChernoHazel"
    location "ChernoHazel"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "hzpch.h"
    pchsource "ChernoHazel/src/hzpch.cpp"
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
    
    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
    }
    
    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }
    
    filter "system:windows"
        systemversion "latest"
        
        defines
        {
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
        
    filter "configurations:Debug"
        defines "HZ_DEBUG"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "HZ_RELEASE"
        runtime "Release"
        optimize "on"
        
    filter "configurations:Dist"
        defines "HZ_DIST"
        runtime "Release"
        optimize "on"

        
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    includedirs
    {
        "ChernoHazel/vendor/spdlog/include",
        "ChernoHazel/src",
        "ChernoHazel/vendor",
        "%{IncludeDir.glm}"
    }
    
    links
    {
        "ChernoHazel"
    }
    
    
    filter "system:windows"
        systemversion "latest"
        
        defines
        {
            "HZ_PLATFORM_WINDOWS"
        }
        
    filter "configurations:Debug"
        defines "HZ_DEBUG"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "HZ_RELEASE"
        runtime "Release"
        optimize "on"
        
    filter "configurations:Dist"
        defines "HZ_DIST"
        runtime "Release"
        optimize "on"
        