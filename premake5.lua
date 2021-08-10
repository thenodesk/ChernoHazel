workspace "ChernoHazel"
    architecture "x86_64"
	startproject "Hazelnut"
    
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
IncludeDir["ImGui"] = "ChernoHazel/vendor/imgui"
IncludeDir["glm"] = "ChernoHazel/vendor/glm"
IncludeDir["stb_image"] = "ChernoHazel/vendor/stb_image"
IncludeDir["entt"] = "ChernoHazel/vendor/entt/include"
IncludeDir["yaml_cpp"] = "ChernoHazel/vendor/yaml-cpp/include"

group "Dependencies"
	include "ChernoHazel/vendor/GLFW"
	include "ChernoHazel/vendor/Glad"
	include "ChernoHazel/vendor/imgui"
	include "ChernoHazel/vendor/yaml-cpp"

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
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}"
    }
    
    links
    {
        "GLFW",
        "Glad",
        "ImGui",
		"yaml-cpp",
        "opengl32.lib"
    }
    
    filter "system:windows"
        systemversion "latest"
        
        defines
        {
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
       
	   
project "Hazelnut"
    location "Hazelnut"
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
        "%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
    }
    
    links
    {
        "ChernoHazel"
    }
    
    filter "system:windows"
        systemversion "latest"
        
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
        