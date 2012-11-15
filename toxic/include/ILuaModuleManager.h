/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        publicsdk/include/ILuaModuleManager.h
*  PURPOSE:     Lua dynamic module interface
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

// INTERFACE for Lua dynamic modules

#ifndef __ILUAMODULEMANAGER_H
#define __ILUAMODULEMANAGER_H

#define MAX_INFO_LENGTH 128

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}
#include <string>

#if defined(WIN32) && defined(__GNUC__)
#define THISCALL __thiscall // MTA uses on Windows MS standard for calling methods
#else
#define THISCALL
#endif

#ifndef __CChecksum_H
class CChecksum
{
public:
    unsigned long ulCRC;
    unsigned char mD5 [16];
};
#endif

/* Interface for modules until DP2.3 */
class ILuaModuleManager
{
public:
	virtual void                      ErrorPrintf             ( const char* szFormat, ... ) = 0;
	virtual void                      DebugPrintf             ( lua_State * luaVM, const char* szFormat, ... ) = 0;
	virtual void                      Printf                  ( const char* szFormat, ... ) = 0;

	virtual bool __thiscall           RegisterFunction        ( lua_State * luaVM, const char *szFunctionName, lua_CFunction Func ) = 0;
    virtual bool __thiscall           GetResourceName         ( lua_State * luaVM, std::string &strName ) = 0;
	virtual CChecksum __thiscall      GetResourceMetaChecksum ( lua_State * luaVM ) = 0;
	virtual CChecksum __thiscall      GetResourceFileChecksum ( lua_State * luaVM, const char* szFile ) = 0;
};

/* Interface for modules until 1.0 */
class ILuaModuleManager10 : public ILuaModuleManager
{
public:
    virtual unsigned long __thiscall  GetVersion              ( ) = 0;
    virtual const char* __thiscall    GetVersionString        ( ) = 0;
    virtual const char* __thiscall    GetVersionName          ( ) = 0;
    virtual unsigned long __thiscall  GetNetcodeVersion       ( ) = 0;
    virtual const char* __thiscall    GetOperatingSystemName  ( ) = 0;

    virtual lua_State* __thiscall     GetResourceFromName     ( const char* szResourceName ) = 0;
};

#endif
