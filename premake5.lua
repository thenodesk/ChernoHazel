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

include "ChernoHazel/vendor/GLFW"
include "ChernoHazel/vendor/Glad"
include "ChernoHazel/vendor/imgui"
    
project "ChernoHazel"
    location "ChernoHazel"
    kind "SharedLib"
    language "C++"
	staticruntime "off"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "hzpch.h"
    pchsource "ChernoHazel/src/hzpch.cpp"
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
    }
    
    links
    {
        "GLFW",
		"Glad",
		"ImGui",
        "opengl32.lib"
    }
    
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        
        defines
        {
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
        }
    
        postbuildcommands
        {
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }
        
    filter "configurations:Debug"
        defines "HZ_DEBUG"
		runtime "Debug"
        symbols "On"
        
    filter "configurations:Release"
        defines "HZ_RELEASE"
		runtime "Release"
        optimize "On"
        
    filter "configurations:Dist"
        defines "HZ_DIST"
		runtime "Release"
        optimize "On"

        
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
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
        "ChernoHazel/vendor/spdlog/include",
        "ChernoHazel/src"
    }
    
    links
    {
        "ChernoHazel"
    }
    
    
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        
        defines
        {
            "HZ_PLATFORM_WINDOWS"
        }
        
    filter "configurations:Debug"
        defines "HZ_DEBUG"
		runtime "Debug"
        symbols "On"
        
    filter "configurations:Release"
        defines "HZ_RELEASE"
		runtime "Release"
        optimize "On"
        
    filter "configurations:Dist"
        defines "HZ_DIST"
		runtime "Release"
        optimize "On"
        