#include "../Headers/Plane.h"
#include "../Headers/LuaStateManager.h"

const char* Plane::m_scriptType = "Plane";

Plane::Plane(unsigned long id) : IGameObject(id)
{
	mPosition =1; 
	mScale = 1.0f; 
	mOrientation = 0.0f;
}


int Plane::GetPosition() const 		
{
	return mPosition;
}


void Plane::SetPosition(int para_pos) 	
{
	mPosition = para_pos;
}
	

float Plane::GetScale() const
{
	return mScale;
}
	

void Plane::SetScale(float para_Scl) 	
{
	mScale = para_Scl;
}


float Plane::GetOrientation() const 	
{
	return mOrientation;
}
	

void Plane::SetOrientation(float para_Ornt) 	
{
	mOrientation = para_Ornt;
}	
	

void Plane::Free()  
{
	delete this;
}	 

IGameObject *_stdcall Plane::Create(unsigned long id) 
{
	return (NEW Plane(id));
}

const char* Plane::GetScriptType() const
{
	return m_scriptType;
}

void Plane::RegisterWithScripts()
{
	LuaStateManager* lua = LUASTATE;
	LuaPlus::LuaObject globals = lua->GetGlobals();
	LuaPlus::LuaObject metatable = globals.GetByName(Plane::m_scriptType);
	if (metatable.IsNil())
	{
		metatable = globals.CreateTable(Plane::m_scriptType);
		metatable.SetObject("__index", metatable);
	}
	metatable.RegisterObjectDirect("Takeoff", (Plane*)0, &Plane::Takeoff);
}

void Plane::Takeoff()
{
	cout << "Taking off at pos " << mPosition;
}
