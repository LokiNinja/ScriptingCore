#include "../Headers/ScriptingCoreDemoExports.h"
#include "../Headers/LuaStateManager.h"
#include "../Headers/Logger.h"
#include "../Headers/EventManager.h"
#include "../Headers/TestEvent.h"
#include "../Headers/LuaCallbackManager.h"
#include "../Headers/LuaEventCallback.h"
#include "../Headers/GameObjectFactory.h"
#include "../Headers/Plane.h"

LuaCallbackManager* ScriptingCoreDemoExports::m_luaCallbacks = nullptr;


ScriptingCoreDemoExports::ScriptingCoreDemoExports()
{
}

bool ScriptingCoreDemoExports::Register()
{
	if (m_luaCallbacks)
	{
		LOGS("LuaCallbackManager already exists");
		return false;
	}
	m_luaCallbacks = NEW LuaCallbackManager;
	LuaPlus::LuaObject globals = LUASTATE->GetGlobals();
	REGISTER_EVENT(TestEvent);
	REGISTER_EVENT(ObjectEvent);
	globals.RegisterDirect("Log", &ScriptingCoreDemoExports::Log);
	globals.RegisterDirect("Fire", &ScriptingCoreDemoExports::FireEvent);
	globals.RegisterDirect("FireImmediate", &ScriptingCoreDemoExports::FireImmediate);
	globals.RegisterDirect("RegisterCallback", &ScriptingCoreDemoExports::RegisterCallback);
	globals.RegisterDirect("UnregisterCallback", &ScriptingCoreDemoExports::UnregisterCallback);
	globals.RegisterDirect("CreateObject", &ScriptingCoreDemoExports::CreateActor);
	globals.RegisterDirect("GetObject", &ScriptingCoreDemoExports::GetObject);
	REGISTER_OBJECT(Plane);
	return true;
}

ScriptEvent* ScriptingCoreDemoExports::BuildEvent(unsigned long type, LuaPlus::LuaObject data)
{
	ScriptEvent* Event = ScriptEvent::CreateEvent(type);
	if (!Event)
	{
		LOGS("Failed to create event of type " + std::to_string(static_cast<long long>(type)));
		return nullptr;
	}
	Event->SetData(data);
	return Event;
}

bool ScriptingCoreDemoExports::FireEvent(unsigned long type, LuaPlus::LuaObject data)
{
	ScriptEvent* Event(BuildEvent(type, data));
	if (Event)
	{
		if (!Event->BuildEventFromScript())
		{
			delete Event;
			return false;
		}
		EVENTS->Fire(Event);
		return true;
	}
	LOGS("Failed to fire event");
	return false;
}

bool ScriptingCoreDemoExports::FireImmediate(unsigned long type, LuaPlus::LuaObject data)
{
	ScriptEvent* Event(BuildEvent(type, data));
	if (Event)
	{
		if (!Event->BuildEventFromScript())
		{
			delete Event;
			return false;
		}
		EVENTS->FireImmediate(Event);
		return true;
	}
	LOGS("Failed to fire event");
	return false;
}

void ScriptingCoreDemoExports::Log(const char* error)
{
	if (error == nullptr)
	{
		LOGS("Cannot log nullptr");
	}
	else
	{
		LOGS(error);
	}
}

unsigned long ScriptingCoreDemoExports::RegisterCallback(unsigned long type, LuaPlus::LuaObject callback)
{
	assert(m_luaCallbacks);
	if (callback.IsFunction())
	{
		LuaEventCallback* listener = NEW LuaEventCallback(type, callback);
		m_luaCallbacks->AddListener(listener);
		return reinterpret_cast<unsigned long>(listener);
	}
	else
	{
		LOGS("Callback is not a function");
		return 0;
	}
}

void ScriptingCoreDemoExports::UnregisterCallback(unsigned long type)
{
	if (!m_luaCallbacks)
	{
		LOGS("Lua Callback manager is nullptr");
		return;
	}
	if (type == 0)
	{
		LOGS("Callback handle cannot be 0");
		return;
	}
	LuaEventCallback* callback = reinterpret_cast<LuaEventCallback*>(type);
	m_luaCallbacks->RemoveListener(callback);
}

unsigned long ScriptingCoreDemoExports::CreateActor(const char* type, int pos, float orientation)
{
	unsigned int ActorId = FACTORY->CreateGameObject(type);
	if (ActorId =! 0)
	{
		IGameObject* obj = FACTORY->GetObject(ActorId);
		obj->SetPosition(pos);
		obj->SetOrientation(orientation);
		cout << "Successfully created " << type << " object." << endl;
	}
	return ActorId;
}

void ScriptingCoreDemoExports::Unregister()
{
	if (m_luaCallbacks)
	{
		delete m_luaCallbacks;
		m_luaCallbacks = 0;
	}
}

LuaPlus::LuaObject ScriptingCoreDemoExports::GetObject(unsigned long id)
{
	IGameObject* object = FACTORY->GetObject(id);
	LuaPlus::LuaState* state = LUASTATE->GetState();
	LuaPlus::LuaObject globals = LUASTATE->GetGlobals();
	LuaPlus::LuaObject luaObject;
	LuaPlus::LuaObject metatable;
	IScriptObject* scriptObject = dynamic_cast<IScriptObject*>(object);
	if (object == nullptr || scriptObject == nullptr)
	{
		luaObject.AssignNil(state);
		return luaObject;
	}
	luaObject.AssignNewTable(state);
	luaObject.SetLightUserData("__object", object);
	metatable = globals.GetByName(scriptObject->GetScriptType());
	if (metatable.IsNil())
	{
		luaObject.AssignNil(state);
		return luaObject;
	}
	luaObject.SetMetaTable(metatable);
	return luaObject;
}