include "./vendor/premake/premake_customization/solution_items.lua"

--Workspace is the solution
workspace "Mana"
    architecture "x86_64"
    startproject "Mana-Editor"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    solution_items
	{
		".editorconfig"
    }
    
    flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include Directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Mana/vendor/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/Mana/vendor/GLAD/include"
IncludeDir["ImGui"] = "%{wks.location}/Mana/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Mana/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Mana/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Mana/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Mana/vendor/yaml-cpp/include"
IncludeDir["irrKlang"] = "%{wks.location}/Breakout/vendor/irrKlang/include"

group "Dependencies"
    include "vendor/premake"
    include "Mana/vendor/GLFW"
    include "Mana/vendor/GLAD"
    include "Mana/vendor/imgui"
    include "Mana/vendor/yaml-cpp"
group ""

include "Mana"
include "Sandbox"
include "Mana-Editor"
include "Breakout"

