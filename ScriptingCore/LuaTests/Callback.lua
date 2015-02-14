function TestCallback(eventData)
	print("Received " .. eventData["m_msg"] .. " in lua")
end

function ObjectCallback(eventData)
	plane = GetObject(eventData.m_id)
	print("Event callback ObjectCallback(event) calling Plane::Takeoff()\nC++ member function from lua\n")
	plane:Takeoff()
	print(" from lua")
end

TestHandle = RegisterCallback(EventTypes.TestEvent, TestCallback)
ObjectHandle = RegisterCallback(EventTypes.ObjectEvent, ObjectCallback)
