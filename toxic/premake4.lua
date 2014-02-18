project "ml_toxic"
	kind "SharedLib"
	language "C++"
	files {
		"*.cpp", "*.h",
		"luaimports/*.h",
		--"extra/*.cpp", "extra/*.h",
		"include/*.h",
	}
	libdirs { "lib" }
	links { "zlib" }
	includedirs { ".", "include", "extra", "luaimports", "../vendor" }
	targetprefix ""
	targetdir "bin"
	
	configuration "windows"
		links { "lua5.1", "ws2_32" }
	
	configuration "linux"
		files { "luaimports/*.cpp" }
