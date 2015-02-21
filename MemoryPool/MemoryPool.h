/* Memory Pool a.k.a. Simple Segregated Storage
Written by Mikhail Jacques
Email: jacques.mikhail@gmail.com
GitHub: https://github.com/MikhailJacques
LinkedIn: https://www.linkedin.com/in/mikhailjacques
*/

#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

#include <malloc.h>
using namespace std;

// Requirement: Different types (Classes? Structures?) and sizes.
template <typename T>
class MemoryPool
{
public:

	MemoryPool(unsigned capacity) throw();
	~MemoryPool() throw();

	void *Allocate(unsigned chunk_size, bool use_memory_pool = true) throw();
	void Deallocate(void *p) throw();

	template<typename... Args>
	T * Create(Args && ...args) throw();
	void Remove(T * p) throw();

private:

	// Address of memory pool
	void *p_memory_pool;

	struct Chunk
	{
		struct Chunk *p_prev, *p_next;
	};

	// Head pointers to Free and Allocated chunks of memory that are arranged in a doubly-linked lists.
	struct Chunk *p_head_free_memory_chunk;
	struct Chunk *p_head_allocated_memory_chunk;

	// Size of memory pool, which is comprised of equal size memory chunks.
	unsigned long memory_pool_size;

	// Size of memory chunk.
	unsigned int memory_chunk_size; // = sizeof(T)
};

// Purpose:
//		Allocates memory block from the system and creates doubly-linked list 
//		to manage memory chunks within the memory block.
// Input parameters:
//		capacity - total number of memory chunks of size T in a memory block.
// Output parameters:
//		None.
template<typename T>
MemoryPool<T>::MemoryPool(unsigned capacity) throw() :
p_memory_pool(NULL), p_head_allocated_memory_chunk(NULL), p_head_free_memory_chunk(NULL),
memory_pool_size((sizeof(T) + sizeof(struct Chunk)) * capacity), memory_chunk_size(sizeof(T)) 
{
	// Allocate a contiguous memory block on the heap
	// p_memory_pool = ::malloc(memory_pool_size);
	p_memory_pool = ::operator new (memory_pool_size);
	
	if (p_memory_pool != NULL)
	{
		struct Chunk *p_curr;

		// Conceptually break the memory blocks into memory chunks and link them
		for (unsigned int i = 0; i < capacity; i++)
		{
			// Calculate the address of the next chunk of memory
			p_curr = (struct Chunk *)((char *)p_memory_pool + i * (sizeof(T) + sizeof(struct Chunk)));

			p_curr->p_prev = NULL;

			// Insert the newly allocated chunk of memory at the head of the linked list.
			p_curr->p_next = p_head_free_memory_chunk;

			if (p_head_free_memory_chunk != NULL)
			{
				p_head_free_memory_chunk->p_prev = p_curr;
			}

			p_head_free_memory_chunk = p_curr;
		}
	}
}

// Purpose:
//		Deallocates system's memory block
template<typename T>
MemoryPool<T>::~MemoryPool() throw()
{
	// ::free(p_memory_pool);
	delete p_memory_pool;
}

// Requirement:  Chunk allocation – Returns a pointer to a clean memory region – O(1).
//
// Purpose:
//		Allocates memory chunk either from the pre-allocated memory pool or from the system (optional). 
// Input parameters:
//		chunk_size - memory chunk size (sizeof(T))
//		use_memory_pool - true use memory pool; false use system call
// Output parameters:
//		A pointer to an allocated memory chunk.
template<typename T>
void *MemoryPool<T>::Allocate(unsigned chunk_size, bool use_memory_pool) throw()
{
	// If either of the following conditions evaluates to true, invoke the memory allocation system call (optional)
	if (chunk_size > memory_chunk_size || use_memory_pool == false
		|| p_memory_pool == NULL || p_head_free_memory_chunk == NULL)
	{
		// return malloc(sizeof(T)); // ::operator new (sizeof(T));
		return ::operator new (sizeof(T));
	}

	// Get the address of the next available memory chunk
	struct Chunk *p_curr = p_head_free_memory_chunk;

	// Update the address of the next available memory chunk
	p_head_free_memory_chunk = p_curr->p_next;

	if (p_head_free_memory_chunk != NULL)
	{
		p_head_free_memory_chunk->p_prev = NULL;
	}

	// Get the address of the next allocated memory chunk
	p_curr->p_next = p_head_allocated_memory_chunk;

	if (p_head_allocated_memory_chunk != NULL)
	{
		p_head_allocated_memory_chunk->p_prev = p_curr;
	}

	p_head_allocated_memory_chunk = p_curr;

	return (void *)((char *)p_curr + sizeof(struct Chunk));
}

// Requirement: Chunk free – Returns the allocated memory chunk into the memory pool – O(1).
// Requirement (bonus): Make sure that the returned chunk belongs to the memory pool 
// (to prevent allocation from one pool and free to another).
//
// Purpose:
//		Deallocates (recycles) a memory chunk by returning it to the memory pool. 
// Input parameters:
//		p - pointer to an address of a memory chunk.
// Output parameters:
//		None.
template<typename T>
void MemoryPool<T>::Deallocate(void *p) throw()
{
	// Check to see if the pointer points to a valid memory chunk address within the memory pool address space.
	// If true, remove it from the allocated memory linked-list and re-insert it into the deallocated memory linked list; 
	// otherwise, invoke the system call function "free".
	if (p_memory_pool < p && p < ((void *)((char *)p_memory_pool + memory_pool_size)))
	{
		struct Chunk * p_curr = (struct Chunk *)((char *)p - sizeof(struct Chunk));

		p_head_allocated_memory_chunk = p_curr->p_next;

		if (p_head_allocated_memory_chunk != NULL)
		{
			p_head_allocated_memory_chunk->p_prev = NULL;
		}

		p_curr->p_next = p_head_free_memory_chunk;

		if (p_head_free_memory_chunk != NULL)
		{
			p_head_free_memory_chunk->p_prev = p_curr;
		}

		p_head_free_memory_chunk = p_curr;
	}
	else
	{
		// free(p);
		delete p;
	}
}


// Purpose:
//		Creates an instance of type T.
//		Uses the allocate function to provide a chunk of memory from the pre-allocated memory pool.
//		Constructs an instance of the object using placement new operator.
//      Employs C++11 variadic template feature.
// Input parameters:
//		p - pointer to an address of a memory chunk.
// Output parameters:
//		Pointer of type T which points to the newly created instance.
template<typename T>
template<typename... Args>
T * MemoryPool<T>::Create(Args && ...args) throw()
{
	T * p = (T*)(Allocate(sizeof(T)));

	// Requirement: Please note the embedded systems usually do not use RTTI & Exception mechanisms.
	// try
	// {
	new (p)T(forward<Args>(args)...);
	// }
	// catch (...)
	// {
	// 	Deallocate(p);
	// 	throw;
	// }

	return p;
}

// Purpose:
//		Destroys and removes an instance of type T (recycles released chunk of memory back to the memory pool).
// Input parameters:
//		p - pointer to an object of type T stored at an address of a memory chunk.
// Output parameters:
//		None.
template<typename T>
void MemoryPool<T>::Remove(T * p) throw()
{
	p->~T();
	Deallocate(p);
}

#endif