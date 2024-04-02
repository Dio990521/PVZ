#pragma once

template<typename T>
class Singleton
{
private:
	Singleton() {}
	~Singleton() {}

public:
	static T* get()
	{
		static T instance;
		return &instance;
	}

	T* operator->() const
	{
		return get();
	}
};

