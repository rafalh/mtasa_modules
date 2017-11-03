workspace "Modules"
	configurations { "Debug", "Release" }
	defines { "_CRT_SECURE_NO_WARNINGS", "_USRDLL" }
	language "C++" -- default

	location "premake-build"
	
	configuration "Debug"
		defines { "DEBUG" }
		symbols "On"
		targetdir "bin/Debug"
	
	configuration "Release"
		defines { "NDEBUG" }
		optimize "Size"
		targetdir "bin/Release"

	configuration "windows"
		defines { "WIN32", "_WINDOWS" }
		
	configuration "linux"
		defines { "LINUX" }
	
	include "toxic"
	--include "sockets"
	include "vendor"
