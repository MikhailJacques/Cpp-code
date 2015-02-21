/*
WHAT IS MEMORY POOL

The malloc or new operators that we normally use for dynamic memory management 
in C/C++ are rather slow and have some memory overhead attached to them. 
This is acceptable if we make a handful of calls and ask for large chunks of memory, 
but if we need to store many small objects, the time and memory overhead may be unacceptable 
for high performance programs. This is where a memory pool comes into play. 
A memory pool allocates memory in big chunks and splits the memory into smaller pieces. 
Every time we request memory, one of these small chunks is returned instead making a call 
to the OS or the heap allocator. We can only use a memory pool if we know the size of the 
objects beforehand.

Advantages of memory pool:
- It is substantially faster than malloc or new.
- There is almost no memory overhead since the size of each object is known beforehand 
  (i.e. no need to store allocation metadata).
- There is little to no memory fragmentation.
- We do not need to free object one by one. The allocator will free all the memory it 
  allocated once its destructor is called (the objects must have a default destructor).

Disadvantages of memory pool:
 - Objects have a fixed size which must be known beforehand. This is usually not a problem and 
   mostly the case if you need to allocate them in a bunch.
 - We may need to fine tune them for our specific application. Fortunately, this is made very 
   easy with the use of template classes.


WHEN TO USE MEMORY POOL

We should use a memory pool when we need to allocate many objects of the same size. 
This is usually the case when we implement common data structures like linked lists, 
binary search trees, hash tables with chaining and so on. Using a memory pool in these 
cases increases performance by several folds and reduce wasted memory substantially.


IMPLEMENTATION

This is a C++98 version of a template class implementation of a memory pool allocator that is
very simple to use and extremely fast with minimal overhead for each allocation/deallocation.
The provided class is mostly compliant with the C++ Standard Library with a few exceptions.
The C++11 version has better memory management, but it requires C++11 features.
It uses variadic templates for perfect argument forwarding and some other optimizations.
Unfortunately, MS VS 2013 does not support all the necessary C++11 features.
Hence, a C++98 version is currently used here.


LIMITATIONS OF THE IMPLEMENTATION

This implementation of the memory pool:
- Cannot allocate multiple objects with a single call to allocate and simply ignores
  the count value passed to the allocate/deallocate functions. While fixing this is not
  too hard, it would deteriorate performance and create memory fragmentation.
- Not thread safe. We should create a different instance for each thread (suggested)
  or find some way of scheduling queries to the allocator.


NOTEWORTHY POINTS

This code handles alignment for the objects automatically, which is necessary for high 
performance memory access and may even be required on certain processors. However, it 
assumes the memory returned by operator new is "aligned enough" to store a pointer.

The allocator does not free any memory until it is destructed (though it does re-use the memory 
that we deallocate). This is acceptable if we only allocate objects, or if we deallocate and 
re-allocate objects equally frequently. This is only a problem if we have peaks of high memory 
usage and we expect the memory to be freed during low memory periods. This was a design choice 
to substantially improve the performance, and under most cases we do not need to worry about this. 
It is good to keep this in mind for projects that require special handling of the memory though.

*/

#ifndef MEMORY_POOL_2_H
#define MEMORY_POOL_2_H

#include <limits.h>
#include <stddef.h>

#define BLOCK_SIZE 2048

// T is the type of the objects to allocate, which can be any object.
// BlockSize is the size of the memory chunks in bytes the allocator asks from the system. 
// It has to be large enough to contain at least two pointers or two T objects, depending on which is bigger.
template <typename T, size_t BlockSize = BLOCK_SIZE>
class MemoryPool_2
{
	public:
		
		// Member types
		// Reference: http://www.cplusplus.com/reference/memory/allocator/
		typedef T               value_type;
		typedef T*              pointer;
		typedef T&              reference;
		typedef const T*        const_pointer;
		typedef const T&        const_reference;
		typedef size_t          size_type;
		typedef ptrdiff_t       difference_type;

		template <typename U> struct rebind {
			typedef MemoryPool_2<U> other;
		};

		// Member functions
		// Reference: http://www.cplusplus.com/reference/memory/allocator/
		MemoryPool_2() throw();
		MemoryPool_2(const MemoryPool_2& MemoryPool_2) throw();
		template <class U> MemoryPool_2(const MemoryPool_2<U>& MemoryPool_2) throw();

		~MemoryPool_2() throw();

		pointer address(reference x) const throw();
		const_pointer address(const_reference x) const throw();

		// Currently memory pool can only allocate one object at a time (n and hint are ignored)
		pointer allocate(size_type n = 1, const_pointer hint = 0) throw();
		void deallocate(pointer p, size_type n = 1);

		size_type max_size() const throw();

		void construct(pointer p, const_reference val);
		void destroy(pointer p);

		// Apart from the standard allocator functions, MemoryPool_2 defines two new functions: 
		// newElement(Args...) and deleteElement(pointer). 
		// These functions behave just like the standard new and delete functions and eliminate 
		// the need to call constructors and destructors separately. 
		// The only difference is that they can only allocate space for a type T object. 
		pointer newElement(const_reference val);
		void deleteElement(pointer p);

	private:

		union Slot_ 
		{
			value_type element;
			Slot_* next;
		};

		typedef char* data_pointer_;
		typedef Slot_ slot_type_;
		typedef Slot_* slot_pointer_;

