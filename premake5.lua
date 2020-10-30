--Workspace is the solution
workspace "Mana"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include Directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Mana/vendor/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/Mana/vendor/GLAD/include"
IncludeDir["ImGui"] = "%{wks.location}/Mana/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Mana/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Mana/vendor/stb_image"

include "Mana/vendor/GLFW" -- includes the premake5.lua file from "Mana/vendor/GLFW"
include "Mana/vendor/GLAD"
include "Mana/vendor/ImGui"

project "Mana"
    location "Mana"
    kind "StaticLib" --changed mana to use a StaticLib from a SharedLib(dynamic lib)
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "mapch.h"
    pchsource "Mana/src/mapch.cpp"

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
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"
    }

    links
    {
        "GLFW",
        "GLAD",
        "ImGui",
        "opengl32.lib"
        
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "MA_PLATFORM_WINDOWS",
            "MA_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
        }

    filter "configurations:Debug"
        defines "MA_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "MA_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "MA_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
        location "Sandbox"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Mana/vendor/spdlog/include",
        "Mana/src",
        "Mana/vendor",
        "%{IncludeDir.glm}"
    }

    links
    {
        "Mana"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "MA_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "MA_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "MA_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "MA_DIST"
        runtime "Release"
        optimize "on"