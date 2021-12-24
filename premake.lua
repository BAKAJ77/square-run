workspace "SquareRun"
    configurations { "Debug", "Release" }
    architecture "x86_64"

------------------------------------------------------------------------------------------------------------------------------------------------

project "SquareRun"
    location "%{prj.name}"

    staticruntime "on"
    language "C++"
    cppdialect "C++17"

    targetdir "bin/game/"
    objdir "objs/%{cfg.buildcfg}-%{cfg.architecture}/"

    includedirs { "%{prj.name}/src" }
    files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp", "%{prj.name}/src/**.tpp" }

    -- Project platform define macro based on identified system
    filter "system:windows"
        defines "_PLATFORM_WINDOWS"

    filter "system:macosx"
        defines "_PLATFORM_MACOSX"

    -- Project settings with values unique to the Debug/Release configurations
    filter "configurations:Debug"
        kind "ConsoleApp"
        targetname "SquareRun-Dbg"
        defines { "_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        kind "WindowedApp"
        targetname "SquareRun"
        entrypoint "mainCRTStartup"
        defines { "NDEBUG" }
        optimize "Speed"

------------------------------------------------------------------------------------------------------------------------------------------------
