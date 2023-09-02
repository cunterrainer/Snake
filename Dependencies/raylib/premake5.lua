project "raylib"
    language "C"
    kind "StaticLib"
    defines "PLATFORM_DESKTOP"
    disablewarnings "deprecated-declarations"

    files {
        "src/**.c",
        "src/**.h"
    }

    removefiles {
        "src/external/glfw/**.c",
        "src/external/glfw/**.h"
    }

    includedirs {
        "src/external/glfw/include"
    }

    filter "system:macosx"
        files "src/rglfw.m"
        removefiles "src/rglfw.c"