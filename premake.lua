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

    includedirs { "%{prj.name}/src", "libs/asio/include", "libs/fmod/core/inc", "libs/freetype/include",
        "libs/glad/include", "libs/glfw/include", "libs/glm", "libs/stb", "libs/json/single_include" }

    files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp", "%{prj.name}/src/**.c", "%{prj.name}/src/**.tpp" }

    -- Project platform define macro based on identified system
    filter "system:windows"
        defines "_PLATFORM_WINDOWS"
        libdirs { "bin/fmod/win64", "bin/freetype/win64", "bin/glfw/win64" }

    filter "system:macosx"
        defines "_PLATFORM_MACOSX"

    -- Project settings with values unique to the Debug/Release configurations
    filter "configurations:Debug"
        kind "ConsoleApp"
        objdir "objs/debug"
        targetname "SquareRun-dbg"

        links { "glfw3-dbg", "freetype-dbg", "fmodL_vc" }
        defines { "_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        kind "WindowedApp"
        objdir "objs/release"
        targetname "SquareRun"

        links { "glfw3", "freetype", "fmod_vc" }
        defines { "NDEBUG" }
        optimize "Speed"
        entrypoint "mainCRTStartup"

    -- Copy required DLL lib files into game executable directory when building
    filter { "system:windows", "configurations:Debug" }
        postbuildcommands { "copy ..\\bin\\fmod\\win64\\fmodL.dll ..\\bin\\game\\fmodL.dll" }

    filter { "system:windows", "configurations:Release" }
        postbuildcommands { "copy ..\\bin\\fmod\\win64\\fmod.dll ..\\bin\\game\\fmod.dll" }

------------------------------------------------------------------------------------------------------------------------------------------------
