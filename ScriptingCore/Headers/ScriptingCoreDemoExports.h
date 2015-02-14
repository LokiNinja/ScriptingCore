#pragma once
#include "IScriptExports.h"
#include <LuaPlus.h>

class ScriptEvent;
class LuaCallbackManager;


class ScriptingCoreDemoExports : public IScriptExports
{
public:
	ScriptingCoreDemoExports();
	bool Register() override;
	static bool FireEvent(unsigned long type, LuaPlus::LuaObject data);
	static bool FireImmediate(unsigned long type, LuaPlus::LuaObject data);
	static void Log(const char*);
	static unsigned long RegisterCallback(unsigned long type, LuaPlus::LuaObject callback);
	static void UnregisterCallback(unsigned long type);
	static unsigned long CreateActor(const char* type, int pos, float orientation);
	static LuaPlus::LuaObject GetObject(unsigned long);
	void Unregister() override;
private:
	static ScriptEvent* BuildEvent(unsigned long type, LuaPlus::LuaObject object);
	static LuaCallbackManager*	m_luaCallbacks;
};