#include "../Headers/TestEvent.h"
#include "../Headers/Logger.h"
#include "../Headers/LuaStateManager.h"

const unsigned long ObjectEvent::s_eventType = 0xc50f853b;
const unsigned long TestEvent::s_eventType = 0xda0d2917;

TestEvent::TestEvent(const char* msg)
{
	if (msg == nullptr)
		m_msg = '\0';
	else
		m_msg = msg;
}

TestEvent::TestEvent()
{
	m_msg = nullptr;
}

bool TestEvent::BuildEventForScript()
{
	if (!m_eventData.IsTable())
	{
		m_eventData.AssignNewTable(LUASTATE->GetState());
	}
	m_eventData.SetString("m_msg", m_msg);
	return true;
}

bool TestEvent::BuildEventFromScript()
{
	LuaPlus::LuaObject message = m_eventData.GetByName("m_msg");
	if (message.IsNil())
	{
		LOGS("Message is nil");
		return false;
	}
	m_msg = m_eventData.GetByName("m_msg").GetString();
	return true;
}

ObjectEvent::ObjectEvent(unsigned long id) : m_id(id)
{
}

ObjectEvent::ObjectEvent() : m_id(0)
{
}

bool ObjectEvent::BuildEventForScript()
{
	if (!m_eventData.IsTable())
	{
		m_eventData.AssignNewTable(LUASTATE->GetState());
	}
	m_eventData.SetNumber("m_id", m_id);
	return true;
}


bool ObjectEvent::BuildEventFromScript()
{
	if (m_eventData.GetByName("m_id").IsNil())
	{
		LOGS("Object Id is Nil");
		return false;
	}
	m_id = m_eventData.GetByName("m_id").GetNumber();
	return true;
}