#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

int main(void) 
{
	ofstream data, query;
	data.open("data.pts");
	query.open("query.pts");

	// Seed random number generator
	srand(time(NULL)); 

	for (int i = 1; i <= 100; i++) 
	{
		// Pick random number between 1 and 100, inclusive, and write it to a data file
		data << 1 + (rand() % 101) << " ";

		if (i % 20 == 0) 
		{
			data << endl;
		}
	}

	for (int i = 1; i <= 2; i++) 
	{
		// Pick random number between 1 and 100, inclusive, and write it to a query file
		query << 1 + (rand() % 101) << " ";
	}

	data.close();
	query.close();

	cin.get();

	return 0;
}