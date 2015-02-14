#pragma once
#include "IScriptState.h"
#include "ISingleton.h"
#include <LuaPlus.h>


class LuaStateManager : public IScriptState, public ISingleton < LuaStateManager >
{
	SET_SINGLETON(LuaStateManager);
public:
	~LuaStateManager();
	ERROR Init() override;
	ERROR DoString(const char*) override;
	ERROR DoFile(const char*) override;
	LuaPlus::LuaObject GetGlobals();
	LuaPlus::LuaState* GetState();
private:
	LuaPlus::LuaState*		m_luastate;
	//Singleton
	LuaStateManager();
	LuaStateManager(const LuaStateManager&);
	LuaPlus::LuaState operator=(const LuaStateManager&);

	virtual void Shutdown() override;
	void LogError(int);
	void ClearStack();
	//Verifies files for DoFile have '.lua' extension
	bool VerifyExtension(const char* file);
};

#define LUASTATE LuaStateManager::GetInstance()