#include "stdafx.h"
#include "module.h"
#include "CompressionUtils.h"
#include "LuaHelper.h"

using namespace std;

void CompressionUtils::InitResource(lua_State *luaVM)
{
    g_pModuleManager->RegisterFunction(luaVM, "compressJSON", CompressJSON);
    g_pModuleManager->RegisterFunction(luaVM, "zlibCompress", zlibCompress);
    g_pModuleManager->RegisterFunction(luaVM, "zlibUncompress", zlibUncompress);
}

int CompressionUtils::CompressJSON(lua_State *luaVM)
{
    LuaHelper lua(luaVM);
    string data = lua.ReadArg<string>();
    
    bool bStr = false, bSlash = false;
    string strRet = "";
    strRet.reserve(data.size());
    
    for (unsigned i = 0; i < data.size(); ++i)
    {
        char ch = data[i];
        switch (ch)
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
    
    return lua.Push(strRet);
}

int CompressionUtils::zlibCompress(lua_State *luaVM)
{
    LuaHelper lua(luaVM);
    string input = lua.ReadArg<string>();
    
    uLongf cbOutput = compressBound(input.size());
    char *pOutput = new char[cbOutput + 4];
    *((uint32_t*)pOutput) = input.size(); // save length at the beggining
    if (compress2((Bytef*)pOutput + 4, &cbOutput, (Bytef*)input.data(), input.size(), Z_BEST_COMPRESSION) != Z_OK)
    {
        delete[] pOutput;
        g_pModuleManager->ErrorPrintf("compress failed\n");
        return lua.Push(false);
    }
    
    string strOutput(pOutput, cbOutput + 4);
    delete[] pOutput;
    return lua.Push(strOutput);
}

int CompressionUtils::zlibUncompress(lua_State *luaVM)
{
    LuaHelper lua(luaVM);
    string input = lua.ReadArg<string>();
    
    if (input.size() < 4)
    {
        g_pModuleManager->ErrorPrintf("Invalid zlibDecompress data\n");
        return lua.Push(false);
    }
    
    uLongf cbOutput = *((uint32_t*)input.data());
    char *pOutput = new char[cbOutput];
    if (uncompress((Bytef*)pOutput, &cbOutput, (Bytef*)input.data() + 4, input.size() - 4) != Z_OK)
    {
        delete[] pOutput;
        
        g_pModuleManager->ErrorPrintf("uncompress failed\n");
        return lua.Push(false);
    }
    
    string strOutput(pOutput, cbOutput);
    delete[] pOutput;
    return lua.Push(strOutput);
}
