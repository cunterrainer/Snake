workspace "Snake"
    configurations {
        "Debug",
        "Release"
    }
    startproject "Snake"

outputdir = "/BIN/%{cfg.toolset}/%{cfg.shortname}/%{prj.name}/"
cwd = os.getcwd() -- get current working directory
targetdir(cwd .. outputdir .. "bin")
objdir(cwd .. outputdir .. "bin-int")

RaylibDir = cwd .. "/Dependencies/raylib"

filter "system:windows"
    platforms { "x64", "x86" }
    defines { "WINDOWS", "_CRT_SECURE_NO_WARNINGS" }
filter "system:linux"
    platforms "x64"
    defines "LINUX"
filter "system:macosx"
    defines { "MAC_OS", "GL_SILENCE_DEPRECATION" }

filter { "platforms:x64" }
    architecture "x64"
filter { "platforms:x86" }
    architecture "x86"

filter { "configurations:Debug" }
    runtime "Debug"
    symbols "on"
    optimize "off"
filter { "configurations:Release" }
    runtime "Release"
    symbols "off"
    optimize "Speed"
filter {}

-- only for visual studio
flags {
    "MultiProcessorCompile"
}
staticruntime "on"
removeunreferencedcodedata "on"

include "Snake"
include "Dependencies/raylib"