#include "../Headers/LuaCallbackManager.h"
#include "../Headers/LuaEventCallback.h"
#include "../Headers/EventManager.h"
#include "../Headers/Logger.h"

LuaCallbackManager::~LuaCallbackManager()
{
	auto it = m_callbacks.begin();
	while (it != m_callbacks.end())
	{
		LuaEventCallback * current = *it;
		EVENTS->Unregister(current->GetDelegate(), current->GetType());
		delete current;
		current = nullptr;
		it = m_callbacks.erase(it);
	}
}

void LuaCallbackManager::AddListener(LuaEventCallback* callback)
{
	auto it = m_callbacks.find(callback);
	if (it == m_callbacks.end())
	{
		m_callbacks.insert(callback);
		EVENTS->Register(callback->GetDelegate(), callback->GetType());
	}
	else
	{
		LOGS("FAILED to AddListenter");
	}
}

void LuaCallbackManager::RemoveListener(LuaEventCallback* callback)
{
	auto it = m_callbacks.find(callback);
	if (it != m_callbacks.end())
	{
		EVENTS->Unregister(callback->GetDelegate(), callback->GetType());
		delete *it;
		m_callbacks.erase(it);
	}
	else
	{
		LOGS("Listener doesn't exist");
	}
}