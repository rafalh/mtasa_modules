#pragma once

#include "stdafx.h"

class ResourcePathCache
{
    public:
        bool GetResPath(const std::string &strResName, std::string &strRet);
		bool ParseResPath(lua_State *luaVM, const std::string &strPath, std::string &strRet);
		bool GetThisResourceName(lua_State *luaVM, std::string &strRet);
    
    private:
        std::map<std::string, std::string> m_List;
        
        bool Find(const std::string &strResName, std::string &strRet, const std::string &strPath = "");
};
