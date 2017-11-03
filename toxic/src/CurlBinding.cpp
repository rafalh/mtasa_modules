#include "stdafx.h"
#include "CurlBinding.h"
#include "module.h"
#include "LuaHelper.h"

using namespace std;

std::unordered_map<lua_State*, std::unique_ptr<CurlBinding::ResourceData>> CurlBinding::m_Resources;
std::unordered_map<std::string, CurlBinding::OptionDesc> CurlBinding::m_OptionMapping;
std::unordered_map<std::string, CurlBinding::InfoOptDesc> CurlBinding::m_InfoOptMapping;

CURLM *CurlBinding::m_CurlMulti;

CurlBinding::CurlBinding()
{
	m_CurlMulti = curl_multi_init();

	m_OptionMapping["CURLOPT_FOLLOWLOCATION"] = OptionDesc(CURLOPT_FOLLOWLOCATION, OptionDesc::T_BOOL);
	m_OptionMapping["CURLOPT_HEADER"] = OptionDesc(CURLOPT_HEADER, OptionDesc::T_STR);
	m_OptionMapping["CURLOPT_NOBODY"] = OptionDesc(CURLOPT_NOBODY, OptionDesc::T_BOOL);
	m_OptionMapping["CURLOPT_POST"] = OptionDesc(CURLOPT_POST, OptionDesc::T_BOOL);
	m_OptionMapping["CURLOPT_CONNECTTIMEOUT"] = OptionDesc(CURLOPT_CONNECTTIMEOUT, OptionDesc::T_INT);
	m_OptionMapping["CURLOPT_MAXREDIRS"] = OptionDesc(CURLOPT_MAXREDIRS, OptionDesc::T_INT);
	m_OptionMapping["CURLOPT_TIMEOUT"] = OptionDesc(CURLOPT_TIMEOUT, OptionDesc::T_INT);
	m_OptionMapping["CURLOPT_COOKIE"] = OptionDesc(CURLOPT_COOKIE, OptionDesc::T_STR);
	m_OptionMapping["CURLOPT_POSTFIELDS"] = OptionDesc(CURLOPT_POSTFIELDS, OptionDesc::T_STR);
	m_OptionMapping["CURLOPT_URL"] = OptionDesc(CURLOPT_URL, OptionDesc::T_STR);

	m_InfoOptMapping["CURLINFO_RESPONSE_CODE"] = InfoOptDesc(CURLINFO_RESPONSE_CODE, InfoOptDesc::T_INT);
}

void CurlBinding::InitResource(lua_State *luaVM)
{
	g_pModuleManager->RegisterFunction(luaVM, "curlEscape", CurlEscape);
	g_pModuleManager->RegisterFunction(luaVM, "curlInit", CurlInit);
	g_pModuleManager->RegisterFunction(luaVM, "curlSetopt", CurlSetopt);
	g_pModuleManager->RegisterFunction(luaVM, "curlPerform", CurlPerform);
	g_pModuleManager->RegisterFunction(luaVM, "curlGetInfo", CurlGetInfo);
	g_pModuleManager->RegisterFunction(luaVM, "curlClose", CurlClose);
	
	LuaHelper lua(luaVM);
	lua.Call("addEvent", "onCurlHeader");
	lua.Call("addEvent", "onCurlData");
	lua.Call("addEvent", "onCurlDone");
}

void CurlBinding::CleanupResource(lua_State *luaVM)
{
	// Note: all requests are deleted because of unique_ptr
	m_Resources.erase(luaVM);
}
static bool inPulse = false;
void CurlBinding::Pulse()
{
	int stillRunning;
	curl_multi_perform(m_CurlMulti, &stillRunning);

	int msgsLeft;
	CURLMsg *msg;
	while ((msg = curl_multi_info_read(m_CurlMulti, &msgsLeft)))
	{
		if (msg->msg == CURLMSG_DONE)
		{
			bool found = false;
			for (auto it = m_Resources.begin(); !found && it != m_Resources.end(); ++it)
			{
                lua_State *luaVM = it->first;
				auto &handleTbl = it->second;
				for (auto it2 = handleTbl->begin(); it2 != handleTbl->end(); ++it2)
				{
					auto &req = it2->second;
					if (req->curl == msg->easy_handle)
					{
                        auto reqKey = it2->first;
						LuaHelper lua(req->luaVM);
						void *resRoot = lua.GetGlobal<void*>("resourceRoot");
                        inPulse = true;
						lua.Call("triggerEvent", "onCurlDone", resRoot, req->h, (int)msg->data.result);
                        inPulse = false;
                        // Dont use iterators here, they may be invalid
						//handleTbl->erase(reqKey);
						found = true;
						break;
					}
				}
			}
		}
	}
}

