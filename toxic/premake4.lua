project "ml_toxic"
	kind "SharedLib"
	language "C++"
	files {
		"src/*.cpp", "src/*.h",
		--"extra/*.cpp", "extra/*.h",
		"luaimports/*.cpp", "luaimports/*.h",
		"include/*.h",
	}
	includedirs {
		"src",
		"include",
		--"extra",
		"luaimports",
		--"../vendor/Selene/include",
	}
	defines { "CURL_STATICLIB" }
	pchheader "stdafx.h"
	pchsource "src/stdafx.cpp"

	targetprefix ""
	
	configuration "gmake"
		buildoptions "-std=c++11 -fPIC"
	
	configuration "linux"
		links {
			"z",
			"curl",
			--"lua5.1",
		}
		--premake.gcc.cc = "gcc-4.9"
		--premake.gcc.cxx = "g++-4.9"

	configuration "windows"
		links {
			"ws2_32",
			"zlib_static",
			"libcurl",
			"lua5.1",
		}
