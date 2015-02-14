#pragma once
#include <set>
using namespace std;

class LuaEventCallback;

class LuaCallbackManager
{
	set<LuaEventCallback*>	m_callbacks;
public:
	LuaCallbackManager() {}
	~LuaCallbackManager();
	void AddListener(LuaEventCallback* listener);
	void RemoveListener(LuaEventCallback* listener);
};