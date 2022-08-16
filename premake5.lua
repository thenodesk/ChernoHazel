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
IncludeDir["GLFW"]      = "%{wks.location}/ChernoHazel/vendor/GLFW/include"
IncludeDir["Glad"]      = "%{wks.location}/ChernoHazel/vendor/Glad/include"
IncludeDir["ImGui"]     = "%{wks.location}/ChernoHazel/vendor/imgui"
IncludeDir["glm"]       = "%{wks.location}/ChernoHazel/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/ChernoHazel/vendor/stb_image"
IncludeDir["entt"]      = "%{wks.location}/ChernoHazel/vendor/entt/include"
IncludeDir["yaml_cpp"]  = "%{wks.location}/ChernoHazel/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"]  = "%{wks.location}/ChernoHazel/vendor/ImGuizmo"
IncludeDir["Box2D"]     = "%{wks.location}/ChernoHazel/vendor/Box2D/include"

group "Dependencies"
	include "ChernoHazel/vendor/GLFW"
	include "ChernoHazel/vendor/Glad"
	include "ChernoHazel/vendor/imgui"
	include "ChernoHazel/vendor/yaml-cpp"
	include "ChernoHazel/vendor/Box2D"

group ""
    
project "ChernoHazel"
    location "ChernoHazel"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    
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
        "%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
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
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Box2D}"
    }
    
    links
    {
        "GLFW",
        "Glad",
        "ImGui",
		"yaml-cpp",
		"Box2D",
        "opengl32.lib"
    }
	
	filter "files:ChernoHazel/vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }
    
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
       
	   
project "Hazelnut"
    location "Hazelnut"
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
        "ChernoHazel/vendor/spdlog/include",
        "ChernoHazel/src",
        "ChernoHazel/vendor",
        "%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
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
		
        