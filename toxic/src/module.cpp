#include "stdafx.h"
#include "module.h"
#include "CompressionUtils.h"
#include "FileUtils.h"
#include "CurlBinding.h"
#include "luaimports.h"

ILuaModuleManager10 *g_pModuleManager = NULL;

CompressionUtils g_CompressionUtils;
FileUtils g_FileUtils;
CurlBinding g_CurlBinding;

// Initialisation function (module entrypoint)
MTAEXPORT bool InitModule(ILuaModuleManager10 *pManager, char *szModuleName, char *szAuthor, float *fVersion)
{
    g_pModuleManager = pManager;
    // Set the module info
    strncpy(szModuleName, MODULE_NAME, MAX_INFO_LENGTH);
    strncpy(szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH);
    (*fVersion) = MODULE_VERSION;
    
    if (!ImportLua())
        return false;
    
    return true;
}

MTAEXPORT void RegisterFunctions(lua_State *luaVM)
{
    if (!g_pModuleManager || !luaVM) return;
    
    g_CompressionUtils.InitResource(luaVM);
    g_FileUtils.InitResource(luaVM);
    g_CurlBinding.InitResource(luaVM);
}

MTAEXPORT bool DoPulse(void)
{
    g_CurlBinding.Pulse();
    return true;
}

MTAEXPORT bool ShutdownModule(void)
{
    // nothing to do
    return true;
}

MTAEXPORT bool ResourceStopping(lua_State *luaVM)
{
    g_CompressionUtils.CleanupResource(luaVM);
    g_FileUtils.CleanupResource(luaVM);
    g_CurlBinding.CleanupResource(luaVM);
    return true;
}

MTAEXPORT bool ResourceStopped(lua_State *luaVM)
{
    // nothing to do
    return true;
}
