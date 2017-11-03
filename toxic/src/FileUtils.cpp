#include "stdafx.h"
#include "FileUtils.h"
#include "FileList.h"
#include "module.h"
#include "LuaHelper.h"

using namespace std;

ResourcePathCache FileUtils::m_ResPathCache;

void FileUtils::InitResource(lua_State *luaVM)
{
	g_pModuleManager->RegisterFunction(luaVM, "fileFind", FileFind);
	//g_pModuleManager->RegisterFunction(luaVM, "fileIsDirectory", FileIsDirectory);
	g_pModuleManager->RegisterFunction(luaVM, "fileModTime", FileModTime);
}

int FileUtils::FileFind(lua_State *luaVM)
{
	const char *pszPattern = luaL_optstring(luaVM, 1, "*");
	const char *pszType = luaL_optstring(luaVM, 2, "all");

	bool bDirs = !strcmp(pszType, "all") || !strcmp(pszType, "directory");
	bool bFiles = !strcmp(pszType, "all") || !strcmp(pszType, "file");

	string strPattern;
	if (!m_ResPathCache.ParseResPath(luaVM, pszPattern, strPattern))
	{
		lua_pushboolean(luaVM, false);
		g_pModuleManager->ErrorPrintf("ParseResPath failed\n");
		return 1;
	}

	if (strPattern.find("..") != strPattern.npos)
	{
		lua_pushboolean(luaVM, false);
		g_pModuleManager->ErrorPrintf("Invalid path\n");
		return 1;
	}

	strPattern = "mods/deathmatch/resources/" + strPattern;
	//g_pModuleManager->Printf("Find %s %u %u\n", strPattern.c_str(), bFiles, bDirs);

	lua_newtable(luaVM);
	unsigned i = 0;

	FileList List(strPattern);
	while (true)
	{
		string strName;
		int iRet = List.GetNextFile(strName);
		if (iRet < 0)
			break;

		//g_pModuleManager->Printf("Found %s\n", strName.c_str());
		if ((iRet == FileList::FILE && bFiles) || (iRet == FileList::DIR && bDirs))
		{
			lua_pushinteger(luaVM, i + 1);
			lua_pushstring(luaVM, strName.c_str());
			lua_settable(luaVM, -3);
			++i;
		}
	}

	return 1;
}

int FileUtils::FileIsDirectory(lua_State *luaVM)
{
	lua_pushboolean(luaVM, false);
	return 1;
}

int FileUtils::FileModTime(lua_State *luaVM)
{
	LuaHelper lua(luaVM);
	string relativePath = lua.ReadArg<string>();
	string fullPath;

	if (!m_ResPathCache.ParseResPath(luaVM, relativePath, fullPath))
	{
		g_pModuleManager->ErrorPrintf("fileModTime: ParseResPath failed\n");
		lua.Push(false);
		return 1;
	}

	if (fullPath.find("..") != fullPath.npos)
	{
		g_pModuleManager->ErrorPrintf("fileModTime: Invalid path\n");
		lua.Push(false);
		return 1;
	}

	fullPath = string("mods/deathmatch/resources/") + fullPath;

#ifdef _WIN32
	struct _stat32 info;
	int ret = _stat32(fullPath.c_str(), &info);
#else
	struct stat info;
	int ret = stat(fullPath.c_str(), &info);
#endif
	if (ret != 0)
	{
		g_pModuleManager->ErrorPrintf("fileModTime: stat failed\n");
		lua.Push(false);
	}
	else
		lua.Push<int>(info.st_mtime);

	return 1;
}
