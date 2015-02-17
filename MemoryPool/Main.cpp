/* Written by Mikhail Jacques
Email: jacques.mikhail@gmail.com
GitHub: https://github.com/MikhailJacques
LinkedIn: https://www.linkedin.com/in/mikhailjacques
*/

#include <iostream>
#include <windows.h>
#include "TestClass.h"
#include "MemoryPool.h"

using namespace std;

// Requirement: Initialization – Usually called right after the system boot, and before the applications are launched.
#define CAPACITY 10
static MemoryPool<TestClass> pool(CAPACITY);

int main()
{
	// const unsigned CAPACITY = 10;
	// MemoryPool<TestClass> pool(CAPACITY);

	const unsigned NUM_TESTS = 5;
	const unsigned NUM_LOOPS = 100000;

	for (int test_count = 1; test_count <= NUM_TESTS; test_count++)
	{
		DWORD tick_count = GetTickCount();

		cout << "[Test " << test_count << "]" << endl;


		// *** USE MEMORY POOL ***

		for (unsigned i = 0; i < NUM_LOOPS; i++)
		{
			TestClass * p[CAPACITY];

			// Use up the entire memory pool space but no more
			for (unsigned j = 0; j < CAPACITY; j++)
			{
				p[j] = pool.Create(j);
			}

			// Free up all but the last 5 chunks in memory pool space
			for (unsigned j = 0; j < CAPACITY - 5; j++)
			{
				pool.Remove(p[j]);
			}

			// Reuse previously freed up chunks in memory pool space
			for (unsigned j = 0; j < CAPACITY - 5; j++)
			{
				p[j] = pool.Create(j);
			}

			// Free up the entire memory pool space
			for (unsigned j = 0; j < CAPACITY; j++)
			{
				pool.Remove(p[j]);
			}
		}

		cout << "[Loop " << NUM_LOOPS << " Times]"
			<< "     Using memory pool = " << GetTickCount() - tick_count << " ms" << endl;

		tick_count = GetTickCount();


		// *** DO NOT USE MEMORY POOL ***

		for (unsigned i = 0; i < NUM_LOOPS; i++)
		{
			TestClass * p[CAPACITY];

			for (unsigned j = 0; j < CAPACITY; j++)
			{
				p[j] = new TestClass(j);
			}

			for (unsigned j = 0; j < CAPACITY - 5; j++)
			{
				delete p[j];
			}

			for (unsigned j = 0; j < CAPACITY - 5; j++)
			{
				p[j] = new TestClass(j);
			}

			for (unsigned j = 0; j < CAPACITY; j++)
			{
				delete p[j];
			}

		}

		cout << "[Loop " << NUM_LOOPS << " Times]"
			<< " NOT using memory pool = " << GetTickCount() - tick_count << " ms" << endl;

		cout << endl;
	}


	cout << "[End of Test]\n";

	cin.get();

	return 0;
}