		slot_pointer_ currentBlock_;
		slot_pointer_ currentSlot_;
		slot_pointer_ lastSlot_;
		slot_pointer_ freeSlots_;

		size_type padPointer(data_pointer_ p, size_type align) const throw();
		void allocateBlock();

		// static_assert(BlockSize >= 2 * sizeof(slot_type_), "BlockSize too small.");
};


// Construct allocator object
template <typename T, size_t BlockSize>
MemoryPool_2<T, BlockSize>::MemoryPool_2() throw()
{
	currentBlock_ = 0;
	currentSlot_ = 0;
	lastSlot_ = 0;
	freeSlots_ = 0;
}

// Copy construct allocator object (currently not implemented)
template <typename T, size_t BlockSize>
MemoryPool_2<T, BlockSize>::MemoryPool_2(const MemoryPool_2& MemoryPool_2) throw()
{
	MemoryPool_2();
}

// Copy construct allocator object (currently not implemented)
template <typename T, size_t BlockSize> 
template<class U>
MemoryPool_2<T, BlockSize>::MemoryPool_2(const MemoryPool_2<U>& MemoryPool_2) throw()
{
	MemoryPool_2();
}

// Destruct allocator object 
template <typename T, size_t BlockSize>
MemoryPool_2<T, BlockSize>::~MemoryPool_2() throw()
{
	slot_pointer_ curr = currentBlock_;

	while (curr != 0) 
	{
		slot_pointer_ prev = curr->next;
		operator delete(reinterpret_cast<void*>(curr));
		curr = prev;
	}
}

// Return address of object T
template <typename T, size_t BlockSize>
inline 
typename MemoryPool_2<T, BlockSize>::pointer
MemoryPool_2<T, BlockSize>::address(reference x) const throw()
{
	return &x;
}

// Return address of object T
template <typename T, size_t BlockSize>
inline 
typename MemoryPool_2<T, BlockSize>::const_pointer
MemoryPool_2<T, BlockSize>::address(const_reference x)
const throw()
{
	return &x;
}

// Allocate block of storage
template <typename T, size_t BlockSize>
inline 
typename MemoryPool_2<T, BlockSize>::pointer
MemoryPool_2<T, BlockSize>::allocate(size_type, const_pointer)
{
	if (freeSlots_ != 0) 
	{
		pointer result = reinterpret_cast<pointer>(freeSlots_);
		freeSlots_ = freeSlots_->next;

		return result;
	}
	else 
	{
		if (currentSlot_ >= lastSlot_)
			allocateBlock();

		return reinterpret_cast<pointer>(currentSlot_++);
	}
}

// Release block of storage
template <typename T, size_t BlockSize>
inline 
void 
MemoryPool_2<T, BlockSize>::deallocate(pointer p, size_type)
{
	if (p != 0) 
	{
		reinterpret_cast<slot_pointer_>(p)->next = freeSlots_;
		freeSlots_ = reinterpret_cast<slot_pointer_>(p);
	}
}

// Maximum size possible to allocate
template <typename T, size_t BlockSize>
inline 
typename MemoryPool_2<T, BlockSize>::size_type
MemoryPool_2<T, BlockSize>::max_size() const throw()
{
	// Currently does NOT do what is ultimately required of it.
	// Provides temporary functionality necessary to use the class.
	size_type maxBlocks = 1; //  -1 / BlockSize;

	return (BlockSize - sizeof(data_pointer_)) / sizeof(slot_type_) * maxBlocks;
}

// Construct an object
template <typename T, size_t BlockSize>
inline 
void
MemoryPool_2<T, BlockSize>::construct(pointer p, const_reference val)
{
	new (p)value_type(val);
}

// Destroy an object
template <typename T, size_t BlockSize>
inline 
void
MemoryPool_2<T, BlockSize>::destroy(pointer p)
{
	p->~value_type();
}

template <typename T, size_t BlockSize>
inline 
typename MemoryPool_2<T, BlockSize>::pointer
MemoryPool_2<T, BlockSize>::newElement(const_reference val)
{
	pointer result = allocate();
	construct(result, val);
	return result;
}

template <typename T, size_t BlockSize>
inline 
void
MemoryPool_2<T, BlockSize>::deleteElement(pointer p)
{
	if (p != 0) 
	{
		p->~value_type();
		deallocate(p);
	}
}

template <typename T, size_t BlockSize>
inline 
typename MemoryPool_2<T, BlockSize>::size_type
MemoryPool_2<T, BlockSize>::padPointer(data_pointer_ p, size_type align) const throw()
{
	size_t result = reinterpret_cast<size_t>(p);
	return ((align - result) % align);
}

template <typename T, size_t BlockSize>
void
MemoryPool_2<T, BlockSize>::allocateBlock() throw()
{
	// Allocate space for the new block and store a pointer to the previous one
	data_pointer_ newBlock = reinterpret_cast<data_pointer_>(operator new(BlockSize));

	reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
	currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);

	// Pad block body to satisfy the alignment requirements for elements
	data_pointer_ body = newBlock + sizeof(slot_pointer_);
	size_type bodyPadding = padPointer(body, sizeof(slot_type_));

	currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
	lastSlot_ = reinterpret_cast<slot_pointer_>(newBlock + BlockSize - sizeof(slot_type_) + 1);
}

#endif // MEMORY_POOL_2_H