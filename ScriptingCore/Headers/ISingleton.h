#pragma once

template <class T>
class ISingleton
{
public:

	static T* GetInstance()
	{
		static T instance;
		return &instance;
	}

protected:
	
	ISingleton() {}
	
	ISingleton(const ISingleton&);
	
	ISingleton& operator=(const ISingleton&);
};

#define SET_SINGLETON(x) friend ISingleton<x>