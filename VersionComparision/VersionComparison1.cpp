// Version String Comparison Algorithm

// Building any kind of version management system in C++, such as a package manager or 
// plugin system necessitates manipulation, comparison and printing out version strings. 

#include <queue>		// std::queue
#include <string>       // std::string
#include <sstream>      // std::istringstream
#include <iostream>     // std::cout

using namespace std;

// Parse_1
// Remove the periods from the unparsed string and store 
// them in a character array in one continuous sequence
void Parse_1(const string & unparsed, char * parsed)
{
	char token;
	unsigned int i = 0;

	istringstream parser(unparsed);

	while(parser >> token)
		if (token != '.')
			parsed[i++] = token;

	parsed[i] = '\0';
}

// Parse_2
// Break the unparsed string down into individual tokens,
// convert them into integers and store them in a queue
void Parse_2(const string & unparsed, queue<int> & parsed)
{
	string s;
	stringstream ss(unparsed);

	while (getline(ss, s, '.')) 
		parsed.push(stoi(s));
}

// VersionComparison_1
//
// On the surface it might look correct, but it is inherently logically incorrect.
// It works correctly ONLY if the number of digits in each token is identical. 
// Each version consists of four separate digits representing 
// major, minor, revision and build numbers, respectively.
// The following different versions, when parsed using the Parse_1 algorithm, 
// translate into identical sequence of digit, for example:
// 
// "3.0.12.5" > "3.0.1.25" = 30125
// "3.0.1.25" < "3.0.12.5" = 30125
//
// Hence, the lexicographical_compare will not know the difference between the two different versions.
// It returns true if the version_a compares lexicographically less than version_b; returns false otherwise
bool VersionComparison_1(const string & version_a, const string & version_b)
{
	char * parsed_version_a = new char [version_a.length() + 1];
	char * parsed_version_b = new char [version_b.length() + 1];

    Parse_1(version_a, parsed_version_a);
    Parse_1(version_b, parsed_version_b);

	// cout << "Parsed out version a: " << parsed_version_a << endl;
	// cout << "Parsed out version b: " << parsed_version_b << endl;

	// bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2);
	// Returns true if the range [first1, last1) compares lexicographically less than the range [first2, last2).
	// A lexicographical comparison is the kind of comparison generally used to sort words alphabetically in dictionaries; 
	// It involves comparing sequentially the elements that have the same position in both ranges against each other until 
	// one element is not equivalent to the other. 
	// The result of comparing these first non-matching elements is the result of the lexicographical comparison.
	// If both sequences compare equal until one of them ends, the shorter sequence is lexicographically less than the longer one.

	// first1, last1 - Input iterators to the initial and final positions of the first sequence. 
	// The range used is [first1, last1), which contains all the elements between first1 and last1, 
	// including the element pointed by first1, but not the element pointed by last1.

	// Lexicographical comparison is a operation with the following properties:
	// - Two ranges are compared element by element.
	// - The first mismatching element defines which range is lexicographically less or greater than the other.
	// - If one range is a prefix of another, the shorter range is lexicographically less than the other.
	// - If two ranges have equivalent elements and are of the same length, then the ranges are lexicographically equal.
	// - An empty range is lexicographically less than any non-empty range.
	// - Two empty ranges are lexicographically equal.

	// Returns true if the first range compares lexicographically less than the second.
	// Returns false otherwise (including when all the elements of both ranges are equivalent).

    return lexicographical_compare(parsed_version_a, parsed_version_a + strlen(parsed_version_a), 
		parsed_version_b, parsed_version_b + strlen(parsed_version_b));
}

// Returns TRUE if version_a (current version) is less than version_b (new version)
// Assumption: Both version_a and version_b must have equal number of tokens, irrespective of the individual token's length
// Addresses special cases such as the following one by comparing individual tokens:
// "3.0.12.5" > "3.0.1.25"
// "3.0.1.25" < "3.0.12.5"
// Returns true if version_a compares token-wise less than version_b; returns false otherwise.
bool VersionComparison_2(const string & version_a, const string & version_b)
{
	int a, b;
	queue<int> parsed_version_a, parsed_version_b;

	Parse_2(version_a, parsed_version_a);
	Parse_2(version_b, parsed_version_b);

	while (!parsed_version_a.empty())
	{
		a = parsed_version_a.front();
		parsed_version_a.pop();

		b = parsed_version_b.front();
		parsed_version_b.pop();

		if (a < b) 
			return true;	// version b is fresher, no need to check the rest
		else if ( a > b )
			return false;	// version a is fresher, no need to check the rest
	}

	return false;			// versions are equal
}

