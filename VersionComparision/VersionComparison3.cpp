// Version String Comparison Algorithm
//
// Building any kind of version management system in C++, such as a package manager or 
// plugin system necessitates manipulation, comparison and printing out version strings. 

// Algorithm:
// - Tokenize the string (break the string representing each version down) into individual numeric tokens
// - Store each token as an integer
// - Compare between two respective integers in version strings

#include <cstdio>
#include <string>
#include <iostream>
#include <assert.h> 

using namespace std;

class Version
{
	int major, minor, revision, build;

	public:

		Version(const string & version)
		{
			sscanf_s(version.c_str(), "%d.%d.%d.%d", &major, &minor, &revision, &build);

			if (major < 0) major = 0;
			if (minor < 0) minor = 0;
			if (revision < 0) revision = 0;
			if (build < 0) build = 0;
		}

		bool operator < (const Version & rhs)
		{
			if (major < rhs.major)			return true;	// new version is fresher, no need to check the rest
			else if (major > rhs.major)		return false;	// current version is fresher, no need to check the rest

			if (minor < rhs.minor)			return true;
			else if (minor > rhs.minor)		return false;

			if (revision < rhs.revision)	return true;
			else if (revision > rhs.revision)return false;

			if (build < rhs.build)			return true;
			else if (build > rhs.build)		return false;

			return false;
		}

		bool operator > (const Version & rhs)
		{
			if (major > rhs.major)			return true;	// current version is fresher, no need to check the rest	
			else if (major < rhs.major)		return false;	// new version is fresher, no need to check the res
				
			if (minor > rhs.minor)			return true;
			else if (minor < rhs.minor)		return false;

			if (revision > rhs.revision)	return true;
			else if (revision < rhs.revision)return false;

			if (build > rhs.build)			return true;
			else if (build < rhs.build)		return false;

			return false;
		}

		bool operator == (const Version & rhs)
		{
			return major == rhs.major 
				&& minor == rhs.minor 
				&& revision == rhs.revision 
				&& build == rhs.build;
		}

		friend ostream & operator << (ostream & stream, const Version & version) 
		{
			stream << version.major;
			stream << '.';
			stream << version.minor;
			stream << '.';
			stream << version.revision;
			stream << '.';
			stream << version.build;

			return stream;
		}
};

void testVersionComparison() 
{
	// If the expression inside assert statement evaluates to 0, it causes an assertion failure that terminates the program.
	// Therefore, this macro is designed to capture programming errors, not user or run-time errors.
	assert((Version("3.0.1.5") < Version("3.0.2.5")) == true);
	assert((Version("3.0.2.5") < Version("3.0.1.5")) == false);
	assert((Version("3.0.1.5") == Version("3.0.1.5")) == true);
	assert((Version("3.0.12.5") < Version("3.0.1.25")) == false);
	assert((Version("3.0.1.25") < Version("3.0.12.5")) == true);
	assert((Version("3.0.1.5") > Version("3.0.1.57")) == false);
	assert((Version("3.0.1.5") == Version("3.0.1.6")) == false);
}

int main()
{
	testVersionComparison();

	Version v1("3.0.1.5");
	Version v2("3.0.2.5");

	cout << "Using token-wise version comparison algorithm (CORRECT)" << endl;

	cout << "\nIs \"3.0.1.5\" less than \"3.0.2.5\"? " 
		<< "\tExpected: Yes,\tComputed: " 
		<< ((v1 < v2) ? "Yes" : "No"); 

	Version v3("3.0.2.5");
	Version v4("3.0.1.5");

	cout << "\nIs \"3.0.2.5\" less than \"3.0.1.5\"? " 
		<< "\tExpected: No,\tComputed: " 
		<< ((v3 < v4) ? "Yes" : "No"); 

	Version v5("3.0.1.5");
	Version v6("3.0.1.5");

	cout << "\nIs \"3.0.1.5\" less than \"3.0.1.5\"? " 
		<< "\tExpected: No,\tComputed: " 
		<< ((v5 < v6) ? "Yes" : "No"); 

	Version v7("3.0.12.5");
	Version v8("3.0.1.25");

	cout << "\nIs \"3.0.12.5\" less than \"3.0.1.25\"? " 
		<< "\tExpected: No,\tComputed: " 
		<< ((v7 < v8) ? "Yes" : "No"); 

	Version v9("3.0.1.25");
	Version v10("3.0.12.5");

	cout << "\nIs \"3.0.1.25\" less than \"3.0.12.5\"? " 
		<< "\tExpected: Yes,\tComputed: " 
		<< ((v9 < v10) ? "Yes" : "No"); 

	Version v11("3.0.1.5");
	Version v12("3.0.1.57");

	cout << "\nIs \"3.0.1.5\" less than \"3.0.1.57\"? " 
		<< "\tExpected: Yes,\tComputed: " 
		<< ((v11 < v12) ? "Yes" : "No"); 

	Version v13("3.0.1.5");
	Version v14("3.0.1.6");

	cout << "\nIs \"3.0.1.5\" less than \"3.0.1.6\"? " 
		<< "\tExpected: Yes,\tComputed: " 
		<< ((v13 < v14) ? "Yes" : "No");

	cout << endl;

	// cout << "\n\nPrinting version (3.0.1.5): " << Version("3.0.1.5") << endl;

	cin.get();

	return 0;
}