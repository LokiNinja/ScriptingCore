#pragma once
#include <LuaPlus.h>
#include "FastDelegate.h"

class EventData;

typedef fastdelegate::FastDelegate1<EventData*> EventDelegate;

class LuaEventCallback
{
	unsigned long m_eventType;
	LuaPlus::LuaObject m_callback;
public:
	explicit LuaEventCallback(const unsigned long& type, const LuaPlus::LuaObject& callback);
	~LuaEventCallback() {}
	EventDelegate GetDelegate()
	{
		return EventDelegate(this, &LuaEventCallback::ScriptEventDelegate);
	}
	const unsigned long& GetType() const { return m_eventType; }
	void ScriptEventDelegate(EventData*);
};