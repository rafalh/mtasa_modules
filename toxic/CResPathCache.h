#ifndef CRESPATHCACHE_H
#define CRESPATHCACHE_H

#include <string>
#include <map>
#include "include/ILuaModuleManager.h"

class CResPathCache
{
    public:
        bool GetResPath(const std::string &strResName, std::string &strRet);
		bool ParseResPath(lua_State* luaVM, const std::string &strPath, std::string &strRet);
		bool GetThisResourceName(lua_State* luaVM, std::string &strRet);
    
    private:
        std::map<std::string, std::string> m_List;
        
        bool Find(const std::string &strResName, std::string &strRet, const std::string &strPath = "");
};

#endif // CRESPATHCACHE_H
