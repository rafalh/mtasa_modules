#pragma once

#include "stdafx.h"

class CompressionUtils
{
public:
    void InitResource(lua_State *luaVM);
    void CleanupResource(lua_State *luaVM) {}
        
    static int CompressJSON(lua_State *luaVM);
    static int zlibCompress(lua_State *luaVM);
    static int zlibUncompress(lua_State *luaVM);
};
