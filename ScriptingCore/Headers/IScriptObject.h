#pragma once

#define DECLARE_AS_SCRIPT_TYPE() \
	public: \
	static void RegisterWithScripts();\
	const char* GetScriptType() const override; \
	static const char*	m_scriptType

#define REGISTER_OBJECT(object) \
	object::RegisterWithScripts()
	

class IScriptObject
{
public:
	virtual const char* GetScriptType() const = 0;
};