#pragma once

#include <string>
#include <map>
#include "IGameObject.h"
#include "ISingleton.h"
#include <unordered_map>

using namespace std;

typedef IGameObject* (_stdcall *CreateGameObjectFn)(unsigned long id);

class GameObjectFactory : public ISingleton<GameObjectFactory>
{
	SET_SINGLETON(GameObjectFactory);
private:
	GameObjectFactory();								
	GameObjectFactory(const GameObjectFactory &)	{}		
	GameObjectFactory &operator=(const GameObjectFactory &) {return *this;}

	typedef map<string, CreateGameObjectFn> FactoryMap;	
	FactoryMap m_FactoryMap;							

	typedef unordered_map<unsigned long, IGameObject*> ActorMap;
	ActorMap m_actorMap;

	static	unsigned long	m_nextId;
public:
	~GameObjectFactory();		



	void addGameObject( const string &str_ObjName, CreateGameObjectFn pfnCreate );	
	unsigned long CreateGameObject( const string &str_ObjName );
	IGameObject*	GetObject(unsigned long);

};

#define FACTORY  GameObjectFactory::GetInstance()
