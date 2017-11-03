solution "Modules"
	configurations { "Debug", "Release" }
	defines { "_CRT_SECURE_NO_WARNINGS", "_USRDLL" }
	language "C++" -- default
	
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		targetdir "bin/Debug"
	
	configuration "Release"
		defines { "NDEBUG" }
		flags { "OptimizeSize" }
		targetdir "bin/Release"

	configuration "windows"
		defines { "WIN32", "_WINDOWS" }
		includedirs "D:/Libs/include"
		libdirs "D:/Libs/lib/vs2013"
		libdirs "D:/Libs/dll/vs2013"
		
	configuration "linux"
		defines { "LINUX" }
	
	include "toxic"
	--include "sockets"
	include "vendor"
