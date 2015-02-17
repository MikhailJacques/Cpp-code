/* Written by Mikhail Jacques
Email: jacques.mikhail@gmail.com
GitHub: https://github.com/MikhailJacques
LinkedIn: https://www.linkedin.com/in/mikhailjacques
*/

#ifndef __TEST_CLASS_H__
#define __TEST_CLASS_H__

class TestClass
{
public:

	TestClass(unsigned val) : num(val) {}

	// Requirement: BONUS: "new" & "delete" operators overloading to provide cleaner APIs to the user.
	// Currently NOT implemented. Left for future research if a need for it should arise.
	//void *operator new(size_t)
	//{
	//	return pool.Create(chunk_size);
	//}

	//void operator delete(void *p)
	//{
	//	pool.Remove(p);
	//}

private:

	// + 8 bytes of struct Chunk attached to each instance of this class
	unsigned num; // 4 bytes
	char m_buffer[1012];
};

#endif