
// Use of singleton design pattern for random number generation

#include <iostream>

using namespace std;

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

int main()
{
	const int num_of_numbers = 10;

	for (int ii = 0; ii < num_of_numbers; ii++)
	{
		cout << Random::Get().Range(1, 100) << endl;
	}

	return 0;
}