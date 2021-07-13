workspace "ChernoHazel"
    architecture "x64"
    
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "ChernoHazel/vendor/GLFW/include"
IncludeDir["Glad"] = "ChernoHazel/vendor/Glad/include"

include "ChernoHazel/vendor/GLFW"
include "ChernoHazel/vendor/Glad"
    
project "ChernoHazel"
    location "ChernoHazel"
    kind "SharedLib"
    language "C++"
    
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
		"%{IncludeDir.Glad}"
    }
    
    links
    {
        "GLFW",
		"Glad",
        "opengl32.lib"
    }
    
    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        
        defines
        {
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
        }
    
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
        
    filter "configurations:Debug"
        defines "HZ_DEBUG"
		buildoptions "/MDd"
        symbols "On"
        
    filter "configurations:Release"
        defines "HZ_RELEASE"
		buildoptions "/MD"
        optimize "On"
        
    filter "configurations:Dist"
        defines "HZ_DIST"
		buildoptions "/MD"
        optimize "On"

        
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    
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
        staticruntime "On"
        systemversion "latest"
        
        defines
        {
            "HZ_PLATFORM_WINDOWS"
        }
        
    filter "configurations:Debug"
        defines "HZ_DEBUG"
		-- SE DER MERDA MUDA PRA /MTd e /MT --
		buildoptions "/MDd"
        symbols "On"
        
    filter "configurations:Release"
        defines "HZ_RELEASE"
		buildoptions "/MD"
        optimize "On"
        
    filter "configurations:Dist"
        defines "HZ_DIST"
		buildoptions "/MD"
        optimize "On"
        