// Main application that tests the functionality of both algorithms
int main()
{
	cout << "Using lexicographic version comparison algorithm (FLAWED)" << endl;

	cout << "\nIs \"3.0.1.5\" less than \"3.0.2.5\"? " 
		<< "\tExpected: Yes,\tComputed: " 
		<< (VersionComparison_1("3.0.1.5", "3.0.2.5") ? "Yes" : "No"); 

	cout << "\nIs \"3.0.2.5\" less than \"3.0.1.5\"? " 
		<< "\tExpected: No,\tComputed: " 
		<< (VersionComparison_1("3.0.2.5", "3.0.1.5") ? "Yes" : "No"); 

	cout << "\nIs \"3.0.1.5\" less than \"3.0.1.5\"? " 
		<< "\tExpected: No,\tComputed: " 
		<< (VersionComparison_1("3.0.2.5", "3.0.1.5") ? "Yes" : "No"); 

	cout << "\nIs \"3.0.12.5\" less than \"3.0.1.25\"? " 
		<< "\tExpected: No,\tComputed: " 
		<< (VersionComparison_1("3.0.12.5", "3.0.1.25") ? "Yes" : "No"); 

	cout << "\nIs \"3.0.1.25\" less than \"3.0.12.5\"? " 
		<< "\tExpected: Yes,\tComputed: " 
		<< (VersionComparison_1("3.0.1.25", "3.0.12.5") ? "Yes" : "No"); 

	cout << "\nIs \"3.0.1.5\" less than \"3.0.1.57\"? " 
		<< "\tExpected: Yes,\tComputed: " 
		<< (VersionComparison_1("3.0.1.5", "3.0.1.57") ? "Yes" : "No"); 

	cout << "\nIs \"3.0.1.5\" less than \"3.0.1.6\"? " 
		<< "\tExpected: Yes,\tComputed: " 
		<< (VersionComparison_1("3.0.1.5", "3.0.1.6") ? "Yes" : "No");



	cout << "\n\nUsing individual tokens version comparison algorithm (CORRECT)" << endl;

	cout << "\nIs \"3.0.1.5\" less than \"3.0.2.5\"? " 
		<< "\tExpected: Yes,\tComputed: " 
		<< (VersionComparison_2("3.0.1.5", "3.0.2.5") ? "Yes" : "No"); 

	cout << "\nIs \"3.0.2.5\" less than \"3.0.1.5\"? " 
		<< "\tExpected: No,\tComputed: " 
		<< (VersionComparison_2("3.0.2.5", "3.0.1.5") ? "Yes" : "No"); 

	cout << "\nIs \"3.0.1.5\" less than \"3.0.1.5\"? " 
		<< "\tExpected: No,\tComputed: " 
		<< (VersionComparison_2("3.0.2.5", "3.0.1.5") ? "Yes" : "No"); 

	cout << "\nIs \"3.0.12.5\" less than \"3.0.1.25\"? " 
		<< "\tExpected: No,\tComputed: " 
		<< (VersionComparison_2("3.0.12.5", "3.0.1.25") ? "Yes" : "No"); 

	cout << "\nIs \"3.0.1.25\" less than \"3.0.12.5\"? " 
		<< "\tExpected: Yes,\tComputed: " 
		<< (VersionComparison_2("3.0.1.25", "3.0.12.5") ? "Yes" : "No"); 

	cout << "\nIs \"3.0.1.5\" less than \"3.0.1.57\"? " 
		<< "\tExpected: Yes,\tComputed: " 
		<< (VersionComparison_2("3.0.1.5", "3.0.1.57") ? "Yes" : "No"); 

	cout << "\nIs \"3.0.1.5\" less than \"3.0.1.6\"? " 
		<< "\tExpected: Yes,\tComputed: " 
		<< (VersionComparison_2("3.0.1.5", "3.0.1.6") ? "Yes" : "No");


	cout << endl;

	cin.get();
}