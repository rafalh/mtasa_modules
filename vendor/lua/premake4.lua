project "lua"
	kind "SharedLib"
	files {
		"src/lapi.c",
		"src/lcode.c",
		"src/ldebug.c",
		"src/ldo.c",
		"src/ldump.c",
		"src/lfunc.c",
		"src/lgc.c",
		"src/llex.c",
		"src/lmem.c",
		"src/lobject.c",
		"src/lopcodes.c",
		"src/lparser.c",
		"src/lstate.c",
		"src/lstring.c",
		"src/ltable.c",
		"src/ltm.c",
		"src/lundump.c",
		"src/lvm.c",
		"src/lzio.c",
		"src/lauxlib.c",
		"src/lbaselib.c",
		"src/ldblib.c",
		"src/liolib.c",
		"src/lmathlib.c",
		"src/loslib.c",
		"src/ltablib.c",
		"src/lstrlib.c",
		"src/loadlib.c",
		"src/linit.c",
		"src/lua5.1.def",
	}
	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"_CRT_NONSTDC_NO_DEPRECATE",
		"LUA_USE_APICHECK"
	}
	configuration "Release"
		defines { "NDEBUG", "MTA_RELEASE" }
		targetname "lua5.1"
	configuration "Debug"
		defines { "DEBUG", "MTA_DEBUG" }
		targetname "lua5.1d"
