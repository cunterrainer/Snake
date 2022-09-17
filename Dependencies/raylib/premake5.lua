project "raylib"
    language "C"
    kind "StaticLib"

    defines "PLATFORM_DESKTOP"
    
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