// Version String Comparison Algorithm

// Building any kind of version management system in C++, such as a package manager or 
// plugin system necessitates manipulation, comparison and printing out version strings. 

#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>

using namespace std;

class Version
{
    // An internal utility structure just used to make the copy in the constructor easy to write.
    struct VersionToken
    {
        int value;
        operator int() const { return value; }
    };

    friend istream & operator>>(istream & str, Version::VersionToken & digit);

    public:

        Version(string const & version)
        {
            // To make processing easier in VersionToken prepend a '.'
            stringstream version_stream(string(".") + version);

            // Copy all parts of the version number into the version_info vector.
            copy( istream_iterator<VersionToken>(version_stream), 
				istream_iterator<VersionToken>(), back_inserter(version_info) );
        }

        // Test if two version numbers are the same. 
        bool operator<(Version const & rhs) const
        {
            return lexicographical_compare(version_info.begin(), version_info.end(), 
				rhs.version_info.begin(), rhs.version_info.end());
        }

    private:

        vector<int> version_info;
};

// Read a single token (can be composed of one or more digits) from the version. 
istream & operator>>(istream & str, Version::VersionToken & digit)
{
    str.get();
    str >> digit.value;
    return str;
}

// Main application that tests the functionality of the algorithm
int main()
{
	Version v1("3.0.1.5");
	Version v2("3.0.2.5");

	cout << "Using lexicographic version comparison algorithm (CORRECT)" << endl;

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

	cin.get();
}