project "Tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    location "."

    files {
        "**.cpp",
        "**.h"
    }

    includedirs {
        "../Lib/include",
    }

    libdirs {
        "bin/%{cfg.buildcfg}"
    }

    links { "NetBoltLib" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"