#include "../Headers/EventManager.h"
#include "../Headers/Logger.h"
#include "../Headers/EventData.h"

bool EventManager::Init()
{
	return true;
}

void EventManager::Shutdown()
{
	for (int i = 0; i < NUM_QUEUES; i++)
	{
		for (auto it = m_queues[i].begin(); it != m_queues[i].end();)
		{
			delete *it;
			*it = nullptr;
			it = m_queues[i].erase(it);
		}
	}
}

bool EventManager::Register(const EventDelegate& callback, const unsigned long& type)
{
	list<EventDelegate>& registeredEvents = m_listeners[type];
	for (auto it = registeredEvents.begin(); it != registeredEvents.end(); it++)
	{
		if (*it == callback)
		{
			LOGS("Attempting to double register delegate");
			return false;
		}
	}
	registeredEvents.push_back(callback);
	return true;
}

bool EventManager::Unregister(const EventDelegate& callback, const unsigned long& type)
{
	auto it = m_listeners.find(type);
	if (it != m_listeners.end())
	{
		list<EventDelegate>& registeredEvents = it->second;
		for (auto it2 = registeredEvents.begin(); it2 != registeredEvents.end(); it2++)
		{
			if (*it2 == callback)
			{
				registeredEvents.erase(it2);
				return true;
			}
		}
	}
	return false;
}

bool EventManager::FireImmediate(EventData* event)
{
	bool processed = false;
	auto it = m_listeners.find(event->GetEventType());
	if (it != m_listeners.end())
	{
		list<EventDelegate>& listenerList = it->second;
		for (auto it2 = listenerList.begin(); it2 != listenerList.end(); it2++)
		{
			EventDelegate callback = *it2;
			callback(event);
			processed = true;
		}
	}
	delete event;
	event = nullptr;
	return processed;
}

bool EventManager::Fire(EventData* event)
{
	if (m_activeQueue < 0 || m_activeQueue >= NUM_QUEUES)
	{
		LOGS("Active queue is out of range");
		return false;
	}
	auto it = m_listeners.find(event->GetEventType());
	if (it == m_listeners.end())
	{
		delete event;
		return false;
	}
	m_queues[m_activeQueue].push_back(event);
	return true;
}

bool EventManager::Update()
{
	bool result = false;
	list<EventData*>& currentQueue = m_queues[m_activeQueue];
	NextQueue();
	while (!currentQueue.empty())
	{
		EventData* event = currentQueue.front();
		currentQueue.pop_front();
		if (event == nullptr)
			continue;
		const unsigned long& type = event->GetEventType();
		auto findIt = m_listeners.find(type);
		if (findIt != m_listeners.end())
		{
			list<EventDelegate>& eventlisteners = findIt->second;
			for (auto it = eventlisteners.begin(); it != eventlisteners.end(); it++)
			{
				EventDelegate callback = *it;
				callback(event);
				result = true;
			}
		}
		delete event;
		event = nullptr;
	}
	return result;
}

void EventManager::NextQueue()
{
	m_activeQueue++;
	if (m_activeQueue >= NUM_QUEUES)
	{
		m_activeQueue = 0;
	}
	m_queues[m_activeQueue].clear();
}