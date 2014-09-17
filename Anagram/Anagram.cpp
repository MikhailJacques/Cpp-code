// Anagram. Two words are said to be anagrams of each other if the letters from one word can be rearranged to form the other word. 
// From the above definition it is clear that two strings are anagrams if all characters in both strings occur same number of times. 
// For example "xyz" and "zxy" are anagram strings, here every character 'x', 'y' and 'z' occur only one time in both strings. 

#include <map>
#include <string>
#include <cctype>
#include <iostream> 
#include <algorithm>
#include <unordered_map>

using namespace std;

bool IsAnagram_1( string w1, string w2 )
{
	// Compare string lengths
	if ( w1.length() != w2.length() )
		return false;

	sort( w1.begin(), w1.end() );
	sort( w2.begin(), w2.end() );

	return w1 == w2;
}

map<char, size_t> key_word( const string & w )
{
	// Declare a map which is an associative container that will store a key value and a mapped value pairs
	// The key value is a letter in a word and the maped value is the number of times this letter appears in the word
	map<char, size_t> m;

	// Step over the characters of string w and use each character as a key value in the map
	for ( auto & c : w )
	{
		// Access the mapped value directly by its corresponding key using the bracket operator
		++m[toupper( c )];	
	}

	return ( m );
}


bool IsAnagram_2( const string & w1, const string & w2 )  
{
	// Compare string lengths
	if ( w1.length() != w2.length() )
		return false;

	return ( key_word( w1 ) == key_word( w2 ) );
}


bool IsAnagram_3( const string & w1, const string & w2 )  
{
	// Compare string lengths
	if ( w1.length() != w2.length() )
		return false;

	// Instantiate a count map, std::unordered_map<char, unsigned int> m
	unordered_map<char, size_t> m;

	// Loop over the characters of string w1 incrementing the count for each character
	for ( auto & c : w1 )
	{
		// Access the mapped value directly by its corresponding key using the bracket operator
		++m[toupper(c)];	
	}

	// Loop over the characters of string w2 decrementing the count for each character
	for ( auto & c : w2 )
	{
		// Access the mapped value directly by its corresponding key using the bracket operator
		--m[toupper(c)];	
	}

	// Check to see if the mapped values are all zeros
	for ( auto & c : w2 )
	{
		if ( m[toupper(c)] != 0 )
			return false;
	}

	return true;
}


int main( )
{
	string word1, word2;

	cout << "Enter first word: ";
	cin >> word1;

	cout << "Enter second word: ";
	cin >> word2;

	if ( IsAnagram_1( word1, word2 ) )
		cout << "\nAnagram" << endl;
	else 
		cout << "\nNot Anagram" << endl;
	

	if ( IsAnagram_2( word1, word2 ) )
		cout << "\nAnagram" << endl;
	else 
		cout << "\nNot Anagram" << endl;

	
	if ( IsAnagram_3( word1, word2 ) )
		cout << "\nAnagram" << endl;
	else 
		cout << "\nNot Anagram" << endl;

	system("pause");

	return 0;
}