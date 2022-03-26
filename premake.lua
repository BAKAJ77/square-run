workspace "square-run"
    configurations { "debug", "release" }
    architecture "x86_64"

------------------------------------------------------------------------------------------------------------------------------------------------

project "square-run"
    location "%{prj.name}"
    staticruntime "on"
    language "C++"
    cppdialect "C++17"

    targetname "square-run"
    targetdir "bin/%{cfg.buildcfg}/"
    objdir "objs/%{prj.name}/%{cfg.buildcfg}/"

    includedirs { "%{prj.name}/src", "libs/asio/asio/include", "libs/irrklang/include", "libs/freetype/include", "libs/glad/include",
        "libs/glfw/include", "libs/glm", "libs/stb", "libs/json/include" }

    files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp", "%{prj.name}/src/**.c", "%{prj.name}/src/**.tpp" }

    links { "glfw3", "freetype", "irrKlang" }

    -- Project platform define macro based on identified system
    filter "system:windows"
        defines "_PLATFORM_WINDOWS"

    filter "system:macosx"
        defines "_PLATFORM_MACOSX"

    -- Project settings with values unique to the Debug/Release configurations
    filter "configurations:debug"
        kind "ConsoleApp"
        
        libdirs { "libs/irrklang/bin", "libs/freetype/bin/debug", "libs/glfw/bin/debug" }
        defines { "_DEBUG" }
        symbols "On"

    filter "configurations:release"
        kind "WindowedApp"
        entrypoint "mainCRTStartup"

        libdirs { "libs/irrklang/bin", "libs/freetype/bin/release", "libs/glfw/bin/release" }
        defines { "NDEBUG" }
        optimize "Speed"

    -- Copy required DLL lib files into game executable directory when building
    filter { "system:windows", "configurations:debug" }
        postbuildcommands { "copy ..\\libs\\irrklang\\bin\\irrKlang.dll ..\\bin\\debug\\irrKlang.dll",
            "copy ..\\libs\\irrklang\\bin\\ikpMP3.dll ..\\bin\\debug\\ikpMP3.dll" }

    filter { "system:windows", "configurations:release" }
        postbuildcommands { "copy ..\\libs\\irrklang\\bin\\irrKlang.dll ..\\bin\\release\\irrKlang.dll",
            "copy ..\\libs\\irrklang\\bin\\ikpMP3.dll ..\\bin\\release\\ikpMP3.dll" }

------------------------------------------------------------------------------------------------------------------------------------------------
