
#include <iostream>
#include "headers.h"
using namespace std;

int main()
{
	const int NUM_ELEMENTS = 10;
	int list[NUM_ELEMENTS] = {5,10,22,32,45,67,73,98,99,101};
	int item, location;

	cout << "Enter the item you are searching for: ";
	cin >> item;
	
	location = BinarySearch1(list, 0, 9, item);		// iterative

	location = BinarySearch2(list, 0, 9, item);		// iterative

	location = BinarySearch3(list, 0, 9, item);		// recursive
	
	if (location > -1)
		cout << "The item was found at index location " << location << endl;
	else
		cout << "The item was not found in the list\n";

	char * fullString = "My dear Alex, alex\n";
	char subString[10];

	cout << "Enter substring you are searching for: ";
	cin  >> subString;

	location = StringSearch1(fullString, subString);	// low lever, simple

	location = StringSearch2(fullString, subString);	// high level, advanced

	if (location > -1)
		cout << "The first occurence of the substring \"" << subString << "\" in the string \"" 
			<< fullString << "\" was found at index location " << location << endl;
	else
		cout << "The substring \"" << subString << "\" was not found in the string \"" << fullString << "\"" << endl;

	system("pause");

	return (0);
}