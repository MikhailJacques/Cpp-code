#include <iostream>
#include <string>
using namespace std;

int StringSearch1(const char * fullString, const char * subString)
{
	int i = 0, j = 0, index = 0, len = 0;
	bool flag = false;

	len = strlen(subString);
 
	while (fullString[i] != EOF)
	{
		if (fullString[i] == subString[j])
		{
			if ( !flag )
				index = i;
			
			i++;
			j++;
			flag = true;
        
			if (j == len)
				return index;
		}
		else
		{
			if ( flag )
			{
				j = 0;
				flag = false;
			}
			else
				i++;
		}
	}

	return -1;
}


int StringSearch2(string input, const string &substring)
{
	// If the substring length is bigger than input length, what's the point to continue?
	if (substring.length() > input.length())
		return -1;

	// If both arguments are equal, what's the point to continue?
	if (input == substring)
		return input[0];

	unsigned int index = 0;

	int substr_length = substring.length();		// Set 'substring' length

	while (index < input.length())
	{
		string to_compare = ""; 
		
		// Position in 'input' of the first 'substring' character
		int f_pos = input.find(substring[0]); 
		
		if (f_pos == input.npos) // Couldn't find?
			return -1;
		
		// Build a substring
		for (int i = f_pos; i <= substr_length + f_pos - 1; i++)
			to_compare += input[i]; 

		// Compare it with the 'substring' parameter
		if (to_compare != substring)
		{
			// Position in 'input' of the first 'substring' character
			int find = input.find(substring[0]); 
			
			// Erase the substring if it wasn't equal with 'substring'
			if (find != input.npos)
				input.erase(find, substr_length - 1); 
		}
		else
			return f_pos; // true

		index++;
	}

	// To avoid a warning
	return -1;
}