project "raylib"
    language "C"
    kind "StaticLib"
    defines "PLATFORM_DESKTOP"

    files {
        "src/**.h",
        "src/rcore.c",
        "src/rmodels.c",
        "src/rshapes.c",
        "src/rtext.c",
        "src/rtextures.c",
        "src/utils.c",
        "src/rglfw.c",
    }

    includedirs {
        "src/external/glfw/include"
    }

    filter "system:macosx"
        files "src/rglfw.m"
        removefiles "src/rglfw.c"

    filter { "toolset:gcc* or toolset:clang*" }
        disablewarnings "deprecated-declarations"