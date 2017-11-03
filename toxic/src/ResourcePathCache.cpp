#include "stdafx.h"
#include "ResourcePathCache.h"
#include "FileList.h"
#include "module.h"

using namespace std;

bool ResourcePathCache::GetResPath(const string &strResName, string &strRet)
{
    map<string, string>::iterator it = m_List.find(strResName);
    if (it != m_List.end())
	{
        strRet = it->second;
		return true;
	}

    if (!Find(strResName, strRet))
		return false;

    m_List.insert(pair<string, string>(strResName, strRet));
    return true;
}

bool ResourcePathCache::ParseResPath(lua_State* luaVM, const string &strPath, string &strRet)
{
	string strResName;
	string strRelPath;

	if (strPath[0] != ':')
	{
		if (!GetThisResourceName(luaVM, strResName))
		{
			g_pModuleManager->ErrorPrintf("GetThisResourceName failed\n");
			return false;
		}
		
		strRelPath = strPath;
	}
	else
	{
		size_t Pos = strPath.find('/');
		if (Pos == string::npos)
		{
			g_pModuleManager->ErrorPrintf("Invalid path\n");
			return false;
		}
		
		strResName = strPath.substr(1, Pos - 1);
		strRelPath = strPath.substr(Pos + 1);
	}

	string strResPath;
	if (!GetResPath(strResName, strResPath))
	{
		g_pModuleManager->ErrorPrintf("GetResPath failed\n");
		return false;
	}
	
	strRet = strResPath + strResName + '/' + strRelPath;
	return true;
}

bool ResourcePathCache::GetThisResourceName(lua_State *luaVM, string &strRet)
{
	lua_getglobal(luaVM, "getThisResource");
	if (lua_pcall(luaVM, 0, 1, 0) != 0)
		return false;
	
	void *pRes = lua_touserdata(luaVM, -1);
	lua_pop(luaVM, 1);
	if (!pRes)
		return false;
	
	lua_getglobal(luaVM, "getResourceName");
	lua_pushlightuserdata(luaVM, pRes);
	if (lua_pcall(luaVM, 1, 1, 0) != 0)
		return false;

	const char *pszResName = lua_tostring(luaVM, -1);
	lua_pop(luaVM, 1);
	if (!pszResName)
		return false;
	
	strRet = pszResName;
	return true;
}

bool ResourcePathCache::Find(const string &strResName, string &strRet, const string &strPath)
{
    string strPattern = string("mods/deathmatch/resources/") + strPath + "*";
    
	//g_pModuleManager->Printf("Find(%s): %s\n", strResName.c_str(), strPattern.c_str());
    FileList List(strPattern);
    while (true)
	{
		string strName;
		int iRet = List.GetNextFile(strName);
		if (iRet < 0)
			break;
		
        //g_pModuleManager->Printf("Found %s (%d)\n", strName.c_str(), iRet);
		if (iRet == FileList::DIR && strName == strResName)
		{
			strRet = strPath;
            return true;
		}
        
        if (!strName.empty() && strName[0] == '[' && strName[strName.size() - 1] == ']')
        {
			bool bFound = Find(strResName, strRet, strPath + strName + "/");
			if(bFound)
				return true;
		}
	}
    return false;
}
