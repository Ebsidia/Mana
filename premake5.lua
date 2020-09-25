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
IncludeDir["GLFW"] = "Mana/vendor/GLFW/include"
IncludeDir["GLAD"] = "Mana/vendor/GLAD/include"
IncludeDir["ImGui"] = "Mana/vendor/ImGui"

include "Mana/vendor/GLFW" -- includes the premake5.lua file from "Mana/vendor/GLFW"
include "Mana/vendor/GLAD"
include "Mana/vendor/ImGui"

project "Mana"
    location "Mana"
    kind "SharedLib" --dynamic library
    language "C++"
    staticruntime "off"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "mapch.h"
    pchsource "Mana/src/mapch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGui}"
    }

    links
    {
        "GLFW",
        "GLAD",
        "ImGui",
        "opengl32.lib"
        
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "MA_PLATFORM_WINDOWS",
            "MA_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

    filter "configurations:Debug"
        defines "MA_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "MA_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "MA_DIST"
        runtime "Release"
        optimize "On"

project "Sandbox"
        location "Sandbox"
        kind "ConsoleApp"
        language "C++"
        staticruntime "off"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

    }

    includedirs
    {
        "Mana/vendor/spdlog/include",
        "Mana/src"
    }

    links
    {
        "Mana"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "MA_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "MA_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "MA_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "MA_DIST"
        runtime "Release"
        optimize "On"