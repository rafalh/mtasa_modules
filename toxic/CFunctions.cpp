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
#include "CFileList.h"
#include "CResPathCache.h"
#include "zlib/zlib.h"
#include <stdint.h>

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

	CFileList List(strPattern);
	while(true)
	{
		string strName;
		int iRet = List.GetNextFile(strName);
		if(iRet < 0)
			break;

		//g_pModuleManager->Printf("Found %s\n", strName.c_str());
		if((iRet == CFileList::FILE && bFiles) || (iRet == CFileList::DIR && bDirs))
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
    if(!luaVM)
        return 0;
    
	lua_pushboolean(luaVM, false);
    return 1;
}

int CFunctions::CompressJSON(lua_State* luaVM)
{
    if(!luaVM)
        return 0;
    
    size_t cbData;
    const char *pszData = lua_tolstring(luaVM, 1, &cbData);
    if(!pszData)
    {
        lua_pushboolean(luaVM, false);
        g_pModuleManager->ErrorPrintf("Invalid JSON data\n");
        return 1;
    }
    
	bool bStr = false, bSlash = false;
	string strRet = "";
	strRet.reserve(cbData);
	
	for(unsigned i = 0; i < cbData; ++i)
	{
	    char ch = pszData[i];
	    switch(ch)
	    {
	        case ' ':
                if(bStr || bSlash)
                    strRet += ch;
                bSlash = false;
                break;
            case '"':
                strRet += ch;
                bStr = !bStr;
                bSlash = false;
                break;
            case '\\':
                strRet += ch;
                bSlash = !bSlash;
                break;
            default:
                strRet += ch;
	    }
	}
    
    lua_pushstring(luaVM, strRet.c_str());
    return 1;
}

int CFunctions::zlibCompress(lua_State* luaVM)
{
    if(!luaVM)
        return 0;
    
    size_t cbInput;
    const char *pInput = lua_tolstring(luaVM, 1, &cbInput);
    if(!pInput)
    {
        lua_pushboolean(luaVM, false);
        g_pModuleManager->ErrorPrintf("Invalid zlibCompress data\n");
        return 1;
    }
    
    uLongf cbOutput = compressBound(cbInput);
    char *pOutput = new char[cbOutput + 4];
    *((uint32_t*)pOutput) = cbInput; // save length at the beggining
    if(compress((Bytef*)pOutput + 4, &cbOutput, (Bytef*)pInput, cbInput) != Z_OK)
    {
        delete[] pOutput;
        lua_pushboolean(luaVM, false);
        g_pModuleManager->ErrorPrintf("compress failed\n");
        return 1;
    }
    lua_pushlstring(luaVM, pOutput, cbOutput + 4);
    delete[] pOutput;
    return 1;
}

int CFunctions::zlibUncompress(lua_State* luaVM)
{
    if(!luaVM)
        return 0;
    
    size_t cbInput;
    const char *pInput = lua_tolstring(luaVM, 1, &cbInput);
    if(!pInput || cbInput < 4)
    {
        lua_pushboolean(luaVM, false);
        g_pModuleManager->ErrorPrintf("Invalid zlibDecompress data\n");
        return 1;
    }
    
    uLongf cbOutput = *((uint32_t*)pInput);
    char *pOutput = new char[cbOutput];
    if(uncompress((Bytef*)pOutput, &cbOutput, (Bytef*)pInput + 4, cbInput - 4) != Z_OK)
    {
        delete[] pOutput;
        lua_pushboolean(luaVM, false);
        g_pModuleManager->ErrorPrintf("uncompress failed\n");
        return 1;
    }
    lua_pushlstring(luaVM, pOutput, cbOutput);
    delete[] pOutput;
    return 1;
}
