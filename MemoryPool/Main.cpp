/* Written by Mikhail Jacques
Email: jacques.mikhail@gmail.com
GitHub: https://github.com/MikhailJacques
LinkedIn: https://www.linkedin.com/in/mikhailjacques
*/

#include <time.h>
#include <iostream>
#include <windows.h>
#include "TestClass.h"
#include "MemoryPool.h"
#include "MemoryPool_2.h"

using namespace std;

// Requirement: Initialization – Usually called right after the system boot, and before the applications are launched.
#define CAPACITY 10
static MemoryPool<TestClass> pool(CAPACITY);
static MemoryPool_2<TestClass, (sizeof(TestClass) * CAPACITY * 2)> pool_2;

int main()
{
	const unsigned NUM_TESTS = 5;
	const unsigned NUM_LOOPS = 100000;

	TestClass * p[CAPACITY], test;
	test.setNum(5);
	test.setBuf("Hello World");

	for (int test_count = 1; test_count <= NUM_TESTS; test_count++)
	{
		clock_t start = clock();
		DWORD tick_count = GetTickCount();

		cout << "[Test " << test_count << "]" << endl;


		// *** USE MEMORY POOL 1 ***

		for (unsigned i = 0; i < NUM_LOOPS; i++)
		{
			// Use up the entire memory pool space but no more
			for (unsigned j = 0; j < CAPACITY; j++)
			{
				p[j] = pool.Create(test);
			}

			// Free up all but the last 5 chunks in memory pool space
			for (unsigned j = 0; j < CAPACITY - 5; j++)
			{
				pool.Remove(p[j]);
			}

			// Reuse previously freed up chunks in memory pool space
			for (unsigned j = 0; j < CAPACITY - 5; j++)
			{
				p[j] = pool.Create(test);
			}

			// Free up the entire memory pool space
			for (unsigned j = 0; j < CAPACITY; j++)
			{
				pool.Remove(p[j]);
			}
		}

		cout << "[Loop " << NUM_LOOPS << " Times]"
			<< "     Using memory pool 1 = " << GetTickCount() - tick_count << " msecs / " 
			<< (((double)clock() - start) / CLOCKS_PER_SEC) << " secs " << endl;

		start = clock();
		tick_count = GetTickCount();
		

		// *** USE MEMORY POOL 2 ***
		for (unsigned i = 0; i < NUM_LOOPS; i++)
		{
			// Use up the entire memory pool space but no more
			for (unsigned j = 0; j < CAPACITY; j++)
			{
				p[j] = pool_2.newElement(test);
			}

			// Free up all but the last 5 chunks in memory pool space
			for (unsigned j = 0; j < CAPACITY - 5; j++)
			{
				pool_2.deleteElement(p[j]);
			}

			// Reuse previously freed up chunks in memory pool space
			for (unsigned j = 0; j < CAPACITY - 5; j++)
			{
				p[j] = pool_2.newElement(test);
			}

			// Free up the entire memory pool space
			for (unsigned j = 0; j < CAPACITY; j++)
			{
				pool_2.deleteElement(p[j]);
			}
		}

		cout << "[Loop " << NUM_LOOPS << " Times]"
			<< "     Using memory pool 2 = " << GetTickCount() - tick_count << " msecs / "
			<< (((double)clock() - start) / CLOCKS_PER_SEC) << " secs " << endl;

		// *** DO NOT USE MEMORY POOL ***

		start = clock();
		tick_count = GetTickCount();

		for (unsigned i = 0; i < NUM_LOOPS; i++)
		{
			for (unsigned j = 0; j < CAPACITY; j++)
			{
				p[j] = new TestClass(test);
			}

			for (unsigned j = 0; j < CAPACITY - 5; j++)
			{
				delete p[j];
			}

			for (unsigned j = 0; j < CAPACITY - 5; j++)
			{
				p[j] = new TestClass(test);
			}

			for (unsigned j = 0; j < CAPACITY; j++)
			{
				delete p[j];
			}

		}

		cout << "[Loop " << NUM_LOOPS << " Times]"
			<< " NOT using memory pool   = " << GetTickCount() - tick_count << " msecs / "
			<< (((double)clock() - start) / CLOCKS_PER_SEC) << " secs " << endl;

		cout << endl;
	}

	cout << "[End of Test]\n";

	cin.get();

	return 0;
}