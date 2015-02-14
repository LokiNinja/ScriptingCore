#pragma once
#include <string>
using namespace std;


class IScriptState
{
public:
	enum ERROR{SCRIPT_SUCCESS = 0, SCRIPT_ROGUENULL, SCRIPT_BADCODE, SCRIPT_BADFILE};
	virtual ~IScriptState() {}
	virtual ERROR Init() = 0;
	virtual ERROR DoString(const char*) = 0;
	virtual ERROR DoFile(const char*) = 0;
private:
	virtual void Shutdown() = 0;
};