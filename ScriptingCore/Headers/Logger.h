#pragma once
#include "ISingleton.h"
#include <string>
#include <fstream>
#include "Globals.h"
using namespace std;

class Logger : public ISingleton<Logger>
{
	SET_SINGLETON(Logger);
public:
	
	Logger();
	
	~Logger();
	
	void Log(const char* file, unsigned int line, string error);

private:
	
	fstream			m_fileHandle;
};

#if defined(_DEBUG) | defined(DEBUG)
#define LOGS(x) Logger::GetInstance()->Log(_FILE_, __LINE__, x)
#else
#define LOGS(x)
#endif