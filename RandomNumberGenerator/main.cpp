
// Use of singleton design pattern for random number generation

#include "Random.h"

int main()
{
	const int num_of_numbers = 10;

	for (int ii = 0; ii < num_of_numbers; ii++)
	{
		std::cout << Random::Get().Range(1, 100) << std::endl;
	}

	return 0;
}