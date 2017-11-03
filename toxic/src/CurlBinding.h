#pragma once

#include "stdafx.h"
#include "HandleTable.h"

class CurlBinding
{
public:
	CurlBinding();
	void InitResource(lua_State *luaVM);
	void CleanupResource(lua_State *luaVM);
	void Pulse();
	
	static int CurlEscape(lua_State *luaVM);
	static int CurlInit(lua_State *luaVM);
	static int CurlSetopt(lua_State *luaVM);
	static int CurlPerform(lua_State *luaVM);
	static int CurlGetInfo(lua_State *luaVM);
	static int CurlClose(lua_State *luaVM);

private:
	const static uint16_t HANDLE_PREFIX = 0xC041;
	const static uint16_t REQUESTS_PER_RESOURCE_LIMIT = 100;

	struct Request;
	typedef HandleTable<Request> ResourceData;
	typedef ResourceData::Handle Handle;

	struct Request
	{
		Request(lua_State *luaVM) :
			luaVM(luaVM) {}
		~Request();

		lua_State *luaVM;
		ResourceData::Handle h;
		CURL *curl = nullptr;
		bool started = false;
	};

	struct OptionDesc
	{
		enum OptType { T_STR, T_INT, T_BOOL };

		OptionDesc() {}
		OptionDesc(CURLoption opt, OptType t) : opt(opt), type(t) {}

		CURLoption opt;
		OptType type;
	};

	struct InfoOptDesc
	{
		enum OptType { T_INT };

		InfoOptDesc() {}
		InfoOptDesc(CURLINFO opt, OptType t) : opt(opt), type(t) {}

		CURLINFO opt;
		OptType type;
	};
	
	static CURLM *m_CurlMulti;
	static std::unordered_map<lua_State*, std::unique_ptr<ResourceData>> m_Resources;
	static std::unordered_map<std::string, OptionDesc> m_OptionMapping;
	static std::unordered_map<std::string, InfoOptDesc> m_InfoOptMapping;
	
	static Request *FindReq(lua_State *luaVM, Handle h);
	static size_t HeaderCallback(void *contents, size_t size, size_t nmemb, void *userp);
	static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
};

