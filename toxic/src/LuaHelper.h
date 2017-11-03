#ifndef LUAHELPER_H
#define LUAHELPER_H

#include "stdafx.h"


class LuaHelper;

class LuaHelper
{
    private:
        template <typename... T> struct LuaPusher
        {
            static void Push(lua_State *luaVM, T... Args);
        };
        
        template<typename T> struct Type
        {
            static void Push(lua_State *luaVM, T Val);
            static void Get(lua_State *luaVM, int Pos);
            static void GetOpt(lua_State *luaVM, int Pos, T DefVal);
        };

    public:
		LuaHelper(lua_State *luaVM) :
			m_LuaVM(luaVM), m_Pos(1) {}
        
		template<typename T>
		T GetGlobal(const char *Name)
		{
			lua_getglobal(m_LuaVM, Name);
			return Type<T>::Get(m_LuaVM, -1);
		}

        template<typename T>
        void SetGlobal(const char *Name, T Val)
        {
            Type<T>::Push(m_LuaVM, Val);
            lua_setglobal(m_LuaVM, Name);
        }
        
        template<typename T>
        T ReadArg()
        {
            return Type<T>::Get(m_LuaVM, m_Pos++);
        }
        
        template<typename T>
        T ReadArg(T DefVal)
        {
            return Type<T>::GetOpt(m_LuaVM, m_Pos++, DefVal);
        }
        
        template<typename... T> int Push(T... Args)
        {
            LuaPusher<T...>::Push(m_LuaVM, Args...);
            return sizeof...(Args);
        }

        template<typename... T> bool Call(const char *FuncName, T... Args)
        {
            lua_getglobal(m_LuaVM, FuncName);
            
			LuaPusher<T...>::Push(m_LuaVM, Args...);
            int iret = lua_pcall(m_LuaVM, sizeof...(Args), 0, 0);
            if (iret == LUA_ERRRUN || iret == LUA_ERRMEM)
            {
                const char *Error = lua_tostring(m_LuaVM, -1);
                fprintf(stderr, "Call failed: %s\n", Error);
                lua_pop(m_LuaVM, -1);
                return false; // the function call failed
            }
            
            return true;
        }

        template<typename R, typename... T> R CallAndGetResult(const char *FuncName, T... Args)
        {
            lua_getglobal(m_LuaVM, FuncName);
            
            LuaPusher<T...>::Push(m_LuaVM, Args...);
            int iret = lua_pcall(m_LuaVM, sizeof...(Args), 1, 0);
            if (iret == LUA_ERRRUN || iret == LUA_ERRMEM)
            {
                const char *Error = lua_tostring(m_LuaVM, -1);
                fprintf(stderr, "Call failed: %s\n", Error);
                lua_pop(m_LuaVM, -1);
                return R(); // the function call failed
            }

            // Pop result
            R result = Type<R>::Get(m_LuaVM, -1);
            lua_pop(m_LuaVM, 1);

            return result;
        }
        
    protected:
        template<typename T> T Get(int Pos);
        template<typename T> T GetOpt(int Pos, T DefVal);
        
    private:
        lua_State *m_LuaVM;
        int m_Pos;
};

template <typename HeadT, typename... T> struct LuaHelper::LuaPusher<HeadT, T...>
{
    static void Push(lua_State *luaVM, HeadT HeadArg, T... TailArgs)
    {
        Type<HeadT>::Push(luaVM, HeadArg);
        LuaPusher<T...>::Push(luaVM, TailArgs...);
    }
};

template<>
struct LuaHelper::LuaPusher<>
{
    static void Push(lua_State *luaVM) {}
};

template<> struct LuaHelper::Type<int>
{
    static void Push(lua_State *luaVM, int Val)
    {
        lua_pushinteger(luaVM, Val);
    }
    
    static int Get(lua_State *luaVM, int Pos)
    {
        return luaL_checkint(luaVM, Pos);
    }
    
    static int GetOpt(lua_State *luaVM, int Pos, int DefVal)
    {
        return luaL_optint(luaVM, Pos, DefVal);
    }
};

template<> struct LuaHelper::Type<bool>
{
    static void Push(lua_State *luaVM, bool Val)
    {
        lua_pushboolean(luaVM, Val);
    }
    
    static bool Get(lua_State *luaVM, int Pos)
    {
		return lua_toboolean(luaVM, Pos) != 0;
    }
    
    static bool GetOpt(lua_State *luaVM, int Pos, bool DefVal)
    {
        if (lua_isnil(luaVM, Pos))
            return DefVal;
        else
			return lua_toboolean(luaVM, Pos) != 0;
    }
};

template<> struct LuaHelper::Type<std::string>
{
    static void Push(lua_State *luaVM, std::string Val)
    {
        lua_pushlstring(luaVM, Val.data(), Val.size());
    }
    
    static std::string Get(lua_State *luaVM, int Pos)
    {
        size_t cbData = 0;
        const char *pszData = luaL_checklstring(luaVM, Pos, &cbData);
        return std::string(pszData, cbData);
    }
    
    static std::string GetOpt(lua_State *luaVM, int Pos, std::string DefVal)
    {
        size_t cbData = 0;
        const char *pszData = luaL_optlstring(luaVM, Pos, DefVal.c_str(), &cbData);
        return std::string(pszData, cbData);
    }
};

template<> struct LuaHelper::Type<const char*> : public LuaHelper::Type<std::string>
{
	static void Push(lua_State *luaVM, const char *Val)
	{
		lua_pushstring(luaVM, Val);
	}
};

template<typename T> struct LuaHelper::Type<T*>
{
    static void Push(lua_State *luaVM, T *Val)
    {
        lua_pushlightuserdata(luaVM, (void*)Val);
    }
    
    static T *Get(lua_State *luaVM, int Pos)
    {
		return lua_touserdata(luaVM, Pos);
    }
    
    static T *GetOpt(lua_State *luaVM, int Pos, T *DefVal)
    {
		if (lua_islightuserdata(luaVM, Pos))
			return lua_touserdata(luaVM, Pos);
		else
			return DefVal;
    }
};

template<> struct LuaHelper::Type<std::nullptr_t>
{
    static void Push(lua_State *luaVM, std::nullptr_t Val)
    {
        lua_pushnil(luaVM);
    }
    
    static std::nullptr_t Get(lua_State *luaVM, int Pos)
    {
        return nullptr;
    }
    
    static std::nullptr_t GetOpt(lua_State *luaVM, int Pos, std::nullptr_t DefVal)
    {
        return nullptr;
    }
};

#endif // LUAHELPER_H
