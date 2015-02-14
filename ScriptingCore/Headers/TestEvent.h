#pragma once
#include "ScriptEvent.h"

class TestEvent : public ScriptEvent
{
	EXPORT_TO_SCRIPT(TestEvent);
public:
	TestEvent(const char* msg);
	TestEvent();
	static const unsigned long s_eventType;
	const char* GetMsg() const { return m_msg; }
	unsigned long GetEventType() const override { return s_eventType; }
protected:
	bool BuildEventForScript() override;
	bool BuildEventFromScript() override;
private:
	const char*		m_msg;
};

class ObjectEvent : public ScriptEvent
{
	EXPORT_TO_SCRIPT(ObjectEvent);
public:
	ObjectEvent(unsigned long id);
	ObjectEvent();
	static const unsigned long s_eventType;
	unsigned long GetEventType() const override { return s_eventType; }
	unsigned long GetId() const { return m_id; }
protected:
	bool BuildEventForScript() override;
	bool BuildEventFromScript() override;
private:
	unsigned long m_id;
};