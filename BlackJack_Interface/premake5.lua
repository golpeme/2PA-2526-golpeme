workspace "BlackJack"
   architecture "x64"
   configurations { "Debug", "Release" }
   startproject "BlackJack"
   location "build"
   libdirs{"."}

project "BlackJack"
kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   files
   {
      "IGame.h",
      "IPlayer.h",
      "ITable.h",
      "Rules.h",

      "MGMGame.h",
      "MGMGame.cc",
      "MGMPlayer.h",
      "MGMPlayer.cc",
      "MGMTable.h",
      "MGMTable.cc",
      "main.cc",
      "premake5.lua",
   }