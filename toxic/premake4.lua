project "ml_toxic"
    kind "SharedLib"
    language "C++"
    files {
        "src/*.cpp", "src/*.h",
        --"extra/*.cpp", "extra/*.h",
        "include/*.h",
    }
    includedirs {
        "src",
        "include",
        --"extra",
        "luaimports",
        --"../vendor/Selene/include",
        "../vendor/zlib",
        "../vendor/curl/include",
    }
    defines { "CURL_STATICLIB" }
    pchheader "stdafx.h"
    pchsource "src/stdafx.cpp"

    targetprefix ""

    cppdialect "C++11"

    configuration "gmake"
        buildoptions "-fPIC"

    configuration "linux"
        files {
            "luaimports/*.cpp", "luaimports/*.h",
        }
        links {
            "z",
            "curl",
        }

    configuration "windows"
        libdirs {
            "../vendor/curl/lib",
            "../vendor/lua",
        }
        links {
            "lua5.1",
            "zlib",
            "libcurl_static",
        }
