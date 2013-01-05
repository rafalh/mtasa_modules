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

class CFunctions;

#ifndef __CFUNCTIONS_H
#define __CFUNCTIONS_H

#include <stdio.h>
#include <string>

#include "module.h"
#include "ILuaModuleManager.h"

extern ILuaModuleManager10 *g_pModuleManager;

class CFunctions
{
    public:
        static int FileFind(lua_State* luaVM);
		static int FileIsDirectory(lua_State* luaVM);
		static int CompressJSON(lua_State* luaVM);
		static int zlibCompress(lua_State* luaVM);
		static int zlibUncompress(lua_State* luaVM);
};
#endif
