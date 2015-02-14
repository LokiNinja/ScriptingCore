#pragma once

#include <iostream>

class IGameObject
{
public:
	IGameObject(unsigned long id) { m_id = id; }
	virtual int GetPosition() const =0;
	virtual void SetPosition(int) =0;
	virtual float GetScale() const =0;
	virtual void SetScale(float) =0;
	virtual float GetOrientation() const =0;
	virtual void SetOrientation(float) =0;
	virtual void Free() =0;
private:
	unsigned long m_id;

	IGameObject() {}

};
