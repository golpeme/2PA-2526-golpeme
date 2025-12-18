workspace "BlackJack"
   architecture "x64"
   configurations { "Debug", "Release" }
   startproject "BlackJack"
   location "build"

project "MGMPlayer"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   includedirs { "include" }
   files { "include/MGMPlayer.h", "src/MGMPlayer.cc" }

project "MGMTable"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   includedirs { "include" }
   files { "include/MGMTable.h", "src/MGMTable.cc" }

project "BlackJack"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   links { "MGMPlayer", "MGMTable" }
   includedirs { "include", "ftxui/install/include" }
   libdirs { "ftxui/install/lib" }
   links { "ftxui-screen", "ftxui-dom", "ftxui-component" }

   files
   {
      "include/IGame.h",
      "include/IPlayer.h",
      "include/ITable.h",
      "include/Rules.h",
      "include/MGMGame.h",
      "include/MGMRules.h",
      "src/MGMGame.cc",
      "src/MGMPlayer.cc",
      "src/MGMTable.cc",
      "src/main.cc"
   }