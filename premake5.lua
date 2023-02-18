include "Dependencies.lua"

workspace "ChernoHazel"
    architecture "x86_64"
	startproject "Hazelnut"
    
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }
	
	-- solution_items
	-- {
		-- ".editorconfig"
	-- }
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)

group "Dependencies"
	-- include "vendor/bin/premake"
	include "ChernoHazel/vendor/GLFW"
	include "ChernoHazel/vendor/Glad"
	include "ChernoHazel/vendor/imgui"
	include "ChernoHazel/vendor/yaml-cpp"
	include "ChernoHazel/vendor/Box2D"

group ""

group "Core"
	include "ChernoHazel"
	include "Hazel-ScriptCore"
group ""

group "Tools"
include "Hazelnut"
group ""

group "Misc"
include "Sandbox"
group ""
