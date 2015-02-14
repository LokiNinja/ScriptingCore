#include "../Headers/LuaEventCallback.h"
#include "../Headers/ScriptEvent.h"

LuaEventCallback::LuaEventCallback(const unsigned long& type, const LuaPlus::LuaObject& callback) :
	m_eventType(type), m_callback(callback)
{

}

void LuaEventCallback::ScriptEventDelegate(EventData* data)
{
	ScriptEvent* Event = (ScriptEvent*)data;
	Event->BuildEventForScript();
	LuaPlus::LuaFunction<void> callback = m_callback;
	callback(Event->GetData());
}