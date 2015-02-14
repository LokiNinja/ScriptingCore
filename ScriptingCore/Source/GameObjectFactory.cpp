#include "../Headers/GameObjectFactory.h"
#include "../Headers/Plane.h"

unsigned long GameObjectFactory::m_nextId = 1;

GameObjectFactory::GameObjectFactory()
{

	addGameObject( "PLANE", Plane::Create );
}

GameObjectFactory::~GameObjectFactory()
{
	m_FactoryMap.clear();
	auto it = m_actorMap.begin();
	while (it != m_actorMap.end())
	{
		delete it->second;
		it = m_actorMap.erase(it);
	}
}

IGameObject* GameObjectFactory::GetObject(unsigned long id)
{
	auto it = m_actorMap.find(id);
	if (it == m_actorMap.end())
		return nullptr;
	return it->second;
}

void GameObjectFactory::addGameObject( const string &str_ObjName, CreateGameObjectFn pfnCreate )

{
	m_FactoryMap[ str_ObjName ] = pfnCreate;	
}

unsigned long GameObjectFactory::CreateGameObject( const string &str_ObjName )
{
	FactoryMap::iterator itrtr = m_FactoryMap.find( str_ObjName );
	if (itrtr != m_FactoryMap.end())
	{
		IGameObject * newObj = itrtr->second(m_nextId);
		m_actorMap[m_nextId] = newObj;
		return m_nextId++;
	}
	return 0;
}

