#pragma once

#include "stdafx.h"

/** MODULE SPECIFIC INFORMATION **/
#define MODULE_NAME         "Toxic Module"
#define MODULE_AUTHOR       "rafalh"
#define MODULE_VERSION      1.2f

#ifdef _WIN32
    #define MTAEXPORT extern "C" __declspec(dllexport)
#else
    #define MTAEXPORT extern "C"
#endif

extern ILuaModuleManager10 *g_pModuleManager;
