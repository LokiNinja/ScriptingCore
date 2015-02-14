#pragma once

class EventData
{
public:
	
	EventData(float timeStamp = 0.f);
	
	virtual ~EventData();
	
	virtual unsigned long GetEventType() const = 0;
	
	float GetTimeStamp() const;

private:

	float m_timeStamp;
};


