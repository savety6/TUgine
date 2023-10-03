workspace "TUgine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "TUgine"
    location "TUgine"
    kind "SharedLib"
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
        "%{prj.name}/Submodules/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "TUG_PLATFORM_WINDOWS",
            "TUG_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "TUG_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "TUG_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "TUG_DIST"
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
        "TUgine/Submodules/spdlog/include",
        "TUgine/src"
    }

    links
    {
        "TUgine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "TUG_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "TUG_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "TUG_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "TUG_DIST"
        optimize "On"