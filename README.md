# ScriptingCore
Flexible and extensible scripting core for a game engine


Authors: Thomas Loudiana, Justin Atkinson, Javier Mendoza

Description:  A scripting core for a game engine.  It utilizes an event manager and templated delegates to pass events
across the C++/Script boundary.  It can be extended to add support to more scripting languages by fleshing out the supplied
interfaces.  Demo implements interface for Lua.

Commands:
String: Entering this command will allow the user to essentially enter ‘interactive’ mode with the core.  This will allow them to enter up to 1024 characters of script (Lua in this case) or to run with fewer than 1024 characters the prompt can be terminated with an ‘@’ symbol. A description of functions exported to Lua can be found below.
File: This command will allow the user to run a preexisting script file found in the same directory as the demo executable.  One great use for this is to have a script file will all of the event callback function in it.  This can be run upon initialization and then events will automatically be delegated to the correct functions as they are dispatched.
Fire: Fire is the command to fire a ‘TestEvent’ event from C++.  Because this is only a demo meant to showcase how the major features work there are only two types of events: ‘TestEvent’ and ‘ObjectEvent’.  TestEvent has two callbacks: one in Lua and one in C++.  Object event has a single callback in Lua.  More on these events can be found below. In order to register the Lua side delegates with the event system, the user should first run the prompt with the file command and then specify the file Callback.lua as the file to run.
Quit: Quits the application framework.

Functions:
Functions can be called either in a standalone Lua file or via the string command. The framework supports all of the standard Lua library functions such as print().  In addition the framework also exports a variety of its own functions.  In the following function list parameters are specified in the syntactical form paramName->paramType.  For purposes of transparency I have substituted the actual parameter names with a more user friendly names of the variety param(n) where n is the parameters position in the argument list: 
void Log(param1->string): Logs the text in param1 to a file Log.txt along with the date and time the error occurred at, the file in which it occurred and the line number on which the log statement lies. Commonly used for debugging purposes.  Its usage is ‘Log(“This is my error in my Lua code”)’. 
unsigned long CreateActor(param1->string, param2->int, param3->float): This will create an object of the type specified by param1.  For purposes of the demo framework, only one type of object is supported of type “PLANE”.  In actuality, as many types as necessary could be added to the factory class and created from Lua via this function.  The param1 parameter is case sensitive. The other two parameters do nothing and were only used for testing purposes.  Take note when calling this function from Lua that param2 is of type int, and param3 is of type float.  In actual practice these would normally be substituted with a vector that defines the objects position and orientation.  The return value of this function is a handle to the object instead of the object itself.  This is because any object present in the factory list can be created via this function, however it is possible that not all these objects support scripting functionality.  An example of its usage is ‘myLuaHandle = CreateObject(“PLANE”, 2, 3.2)’
ScriptObject GetObject(param1->unsigned long): Calling this function attempts to grab the scripting interface of an object if it exists.  If the object does not have a scripting interface exposed this function will return ‘nil’.  The parameter param1 is simply the handle of the object.  If the object was created with the function CreateObject from Lua script than it would be the handle returned by CreateObject.  Its usage is as follows: ‘myLuaPlane = GetObject(myLuaHandle)’
void Fire(param1->unsigned long, param2->event): This function fires an event to the C++ event manager which then delegates to both Lua callbacks and C++ callbacks.  Param1 is the event id of the actual event.  When events are exported to Lua their id is registered in a global table called EventTypes.  This allows the event id to be grabbed by name.  An example of grabbing the event id for the TestEvent event type is ‘EventTypes.TestEvent’. It’s that simple.  Param2 is the actual event data that needs to be sent.  The execution of this function then looks like ‘Fire(EventTypes.TestEvent, myTestEvent).  These will send of the Lua table myTestEvent to the event manager and interpret it as a TestEvent type.  The difference between this function and FireImmediate(see below) has to do with the implementation of the event system.  The event system uses a series of alternating queues to prevent infinite looping.  Event callbacks can fire off events, which fire off events, etc.  To prevent the event system from entering an infinite loop it alternates queues every update step.  See below for information specific to events.
void FireImmediate(param1->unsigned long, param2->event): This version is exactly the same as its counterpart above.  The only difference is that it does not wait for the update step of the event system and, instead, directly calls all delegates associated with an event.
unsigned long RegisterCallback(param1->unsigned long,param2->function): RegisterCallback registers a Lua function as a callback for an event.  After calling this function, the function passed in param2 will be called whenever a function of type param1 is fired.  The return value of this function is a handle to the delegate registered so that it can be unregistered at a later juncture if needed. The following is an example: ‘myCallbackHandle = RegisterCallback(EventTypes.ObjectEvent, ObjectEventCallback)’
void UnregisterCallback(param1->unsigned long): Unregisters a callback previsouly registered with RegisterCallback.  Param1 is the handle to the callback that was returned with RegisterCallback.  It can be used like so ‘UnregisterCallback(myCallbackHandle)’

