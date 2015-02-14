#pragma once


class IScriptExports
{
public:
	virtual bool Register() = 0;
	virtual void Unregister() = 0;
};