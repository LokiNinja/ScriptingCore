#pragma once
#include "EventData.h"
#include <unordered_map>
#include "Globals.h"
#include <LuaPlus.h>
using namespace std;

class ScriptEvent;

typedef ScriptEvent*(*ScriptEventCreator)();

#define REGISTER_EVENT(eventClass) \
	ScriptEvent::RegisterType(#eventClass, eventClass::s_eventType); \
	ScriptEvent::RegisterEventCreator(eventClass::s_eventType, &eventClass::CreateEvent);

#define EXPORT_TO_SCRIPT(eventClass) \
	public: \
		static ScriptEvent* CreateEvent() \
		{ \
			return NEW eventClass(); \
		}


class ScriptEvent : public EventData
{
public:
	ScriptEvent();
	static bool RegisterEventCreator(unsigned long, ScriptEventCreator);
	static bool RegisterType(const char*, unsigned long);
	static ScriptEvent* CreateEvent(unsigned long);
	LuaPlus::LuaObject	GetData() const;
	void		SetData(LuaPlus::LuaObject data);
	virtual bool BuildEventForScript() = 0;
	virtual bool BuildEventFromScript() = 0;
private:
	static unordered_map<unsigned long, ScriptEventCreator>	m_creators;
protected:
	LuaPlus::LuaObject		m_eventData;
};