Files:
There are two Lua scrip files included with the demo.  The first is Callback.lua.  This file contains callback functions for both types of events in the demo.  The second is Fire.lua.  This file will fire off the events.  As such, if events should be received by the functions in lua, the user should run the file Callback.lua before the file Fire.lua. These two files display one more piece of functionality present in the scripting core.  The ability to register member functions to be called in lua!

Example:
Here we will include a typical example of how to use the actual scripting core as opposed to the demo application.  The very first thing that needs to be done is a new IScriptState interface needs to be implemented for the scripting language of your choice.  We will follow along with the creation of the demo and create one for Lua.  
The IScriptState interface has four functions that need to be overridden: Init(), Shutdown(), DoString(char*), and DoFile(char*).  Init() and Shutdown() both take now arguments and Init() returns an error code while Shutdown() returns nothing.  DoString(char*) takes as its parameter a string in the scripting language of your choice and directly executes it.  DoFile(char*) attempts to execute a script file.  Both of these return an error code.  For Lua the implementation looks something like this:

    class LuaStateManager : public IScriptState, public ISingleton<LuaStateManager>
    {
    SET_SINGLETON(LuaStateManager);
    public:
    	~LuaStateManager();
    	ERROR Init() override;
    	ERROR DoString(const char*) override;
    	ERROR DoFile(const char*) override;
    	LuaPlus::LuaObject GetGlobals();
    	LuaPlus::LuaState* GetState();
    private:
    LuaPlus::LuaState*		m_luastate;
    //More functions here
    };
    #define LUASTATE LuaStateManager::GetInstance()
You will notice that this class implements two interfaces.  The first is the IScriptState interface defined above.  The second is the ISingleton interface which defines this as a singleton class.  There are several things to note about the ISingleton interface.  The first is that it is a template and you must pass the name of the child class as a template parameter when implementing from this interface.  The second is the SET_SINGLETON() macro.  You must also pass the name of the implementing class to this macro.  The macro simply expands to friend class ISingleton<childclassname> so that the ISingleton interface can invoke the private constructors of the derived class.  Finally, for clarity in our core we have added the macro LUASTATE at the bottom of the file.  This is just for simplicity and readability and simply invokes the ISingleton::GetInstance() function on LuaStateManager.  In this way you can access functions such as DoFile in the manner of LUASTATE->DoFile(“myFile.lua”).
Next we will export some functions to the scripting language of our choice (Lua in this example).  For this there is a very simple interface that takes care of exposing all the functions we would like to the script state.  It is not necessary to put your exports in this class, however we do it for consistency and readability.  It looks like this:

    class IScriptExports
    {
    public:
    virtual bool Register() = 0;
    virtual void Unregister() = 0;
    };
    
There are two methods that need to be implemented here.  One is the function that takes care of all registrations and the other performs any cleanup (rarely is any needed).  The implementation of this class for our demo looks like this:

    class ScriptingCoreDemoExports : public IScriptExports
    {
    public:
    	ScriptingCoreDemoExports();
    	bool Register() override;
    	static bool FireEvent(unsigned long type, LuaPlus::LuaObject data);
    	static bool FireImmediate(unsigned long type, LuaPlus::LuaObject data);
    	static void Log(const char*);
    	static unsigned long RegisterCallback(unsigned long type, LuaPlus::LuaObject callback);
    	static void UnregisterCallback(unsigned long type);
    	static unsigned long CreateActor(const char* type, int pos, float orientation);
    	static LuaPlus::LuaObject GetObject(unsigned long);
    	void Unregister() override;
    private:
    	static ScriptEvent* BuildEvent(unsigned long type, LuaPlus::LuaObject object);
    	static LuaCallbackManager*	m_luaCallbacks;
    };
    
You will notice that there are several static functions in this class as well.  These are the functions that we will be exporting to Lua via our implementation of the Register method. These functions can be whatever you decide to expose.  For this demo we chose some functions to fire events, and create objects.  Typically these functions just call functions in the actual game engine but with some added bookkeeping for Lua.  There is also a LuaCallbackManager which we will get to in a bit.  Here is how the registration looks in the actual Register function:

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

