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
	links { "lua5.1" }
	includedirs { ".", "include", "extra", "luaimports" }
	targetprefix ""
	targetdir "bin"
	
	configuration "windows"
		links { "ws2_32" }
	
	configuration "linux"
		files { "luaimports/*.cpp" }
