-- premake5.lua
workspace "Parcheesi"
   architecture "x64"
   configurations { "Debug", "Release" }
   startproject "Parcheesi"
   location "build"

project "MGMParcheesiPlayer"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   files { "MGMPlayer.h" ,"MGMPlayer.cc"}

project "MGMParcheesiBoard"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"

   files { "MGMParcheesi.cc" ,"MGMParcheesi.h" }

project "Parcheesi"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   links {"MGMParcheesiPlayer","MGMParcheesiBoard"}

   files
   {
      "game.h",
      "iparcheesi.h",
      "mock_parcheesi.h",

      "game.cc",
      "main.cc",
   }
