#pragma once

#include "stdafx.h"
#include "ResourcePathCache.h"

class FileUtils
{
public:
	void InitResource(lua_State *luaVM);
	void CleanupResource(lua_State *luaVM) {}
	static int FileFind(lua_State *luaVM);
	static int FileIsDirectory(lua_State *luaVM);
	static int FileModTime(lua_State *luaVM);

private:
	static ResourcePathCache m_ResPathCache;
};
