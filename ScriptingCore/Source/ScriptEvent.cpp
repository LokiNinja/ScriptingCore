#include "../Headers/ScriptEvent.h"
#include "../Headers/Logger.h"
#include "../Headers/LuaStateManager.h"
#include <LuaPlus.h>

unordered_map<unsigned long, ScriptEventCreator> ScriptEvent::m_creators;

ScriptEvent::ScriptEvent() : EventData(0.f)
{
}

bool ScriptEvent::RegisterEventCreator(unsigned long type, ScriptEventCreator creator)
{
	auto it = m_creators.find(type);
	if (it == m_creators.end())
	{
		m_creators[type] = creator;
		return true;
	}
	LOGS("Event creator for id " + to_string(static_cast<long long>(type)) + " already exists");
	return false;
}

bool ScriptEvent::RegisterType(const char* key, unsigned long type)
{
	LuaPlus::LuaObject eventTable = LUASTATE->GetGlobals().GetByName("EventTypes");
	if (eventTable.IsNil())
	{
		eventTable = LUASTATE->GetGlobals().CreateTable("EventTypes");
	}
	if (!eventTable.IsTable() || !eventTable[key].IsNil())
	{
		LOGS("Could not create or register event with id " + to_string(static_cast<long long>(type)) + "  with Lua");
		return false;
	}
	eventTable.SetInteger(key, type);
	return true;
}

ScriptEvent* ScriptEvent::CreateEvent(unsigned long eventType)
{
	auto it = m_creators.find(eventType);
	if (it == m_creators.end())
	{
		LOGS("Could not find event creator for event " + to_string(static_cast<long long>(eventType)));
		return nullptr;
	}
	return it->second();
}

LuaPlus::LuaObject ScriptEvent::GetData() const
{
	return m_eventData;
}

void ScriptEvent::SetData(LuaPlus::LuaObject data)
{
	m_eventData = data;
}