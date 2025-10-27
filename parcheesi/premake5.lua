-- premake5.lua
workspace "Parcheesi"
   architecture "x64"
   configurations { "Debug", "Release" }
   startproject "Parcheesi"
   location "build"

project "Parcheesi"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"

   files
   {
      "**.h",
      "**.cc"
   }
