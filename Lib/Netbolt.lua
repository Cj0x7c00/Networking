project "NetBoltLib"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    location "."

    files {
        "src/**.cpp",
        "src/Platform/**.cpp",
        "include/**.h"
    }

    includedirs {
        "include",
        "src"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"