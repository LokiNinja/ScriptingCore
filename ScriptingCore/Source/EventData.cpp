#include "../Headers/EventData.h"

EventData::EventData(float timeStamp) : m_timeStamp(timeStamp)
{

}

EventData::~EventData()
{

}

float EventData::GetTimeStamp() const 
{
	return m_timeStamp; 
}