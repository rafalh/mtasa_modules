#include "stdafx.h"
#include "FileUtils.h"
#include "FileList.h"
#include "module.h"
#include "LuaHelper.h"

using namespace std;

const string RESOURCES_PATH = "mods/deathmatch/resources/";

inline bool FileExists(const string &path)
{
#ifdef _WIN32
    struct _stat32 info;
    int ret = _stat32(path.c_str(), &info);
#else
    struct stat info;
    int ret = stat(path.c_str(), &info);
#endif
    return ret == 0;
}

void FileUtils::InitResource(lua_State *luaVM)
{
    g_pModuleManager->RegisterFunction(luaVM, "fileFind", FileFind);
    g_pModuleManager->RegisterFunction(luaVM, "fileModTime", FileModTime);
}

int FileUtils::FileFind(lua_State *luaVM)
{
    const char *pszPattern = luaL_optstring(luaVM, 1, "*");
    const char *pszType = luaL_optstring(luaVM, 2, "all");

    bool bDirs = !strcmp(pszType, "all") || !strcmp(pszType, "directory");
    bool bFiles = !strcmp(pszType, "all") || !strcmp(pszType, "file");

    string strPattern;
    if (!ParseResPath(luaVM, pszPattern, strPattern))
    {
        lua_pushboolean(luaVM, false);
        g_pModuleManager->ErrorPrintf("fileFind: ParseResPath failed\n");
        return 1;
    }

    if (strPattern.find("..") != strPattern.npos)
    {
        lua_pushboolean(luaVM, false);
        g_pModuleManager->ErrorPrintf("fileFind: Invalid path\n");
        return 1;
    }

    strPattern = RESOURCES_PATH + strPattern;
    //g_pModuleManager->Printf("Find %s %u %u\n", strPattern.c_str(), bFiles, bDirs);

    lua_newtable(luaVM);
    FileList List(strPattern);
    unsigned i = 0;
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

int FileUtils::FileModTime(lua_State *luaVM)
{
    LuaHelper lua(luaVM);
    string relativePath = lua.ReadArg<string>();
    string fullPath;

    if (!ParseResPath(luaVM, relativePath, fullPath))
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

    fullPath = RESOURCES_PATH + fullPath;

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

bool FileUtils::ParseResPath(lua_State* luaVM, const string &strPath, string &strRet)
{
    string strResName;
    string strRelPath;
    void *pResource;

    LuaHelper lua(luaVM);

    if (strPath[0] != ':')
    {
        pResource = lua.CallAndGetResult<void*>("getThisResource");
        if (!pResource)
        {
            g_pModuleManager->ErrorPrintf("[ml_toxic] getThisResource failed\n");
            return false;
        }

        strResName = lua.CallAndGetResult<string>("getResourceName", pResource);
        if (strResName.empty())
        {
            g_pModuleManager->ErrorPrintf("[ml_toxic] getResourceName failed\n");
            return false;
        }
        
        strRelPath = strPath;
    }
    else
    {
        size_t Pos = strPath.find('/');
        if (Pos == string::npos)
        {
            g_pModuleManager->ErrorPrintf("[ml_toxic] Invalid path\n");
            return false;
        }
        
        strResName = strPath.substr(1, Pos - 1);
        pResource = lua.CallAndGetResult<void*>("getResourceFromName", strResName);
        if (!pResource)
        {
            g_pModuleManager->ErrorPrintf("[ml_toxic] getResourceFromName failed\n");
            return false;
        }

        strRelPath = strPath.substr(Pos + 1);
    }

    string strOrgPath = lua.CallAndGetResult<string>("getResourceOrganizationalPath", pResource);
    string strResPath;
    if (!strOrgPath.empty())
        strResPath += strOrgPath + "/";
    strResPath += strResName;

    if (!FileExists(RESOURCES_PATH + strResPath + "/meta.xml"))
    {
        g_pModuleManager->ErrorPrintf("[ml_toxic] meta.xml not found %s\n", strResPath.c_str());
        return false;
    }

    strRet = strResPath + '/' + strRelPath;
    return true;
}
