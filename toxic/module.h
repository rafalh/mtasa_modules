/*********************************************************
*
*  Multi Theft Auto: San Andreas - Deathmatch
*
*  ml_base, External lua add-on module
*
*  Copyright � 2003-2008 MTA.  All Rights Reserved.
*
*  Grand Theft Auto is � 2002-2003 Rockstar North
*
*  THE FOLLOWING SOURCES ARE PART OF THE MULTI THEFT
*  AUTO SOFTWARE DEVELOPMENT KIT AND ARE RELEASED AS
*  OPEN SOURCE FILES. THESE FILES MAY BE USED AS LONG
*  AS THE DEVELOPER AGREES TO THE LICENSE THAT IS
*  PROVIDED WITH THIS PACKAGE.
*
*********************************************************/

/** MODULE SPECIFIC INFORMATION **/
#define MODULE_NAME         "Toxic Module"
#define MODULE_AUTHOR       "rafalh"
#define MODULE_VERSION      1.0

#include <string.h>
#include "CFunctions.h"
#include "include/ILuaModuleManager.h"

#ifdef WIN32
    #define MTAEXPORT extern "C" __declspec(dllexport)
#else
    #define MTAEXPORT extern "C"
#endif