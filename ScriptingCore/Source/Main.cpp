#include <iostream>
#include "../Headers/LuaStateManager.h"
#include "../Headers/ScriptingCoreDemoExports.h"
#include "../Headers/TestEvent.h"
#include "../Headers/EventManager.h"
#include "../Headers/IGameObject.h"
#include "../Headers/GameObjectFactory.h"
#include <Windows.h>

#if defined _DEBUG || defined DEBUG
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

using namespace std;

void Print(EventData*);
void Uppercase(char* stuff);
void RunFile(IScriptState*);
void RunString(IScriptState*);
void ConsoleOut(const char*);

HANDLE hstdOut;
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

int main()
{
#if defined _DEBUG || defined DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	IScriptState* lua = LUASTATE;
	IGameObject *pGameObject = NULL;
	if (LUASTATE->Init() != IScriptState::SCRIPT_SUCCESS)
	{
		return -1;
	}
	
	ScriptingCoreDemoExports myExports;			//registers firing events
	if (!myExports.Register())
	{
		return -1;
	}
	
	EVENTS->Register(&Print, TestEvent::s_eventType);

	char* command = NEW char[64];					

	hstdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hstdOut, &consoleInfo);

	ConsoleOut("Enter \'string\', lua \'file\', \'fire\' TestEvent, or \'quit\':");
	cin.getline(command, 64, '\n');
	Uppercase(command);
	while (strcmp(command, "QUIT") != 0)
	{
		if (strcmp(command, "FILE") == 0)
		{
			RunFile(lua);		
		}
		else if (strcmp(command, "STRING") == 0)
		{
			RunString(lua);
		}
		else if (strcmp(command, "FIRE") == 0)
		{
			TestEvent* test = NEW TestEvent("TestEvent");
			EVENTS->Fire(test);
		}
		else
		{
			ConsoleOut("I don't understand");
		}
		EVENTS->Update();
		ConsoleOut("Enter \'string\', lua \'file\', \'fire\' TestEvent, or \'quit\':");
		cin.getline(command, 64, '\n');
		Uppercase(command);
	}
	myExports.Unregister();
	delete[] command;
	return 0;
}

void Print(EventData* data)
{
	TestEvent* Event = (TestEvent*)data;
	char message[1024];
	cout << Event->GetMsg() << " received in C++!!" << endl;
}

void RunFile(IScriptState* state)
{
	ConsoleOut("Please enter name of lua file to run:");
	char fileName[256];
	cin.getline(fileName, 256, '\n');
	if (!state->DoFile(fileName) == IScriptState::SCRIPT_SUCCESS)
	{
		ConsoleOut("Could not run file.  See Log.txt for details");
	}
}

void RunString(IScriptState* state)
{
	ConsoleOut("Please enter a string of lua code [up to 1024 characters or terminated by \'@\']:");
	char code[1024];
	cin.getline(code, 1024, '@');
	cin.get();
	state->DoString(code);
}


void Uppercase(char* stuff)
{
	int length = strlen(stuff);
	int current = 0;
	while (current < length && stuff[current] != '\0')
	{
		stuff[current] = toupper(stuff[current]);
		current++;
	}
}

void ConsoleOut(const char* output)
{
	SetConsoleTextAttribute(hstdOut, 0x0C);
	cout << output << endl;
	SetConsoleTextAttribute(hstdOut, consoleInfo.wAttributes);
}