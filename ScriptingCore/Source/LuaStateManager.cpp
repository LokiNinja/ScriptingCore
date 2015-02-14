#include "../Headers/LuaStateManager.h"
#include "../Headers/Logger.h"
#include <assert.h>
#pragma comment(lib, "LuaPlus.lib")

LuaStateManager::LuaStateManager()
{
	m_luastate = nullptr;
}

LuaStateManager::~LuaStateManager()
{
	Shutdown();
}

IScriptState::ERROR LuaStateManager::Init()
{
	m_luastate = LuaPlus::LuaState::Create();
	if (!m_luastate)
	{
		LOGS("Failed to initialize LuaStateManager. m_luastate returned NULL");
		return SCRIPT_ROGUENULL;
	}
	m_luastate->OpenLibs();
	LuaPlus::LuaObject globals = m_luastate->GetGlobals();
	m_luastate->GetGlobals().RegisterDirect("DoFile", (*this), &LuaStateManager::DoFile);
	m_luastate->GetGlobals().RegisterDirect("DoString", (*this), &LuaStateManager::DoString);
	return SCRIPT_SUCCESS;
}

void LuaStateManager::Shutdown()
{
	if (m_luastate)
	{
		LuaPlus::LuaState::Destroy(m_luastate);
		m_luastate = nullptr;
	}
}

IScriptState::ERROR LuaStateManager::DoString(const char* chunk)
{
	int result = 0;
	if (!chunk || !m_luastate)
	{
		LOGS("Cannot pass string to state.  nullptr in DoString");
		return SCRIPT_ROGUENULL;
	}
	if (strlen(chunk) >= 1 && chunk[0] != '#')
	{
		result = m_luastate->DoString(chunk);
		if (result != 0)
		{
			LogError(result);
			return SCRIPT_BADCODE;
		}
		return SCRIPT_SUCCESS;
	}
	string buffer("print(");
	buffer += (chunk + 1);
	buffer += ")";
	result = m_luastate->DoString(buffer.c_str());
	if (result != 0)
	{
		LogError(result);
		return SCRIPT_BADCODE;
	}
	return SCRIPT_SUCCESS;
}

IScriptState::ERROR LuaStateManager::DoFile(const char* file)
{
	if (!m_luastate || !file)
	{
		LOGS("Cannot execute lua script.  nullptr in DoFile");
		return SCRIPT_ROGUENULL;
	}
	if (!VerifyExtension(file))
	{
		LOGS("File is not lua script file");
		return SCRIPT_BADFILE;
	}
	int result = m_luastate->DoFile(file);
	if (result != 0)
	{
		LogError(result);
		return SCRIPT_BADCODE;
	}
	return SCRIPT_SUCCESS;
}

LuaPlus::LuaObject LuaStateManager::GetGlobals()
{
	assert(m_luastate);
	return m_luastate->GetGlobals();
}

void LuaStateManager::LogError(int error)
{
	LuaPlus::LuaStackObject stackObj(m_luastate, -1);
	string errStr = stackObj.GetString();
	if (!errStr.empty())
	{
		LOGS(errStr);
		ClearStack();
	}
	else
	{
		LOGS("Unknown Lua Error");
	}
}

void LuaStateManager::ClearStack()
{
	if (m_luastate)
	{
		m_luastate->SetTop(0);
	}
}

bool LuaStateManager::VerifyExtension(const char* file)
{
	string buffer(file);
	int position = buffer.find_last_of('.');
	if (position == string::npos)
	{
		return false;
	}
	string extension = buffer.substr(position + 1, 3);
	if (extension != "lua")
		return false;
	return true;
}

LuaPlus::LuaState* LuaStateManager::GetState()
{
	if (m_luastate)
		return m_luastate;
	LOGS("LuaState is null");
	return nullptr;
}