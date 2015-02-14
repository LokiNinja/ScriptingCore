#pragma once
#include <list>
#include <unordered_map>
#include "FastDelegate.h"
#include "ISingleton.h"
using namespace std;

class EventData;

typedef fastdelegate::FastDelegate1<EventData*> EventDelegate;

const unsigned int NUM_QUEUES = 2;

class EventManager : public ISingleton<EventManager>
{
	SET_SINGLETON(EventManager);
public:
	
	~EventManager() { Shutdown(); }
	
	bool Register(const EventDelegate&, const unsigned long&);
	
	bool Unregister(const EventDelegate&, const unsigned long&);
	
	bool FireImmediate(EventData*);
	
	bool Fire(EventData*);
	
	bool Update();
	
	bool Init();
	
	void Shutdown();

private:

	EventManager() { m_activeQueue = 0; }
	
	EventManager(const EventManager&);
	
	EventManager& operator=(const EventManager&);

	void NextQueue();

	unordered_map<unsigned long, list<EventDelegate>>		m_listeners;
	list<EventData*>										m_queues[NUM_QUEUES];
	int														m_activeQueue;
};

#define EVENTS EventManager::GetInstance()
