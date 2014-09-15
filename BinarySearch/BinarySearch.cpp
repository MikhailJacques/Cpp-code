
// Returns: location of key in the list if found; -1 otherwise
// int BinarySearch1(int list[], int size, int key)
int BinarySearch1(int sortedArray[], int left, int right, int key) 
{
	int midpt;

	while (left <= right)
	{
		midpt = (int) ((left + right) / 2);
		
		if (key == sortedArray[midpt])
			return midpt;
		else if (key > sortedArray[midpt])
			left = midpt + 1;
		else
			right = midpt - 1;
	}

	return -1;
}

// http://www.fredosaurus.com/notes-cpp/algorithms/searching/binarysearch.html
// Searches sortedArray[first]..sortedArray[last] for key.  
// Returns: index of the matching element if it finds key; 
// otherwise  -(index where it could be inserted)-1.
// Parameters: sortedArray in  array of sorted (ascending) values.
// first, last in lower and upper subscript bounds
// key in  value to search for.
// returns: index of key, or -1 if key is not in the array. 
// This value can easily be transformed into the position to insert it.
//int BinarySearch2(int sortedArray[], int size, int key) 
int BinarySearch2(int sortedArray[], int first, int last, int key) 
{
	while (first <= last) 
	{ 
		int mid = (first + last) / 2;  // compute mid point
       
		if (key > sortedArray[mid]) 
			first = mid + 1;	// repeat search in top half
		else if (key < sortedArray[mid]) 
           last = mid - 1;		// repeat search in bottom half
		else
           return mid;			// found it, return position
   }

   return -1;    // failed to find key
}

// http://www.fredosaurus.com/notes-cpp/algorithms/searching/rbinarysearch.html
// Searches sortedArray[first]..sortedArray[last] for key.  
// Returns: index of the matching element if it finds key, 
//         otherwise  -(index where it could be inserted)-1.
// Parameters:
//   sortedArray in array of sorted (ascending) values.
//   first, last in lower and upper subscript bounds
//   key         in value to search for.
// Returns:
//   index of key, or -1 if key is not in the array.
int BinarySearch3(int sortedArray[], int first, int last, int key) 
{
	if (first <= last) 
	{
		// compute mid point
		int mid = (first + last) / 2;  

		if (key == sortedArray[mid]) 
			return mid;   // found it
		else if (key < sortedArray[mid]) 
           // call itself for the lower part of the array
           return BinarySearch2(sortedArray, first, mid-1, key);
		else
           // call itself for the upper part of the array
           return BinarySearch2(sortedArray, mid+1, last, key);
	}
	
	return -1;    // failed to find key
}