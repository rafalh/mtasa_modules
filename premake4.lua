solution "Modules"
	configurations { "Debug", "Release" }
	defines { "_CRT_SECURE_NO_WARNINGS", "_USRDLL" }
	language "C++" -- default
	
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
	
	configuration "Release"
		defines { "NDEBUG" }
		flags { "OptimizeSize" }

	configuration "windows"
		defines { "WIN32", "_WINDOWS" }
		
	configuration "linux"
		defines { "LINUX" }
	
	include "toxic"
	include "sockets"
	include "vendor/zlib"
