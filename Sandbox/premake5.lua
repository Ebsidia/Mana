project "Sandbox"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Mana/vendor/spdlog/include",
        "%{wks.location}/Mana/src",
        "%{wks.location}/Mana/vendor",
        "%{IncludeDir.glm}"
    }

    links
    {
        "Mana"
    }

    filter "system:windows"
        systemversion "latest"

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