int CurlBinding::CurlInit(lua_State *luaVM)
{
    LuaHelper lua(luaVM);
    string url = lua.ReadArg<string>("");
    
	auto it = m_Resources.find(luaVM);
	if (it == m_Resources.end())
    {
		it = m_Resources.insert(std::make_pair(luaVM, std::unique_ptr<ResourceData>(new ResourceData(HANDLE_PREFIX)))).first;
    }

	auto &handleTbl = it->second;
	if (handleTbl->size() >= REQUESTS_PER_RESOURCE_LIMIT)
	{
		g_pModuleManager->ErrorPrintf("curlInit: Requests per resource limit of %d reached\n", REQUESTS_PER_RESOURCE_LIMIT);
		lua.Push(false);
		return 1;
	}

	std::unique_ptr<Request> req(new Request(luaVM));
	req->curl = curl_easy_init();
	if (!req->curl)
	{
		g_pModuleManager->ErrorPrintf("curlInit: curl_easy_init failed\n");
		lua.Push(false);
		return 1;
	}

	curl_easy_setopt(req->curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
	curl_easy_setopt(req->curl, CURLOPT_HEADERDATA, req.get());
	curl_easy_setopt(req->curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(req->curl, CURLOPT_WRITEDATA, req.get());

//if (inPulse)
//     return 0;

	if (!url.empty())
		curl_easy_setopt(req->curl, CURLOPT_URL, url.c_str());

	Handle h = handleTbl->GenerateHandle();
	req->h = h;
	handleTbl->Insert(h, std::move(req));

	// Note: req is a null pointer here
	lua.Push(h);
    return 1;
}

int CurlBinding::CurlEscape(lua_State *luaVM)
{
	LuaHelper lua(luaVM);
	Handle h = lua.ReadArg<Handle>();
	string str = lua.ReadArg<string>();

	Request *req = FindReq(luaVM, h);
	if (!req)
	{
		g_pModuleManager->ErrorPrintf("CurlEscape: Cannot find curl request\n");
		lua.Push(false);
		return 1;
	}
	
	char *output = curl_easy_escape(req->curl, str.data(), str.size());
	if (!output)
	{
		g_pModuleManager->ErrorPrintf("CurlEscape: curl_easy_escape failed\n");
		lua.Push(false);
		return 1;
	}

	lua.Push((const char*)output);
	curl_free(output);
	return 1;
}

int CurlBinding::CurlSetopt(lua_State *luaVM)
{
    LuaHelper lua(luaVM);
	Handle h = lua.ReadArg<Handle>();
	string optName = lua.ReadArg<string>();

	Request *req = FindReq(luaVM, h);
	if (!req)
	{
		g_pModuleManager->ErrorPrintf("curlSetopt: Cannot find curl request\n");
		lua.Push(false);
		return 1;
	}

	auto it = m_OptionMapping.find(optName);
	if (it == m_OptionMapping.end())
	{
		g_pModuleManager->ErrorPrintf("curlSetopt: Invalid option name\n");
		lua.Push(false);
		return 1;
	}
	OptionDesc &optDesc = it->second;
	CURLcode ret = CURLE_OK;
	switch (optDesc.type)
	{
	case OptionDesc::T_STR:
	{
		string val = lua.ReadArg<string>();
		ret = curl_easy_setopt(req->curl, optDesc.opt, val.c_str());
		break;
	}
	case OptionDesc::T_INT:
	{
		int val = lua.ReadArg<int>();
		ret = curl_easy_setopt(req->curl, optDesc.opt, val);
		break;
	}
	case OptionDesc::T_BOOL:
	{
		bool val = lua.ReadArg<bool>();
		ret = curl_easy_setopt(req->curl, optDesc.opt, val ? 1L : 0L);
		break;
	}
	}

	if (ret != CURLE_OK)
		g_pModuleManager->ErrorPrintf("curlSetopt: curl_easy_setopt failed\n");
	
	lua.Push(ret == CURLE_OK);
    return 1;
}

int CurlBinding::CurlPerform(lua_State *luaVM)
{ //if (inPulse) return 0;
    LuaHelper lua(luaVM);

	Handle h = lua.ReadArg<Handle>();
	Request *req = FindReq(luaVM, h);
	if (!req)
	{
		g_pModuleManager->ErrorPrintf("curlPerform: Cannot find curl request\n");
		lua.Push(false);
		return 1;
	}

	CURLMcode ret = CURLM_OK;
	if (!req->started)
	{
		req->started = true;
		ret = curl_multi_add_handle(m_CurlMulti, req->curl);
	}

	if (ret != CURLM_OK)
		g_pModuleManager->ErrorPrintf("curlPerform: curl_multi_add_handle failed\n");

	lua.Push(ret == CURLM_OK);
    return 1;
}

int CurlBinding::CurlGetInfo(lua_State *luaVM)
{
	LuaHelper lua(luaVM);
	Handle h = lua.ReadArg<Handle>();
	Request *req = FindReq(luaVM, h);
	if (!req)
	{
		g_pModuleManager->ErrorPrintf("curlGetInfo: Cannot find curl request\n");
		lua.Push(false);
		return 1;
	}

	CURLcode ret;

	string key = lua.ReadArg<string>();
	auto it = m_InfoOptMapping.find(key);
	if (it == m_InfoOptMapping.end())
	{
		g_pModuleManager->ErrorPrintf("curlGetInfo: Unsupported curlGetInfo key: %s\n", key.c_str());
		lua.Push(false);
	}
	else
	{
		const InfoOptDesc &optDesc = it->second;
		long val;
		ret = curl_easy_getinfo(req->curl, optDesc.opt, &val);
		lua.Push<int>(val);
	}
	
	return 1;
}

int CurlBinding::CurlClose(lua_State *luaVM)
{
    LuaHelper lua(luaVM);

	Handle h = lua.ReadArg<Handle>();
	Request *req = FindReq(luaVM, h);
	if (!req)
	{
		g_pModuleManager->ErrorPrintf("ml_toxic: Cannot find curl request\n");
		lua.Push(false);
		return 1;
	}

	auto it = m_Resources.find(req->luaVM);
	auto &resData = it->second;
	resData->Remove(req->h);

    lua.Push(true);
    return 1;
}

CurlBinding::Request *CurlBinding::FindReq(lua_State *luaVM, Handle h)
{
	auto it = m_Resources.find(luaVM);
	if (it == m_Resources.end())
		return nullptr;

	auto &resData = it->second;
	return resData->Get(h);
}

size_t CurlBinding::HeaderCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	Request *req = (Request*)userp;

	LuaHelper lua(req->luaVM);
	void *resRoot = lua.GetGlobal<void*>("resourceRoot");
	lua.Call("triggerEvent", "onCurlHeader", resRoot, req->h, string((char*)contents, realsize));
	bool cancelled = lua.CallAndGetResult<bool>("wasEventCancelled");
	if (cancelled)
		return 0;
	return realsize;
}

size_t CurlBinding::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	Request *req = (Request*)userp;

	LuaHelper lua(req->luaVM);
	void *resRoot = lua.GetGlobal<void*>("resourceRoot");
	lua.Call("triggerEvent", "onCurlData", resRoot, req->h, string((char*)contents, realsize));
	bool cancelled = lua.CallAndGetResult<bool>("wasEventCancelled");
	if (cancelled)
		return 0;
	return realsize;
}

CurlBinding::Request::~Request()
{
	if (started)
		curl_multi_remove_handle(m_CurlMulti, curl);
	if (curl)
		curl_easy_cleanup(curl);
}
