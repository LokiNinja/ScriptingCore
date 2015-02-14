#include "../Headers/Logger.h"
#include <cstdarg>
#include <ctime>
using namespace std;

Logger::Logger()
{
	m_fileHandle.open("Log.txt", ios::trunc | ios::out);
	time_t t = time(0);
	tm now;
	localtime_s(&now, &t);
	char buffer[32];
	asctime_s(buffer, 32, &now);
	m_fileHandle << "Log Begin: " << buffer;
}

Logger::~Logger()
{
	m_fileHandle << "Log End";
	m_fileHandle.close();
}

void Logger::Log(const char* file, unsigned int line, string error)
{
	time_t t = time(0);
	tm now;
	localtime_s(&now, &t);
	m_fileHandle << now.tm_mon << "/" << now.tm_mday << "/" << now.tm_year + 1900 << " " << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << " ";
	m_fileHandle << "File: " << file << " Line: " << line << " Error: " << error << endl;
}
