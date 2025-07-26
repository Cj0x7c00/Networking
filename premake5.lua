workspace "NetBolt"
   configurations { "Debug", "Release" }
   platforms { "x64", "arm64" }

   filter "platforms:x64"
      architecture "x86_64"

    filter "platforms: arm64"
      architecture "arm64"

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      optimize "Off"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"


    startproject "Tests"

    group "NetBolt"
        include "Lib/NetBolt.lua"
    group ""

    group "Tests"
        include "Test/Tests.lua"
    group ""