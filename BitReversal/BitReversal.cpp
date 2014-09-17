// Author: Mikhail Jacques
// Date: 17-Sep-2014

#include <bitset>
#include <iostream>
#include <assert.h> 
using namespace std;

// Purpose: to reverse bits in an unsigned short integer 
// Input: an unsigned short integer whose bits are to be reversed
// Output: an unsigned short integer with the reversed bits of the input one
unsigned short ReverseBits_1( unsigned short a )
{
	// declare and initialize number of bits in the unsigned short integer
	const char num_bits = sizeof(a) * CHAR_BIT;

	// declare and initialize bitset representation of integer a
	bitset<num_bits> bitset_a(a);			

	// declare and initialize bitset representation of integer b (0000000000000000)
	bitset<num_bits> bitset_b(0);					

	// declare and initialize bitset representation of mask (0000000000000001)
	bitset<num_bits> mask(1);			

	for ( char i = 0; i < num_bits; ++i )
	{
		bitset_b = (bitset_b << 1) | bitset_a & mask;
		bitset_a >>= 1;
	}

	return (unsigned short) bitset_b.to_ulong();
}

unsigned short ReverseBits_2( unsigned short a )	
{  
	// declare and unitialize integer b that will be reversed bits of a; first get LSB of a
	unsigned short b = a;				

	// declare and initialize number of bits in the unsigned short integer
	const unsigned short num_bits = sizeof(a) * CHAR_BIT;

	// compute the extra shift that is needed at end
	unsigned short shift = num_bits - 1;		

	for (a >>= 1; a; a >>= 1)
	{   
		b <<= 1;
		b |= a & 1;
		shift--;
	}

	// shift when a's highest bits are all zero
	b <<= shift; 

	return b;
}

template <typename T>
T ReverseBits_3( T a, unsigned short num_bits = sizeof(T) * CHAR_BIT )
{
	assert( num_bits <= sizeof(T) * CHAR_BIT );

	T b = 0;

	for ( char i = 0; i < num_bits; ++i, a >>= 1 )
		b = (b << 1) | (a & 0x01);

	return b;
}

template<unsigned short num_bits>
const bitset<num_bits> ReverseBits_4( const bitset<num_bits> & a )
{
	// declare and initialize bitset representation of a
	bitset<num_bits> b;

	for ( unsigned short i = 0, j = num_bits - 1; i < num_bits; ++i, --j )
		b[j] = a[i];

	return b;
}


// Recursive version
unsigned short ReverseBits_5(unsigned short a, unsigned short num_bits)
{
    unsigned int b;
    unsigned int mask = 0;

    mask = (0x1 << (num_bits/2)) - 1;

    if ( num_bits == 1 ) 
		return a;

    b = ReverseBits_5(a >> num_bits/2, num_bits/2) | ReverseBits_5((a & mask), num_bits/2) << num_bits/2;
    
	return b;
}


void PrintBits( unsigned short a )
{
	// declare and initialize bitset representation of a
	bitset<sizeof(a) * CHAR_BIT> bitset(a);

	// print out bits
    cout << bitset << endl;
}


int main()
{
	unsigned short a = 17, b;

	// declare and initialize number of bits in the unsigned short integer
	const char num_bits = sizeof(a) * CHAR_BIT;

	cout << "Enter integer value: ";
	cin >> a;

	cout << "Original: "; 
	PrintBits(a);

	b = ReverseBits_1( a );

	cout << "Reversed: ";
	PrintBits(b);

	b = ReverseBits_2( a );

	cout << "Reversed: ";
	PrintBits(b);

	b = ReverseBits_3( a );

	cout << "Reversed: ";
	PrintBits(b);

	// declare and initialize bitset representation of a
	const bitset<num_bits> bitset(a);	

	b = (unsigned short) ReverseBits_4<num_bits>( bitset ).to_ulong();

	cout << "Reversed: ";
	PrintBits(b);

	b = ReverseBits_5( a, num_bits);

	cout << "Reversed: ";
	PrintBits(b);

	cin >> a;
}