/*********************************************************
*
*  Multi Theft Auto: San Andreas - Deathmatch
*
*  ml_base, External lua add-on module
*
*  Copyright © 2003-2008 MTA.  All Rights Reserved.
*
*  Grand Theft Auto is © 2002-2003 Rockstar North
*
*  THE FOLLOWING SOURCES ARE PART OF THE MULTI THEFT
*  AUTO SOFTWARE DEVELOPMENT KIT AND ARE RELEASED AS
*  OPEN SOURCE FILES. THESE FILES MAY BE USED AS LONG
*  AS THE DEVELOPER AGREES TO THE LICENSE THAT IS
*  PROVIDED WITH THIS PACKAGE.
*
*********************************************************/

#include "CFunctions.h"
#include "CLuaArguments.h"
#include "CFileList.h"
#include "CResPathCache.h"

using namespace std;

extern ILuaModuleManager10 *g_pModuleManager;
CResPathCache g_ResPathCache;

int CFunctions::FileFind(lua_State *luaVM)
{
    if(!luaVM)
        return 0;
    
	const char *pszPattern = luaL_optstring(luaVM, 1, "*");
	const char *pszType = luaL_optstring(luaVM, 2, "all");

	bool bDirs = !strcmp(pszType, "all") || !strcmp(pszType, "directory");
	bool bFiles = !strcmp(pszType, "all") || !strcmp(pszType, "file");

	if(strstr(pszPattern, ".."))
	{
		lua_pushboolean(luaVM, false);
		g_pModuleManager->ErrorPrintf("Invalid path\n");
		return 1;
	}

	string strPattern;
	if(!g_ResPathCache.ParseResPath(luaVM, pszPattern, strPattern))
	{
		lua_pushboolean(luaVM, false);
		g_pModuleManager->ErrorPrintf("ParseResPath failed\n");
		return 1;
	}

	strPattern = "mods/deathmatch/resources/" + strPattern;
	//g_pModuleManager->Printf("Find %s %u %u\n", strPattern.c_str(), bFiles, bDirs);

    lua_newtable(luaVM);
	unsigned i = 0;

	CFileList List(strPattern.c_str());
	while(true)
	{
		string strName;
		int iRet = List.GetNextFile(strName);
		if(iRet < 0)
			break;

		//g_pModuleManager->Printf("Found %s\n", strName.c_str());
		if((iRet == 0 && bFiles) || (iRet == 1 && bDirs))
		{
			lua_pushinteger(luaVM, i + 1);
			lua_pushstring(luaVM, strName.c_str());
			lua_settable(luaVM, -3);
			++i;
		}
	}
    
    return 1;
}

int CFunctions::FileIsDirectory(lua_State *luaVM)
{
	lua_pushboolean(luaVM, false);
    return 1;
}
