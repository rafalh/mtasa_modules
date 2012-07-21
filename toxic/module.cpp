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

#include "module.h"
#include "luaimports.h"

ILuaModuleManager10 *g_pModuleManager = NULL;

// Initialisation function (module entrypoint)
MTAEXPORT bool InitModule(ILuaModuleManager10 *pManager, char *szModuleName, char *szAuthor, float *fVersion)
{
    g_pModuleManager = pManager;
    
    // Set the module info
    strncpy(szModuleName, MODULE_NAME, MAX_INFO_LENGTH);
    strncpy(szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH);
    (*fVersion) = MODULE_VERSION;
    
    ImportLua();
    
    return true;
}


MTAEXPORT void RegisterFunctions(lua_State *luaVM)
{
    if(g_pModuleManager && luaVM)
    {
        g_pModuleManager->RegisterFunction(luaVM, "fileFind", CFunctions::FileFind);
		g_pModuleManager->RegisterFunction(luaVM, "fileIsDirectory", CFunctions::FileIsDirectory);
    }
}


MTAEXPORT bool DoPulse(void)
{
    return true;
}

MTAEXPORT bool ShutdownModule(void)
{
    return true;
}
