#pragma once


#include "IGameObject.h"
#include "Globals.h"
#include "IScriptObject.h"

class Plane : public IGameObject, public IScriptObject
{
	DECLARE_AS_SCRIPT_TYPE();
private:
	int mPosition;
	float mScale;
	float mOrientation;
public:
	Plane(unsigned long id);							
	int GetPosition() const;
	void SetPosition(int para_pos);
	float GetScale() const;
	void SetScale(float para_Scl);
	float GetOrientation() const;
	void SetOrientation(float para_Ornt);
	void Takeoff();
	void Free(); 

	static IGameObject *_stdcall Create(unsigned long id);
};	//end class Plane