You can see we invoke the LUASTATE singleton and grab its global state.  If you have added support for other scripting languages that these functions would be exposed to you would do the same with their state managers.  There are several macros here which we will cover later.  The functions themselves are registered with Lua via the RegisterDirect method of the globals table (these are the static methods in our class).  After this method runs we will be able to call these functions from script.  The first parameter passed is a string that will become the script name of the function and the second parameter is the address of the function itself.
Next we will talk about creating events and callbacks for Lua.  There is an interface that needs to be implemented for any type of event, whether it will be received by script or not.  It is very simple and looks like this:

    class EventData
    {
    public:
    	EventData(float timeStamp = 0.f);
    	virtual ~EventData();
    	virtual unsigned long GetEventType() const = 0;
    	float GetTimeStamp() const;
    private:
    	float m_timeStamp;
    };

It essentially just wraps an event id around the particular event types.  For an event that will need to be communicated with via script we have derived an interface from this EventData interface.  It looks like the following:

    class ScriptEvent;
    typedef ScriptEvent*(*ScriptEventCreator)();
    #define REGISTER_EVENT(eventClass) \
    	ScriptEvent::RegisterType(#eventClass, eventClass::s_eventType);\
    	ScriptEvent::RegisterEventCreator(eventClass::s_eventType, &eventClass::CreateEvent);
    	
    #define EXPORT_TO_SCRIPT(eventClass) \
    public: \
    	static ScriptEvent* CreateEvent() \
    	{ \
    		return NEW eventClass(); \
    	}
    	
    class ScriptEvent : public EventData
    {
    public:
    	ScriptEvent();
    	static bool RegisterEventCreator(unsigned long, ScriptEventCreator);
    	static bool RegisterType(const char*, unsigned long);
    	static ScriptEvent* CreateEvent(unsigned long);
    	LuaPlus::LuaObject	GetData() const;
    	void		SetData(LuaPlus::LuaObject data);
    	virtual bool BuildEventForScript() = 0;
    	virtual bool BuildEventFromScript() = 0;
    private:
    	static unordered_map<unsigned long, ScriptEventCreator>	m_creators;
    protected:
    	LuaPlus::LuaObject		m_eventData;
    };
    
There are three static methods which take care of looking up the correct creation functions when Lua tries to create an event.  There are also two pure virtual methods that any type of script event will need to implement.  These are BuildEventForScript() and BuildEventFromScript().  BuildEventForScript() is called whenever C++ fires an event that needs to be received in script.  For our Lua example this would take care of assembling the m_eventData member that you see in the above declaration.  For example if the event contained an actor/object id as part of its data this function might do something like m_eventData.SetString(“ActorId”, m_id).  Lua could then retrieve this value as follows: EventTable.ActorId.  
The BuildEventFromScript() needs to be implemented to do the inverse of this.  It is called whenever the event manager receives an event fired from script.  Continuing with the above example its implementation might look something like: m_id = m_eventData.GetByName(“ActorId").GetNumber();
Finally there are two macros for script events included here.  The EXPORT_TO_SCRIPT() macro is included in the class declaration and simply declares a creation function:

    class TestEvent : public ScriptEvent
    {
    EXPORT_TO_SCRIPT(TestEvent);
    public:
    	TestEvent(const char* msg);
    	TestEvent();
    	//More functions
    };
    
The REGISTER_EVENT() macro is called from the Register function as can be seen above and simply does some bookkeeping for Lua such as adding the event name to a table on the Lua stack.
The final topic we need to cover is how to register objects themselves with Lua.  This is done, first, by implementing the IScriptObject interface:

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
    
The interface itself only contains a single method that must be implemented.  This function should simply return a string that contains the name of the class for reference in script.  There are also two macros.  DECLARE_AS_SCRIPT_TYPE() needs to be included in the class declaration of the object you wish to register.  It forces you to supply the string name of the class and also defines a static registration function for the class:

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

Member functions can be registered like so: 

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
    
The only real difference here between registering our other functions above, is that instead of using the RegisterDirect method we use the RegisterObjectDirect method.  We pass a string for the name of the method on the script side, a nullptr that has been cast to our type, and the address of the function.  This will now act as a member function in Lua and could be called as follows: myPlane:Takeoff().
The REGISTER_OBJECT() macro is called from the Register function as seen above and simply calls the static RegisterWithScripts method.  It was implemented as a macro for consistency with the way it is done for events.
That’s really all there is to it.  Get out there and have some fun with it!
