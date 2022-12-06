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

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)

group "Dependencies"
	include "ChernoHazel/vendor/GLFW"
	include "ChernoHazel/vendor/Glad"
	include "ChernoHazel/vendor/imgui"
	include "ChernoHazel/vendor/yaml-cpp"
	include "ChernoHazel/vendor/Box2D"

group ""

include "ChernoHazel"
include "Sandbox"
include "Hazelnut"
