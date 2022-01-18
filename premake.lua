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

    includedirs { "%{prj.name}/src", "libs/asio/include", "libs/sdl2/include", "libs/sdl2_mixer/include", "libs/freetype/include",
        "libs/glad/include", "libs/glfw/include", "libs/glm", "libs/stb", "libs/json/single_include" }

    files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp", "%{prj.name}/src/**.c", "%{prj.name}/src/**.tpp" }

    -- Project platform define macro based on identified system
    filter "system:windows"
        defines "_PLATFORM_WINDOWS"
        libdirs { "bin/sdl2/win64", "bin/sdl2_mixer/win64", "bin/freetype/win64", "bin/glfw/win64" }

    filter "system:macosx"
        defines "_PLATFORM_MACOSX"

    -- Project settings with values unique to the Debug/Release configurations
    filter "configurations:Debug"
        kind "ConsoleApp"
        objdir "objs/debug"
        targetname "SquareRun-dbg"

        links { "glfw3-dbg", "freetype-dbg", "SDL2", "SDL2main", "SDL2_mixer" }
        defines { "_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        kind "WindowedApp"
        objdir "objs/release"
        targetname "SquareRun"

        links { "glfw3", "freetype", "SDL2", "SDL2main", "SDL2_mixer" }
        defines { "NDEBUG" }
        optimize "Speed"
        entrypoint "mainCRTStartup"

    -- Copy required DLL lib files into game executable directory when building
    filter { "system:windows" }
        postbuildcommands { "copy ..\\bin\\sdl2\\win64\\SDL2.dll ..\\bin\\game\\SDL2.dll",
                            "copy ..\\bin\\sdl2_mixer\\win64\\SDL2_mixer.dll ..\\bin\\game\\SDL2_mixer.dll" }

------------------------------------------------------------------------------------------------------------------------------------------------
