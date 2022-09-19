#pragma once

#include <iostream>

class Random
{
public:

	Random(const Random&) = delete;
	Random& operator= (const Random&) = delete;

	// The core of the singleton 
	static Random& Get()
	{
		static Random instance;

		return instance;
	}

	// Static function that internally maps to member function that can access member data and all the functionality of the class
	static int Range(unsigned short lower, unsigned short upper) { return Get().IRange(lower, upper); }

private:

	// Disable instantiation outside of the class
	Random()
	{
		srand(time(0));
	}

	int IRange(unsigned short lower, unsigned short upper)
	{
		return (rand() % (upper - lower + 1)) + lower;
	}
};
