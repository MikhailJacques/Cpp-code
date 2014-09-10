// TheBlocksProblem.cpp : Defines the entry point for the console application

#include <string>
#include <vector>
#include <iostream>

using namespace std;

// Purpose: To move block A onto block B, where A and B are block numbers.
// Algorithm: Puts block A onto block B after returning any blocks that are stacked on top of blocks A and B to their initial positions.
// Parms: A pointer to the array of block stacks, the actual number of blocks that are stored in the entire 'block world', block A and block B.
void moveOnto(vector<int>* stacks, int num_stacks, int a, int b);

// Purpose: To move block A over B where A and B are block numbers.
// Algorithm: Puts block A onto the top of the stack containing block B, after returning any blocks that are stacked on top of block A to their initial positions.
// Parms: A pointer to the array of block stacks, the actual number of blocks that are stored in the entire 'block world', block A and block B.
void moveOver(vector<int>* stacks, int num_stacks, int a, int b);

// Purpose: To move the pile of blocks consisting of block A, and any blocks that are stacked above block A, onto block B. 
// Algorithm: All blocks on top of block B are moved to their initial positions prior to the pile taking place. 
//            The blocks stacked above block A retain their order when moved.
// Parms: A pointer to the array of block stacks, the actual number of blocks that are stored in the entire 'block world', block A and block B.
void pileOnto(vector<int>* stacks, int num_stacks, int a, int b);

// Purpose: To put the pile of blocks consisting of block A, and any blocks that are stacked above block A, onto the top of the stack containing block B. 
// Algorithm: The blocks stacked above block A retain their original order when moved.
// Parms: A pointer to the array of block stacks, the actual number of blocks that are stored in the entire 'block world', block A and block B.
void pileOver(vector<int>* stacks, int num_stacks, int a, int b);

// Purpose: To return all blocks that are stacked on top of a certain block (block_num) to their initial positions.
// Algorithm: Iteratively examines blocks on top of the stack and removes them until the specified block in the argument has been reached.
// Parms: A pointer to the array of block stacks, the stack number from which to remove the blocks and the block number on which the blocks to be removed are stacked.
void moveToInitialPosition( vector<int>* stacks, int stack_num, int block_num );

// Purpose: To check if both blocks are currently in the same stack.
// Algorithm: Iteratively steps over the block stacks one stack at a time and examines if both blocks in question reside in any given stack; time complexity: O(n^2) 
// Parms: A pointer to the array of block stacks, the actual number of blocks that are stored in the entire 'block world', block A and block B.
bool areInSameStack(vector<int>* stacks, int num_stacks, int a, int b);

// Purpose: To print out the layout of the simulated block world.
// Algorithm:  Steps over the block stacks (implemented as array of vectors) and prints out the contents of each stack in the reverse order, 
// i.e. bottom-up as opposed to the stack's natural order top-down.
// Parms: A pointer to the array of block stacks and the actual number of blocks that are stored in the entire 'block world'.
void print( vector<int>* stacks, int num_stacks );



int main()
{
	int a, b;										// User-defined block numbers
	int num_stacks;									// User-defined number of block stacks
	string first_part, second_part, operation;		// User-defined operations
		
	// Declare array of 25 vectors since the number of block stacks in the simulated block world is limited to 25
	// Note: Not an optimal way to allocate memory
	vector<int> stacks[25]; 

	// Read in the number of block stacks to be used in the simulated block world
	cin >> num_stacks;

	// Fill in the array of vector containers with the user-defined number of block stacks; each container initially stores only one block
	// Warning: the user-defined number of block stacks must not exceed 25; otherwise an exception will be thrown which is currently not being catched
	for (int i = 0; i < num_stacks; i++)
	{
		stacks[i].push_back( i );
	}

	// Enter the do-while loop and execute it until the user terminates it by entering the "quit" operation
	do 
	{
		// Read in the first part of the user-supplied command
		cin >> first_part;

		// Check to see if the command is to terminate the execution of the while loop
		if ( first_part == "quit" ) 
			break;
		
		// Read in the first block number (the 'travelling' block), the second part of the user-supplied command and the second block number (the 'hosting' block)
		cin >> a >> second_part >> b;

		// Check to see if the user supplied an illegal command
		// Illegal command conditions: either a is equal to b or a and b are located in the same stack of blocks 
		if ( a == b || areInSameStack(stacks, num_stacks, a, b) )
		{
			// Ignore the illegal command by transfering control to the beginning of the while loop
			continue;
		}

		// Construct operation description
		operation = first_part + '_' + second_part;
		
		if ( operation == "move_onto" )
		{
			moveOnto(stacks, num_stacks, a, b);
		}
		else if ( operation == "move_over" )
		{
			moveOver(stacks, num_stacks, a, b);
		}
		else if ( operation == "pile_onto" )
		{
			pileOnto(stacks, num_stacks, a, b);
		}
		else if ( operation == "pile_over" )
		{
			pileOver(stacks, num_stacks, a, b);
		}
		else
		{
			// do nothing
		}

	} while ( first_part != "quit" );
	
	print( stacks, num_stacks );

	return 0;
}


// Purpose: To move block A onto block B, where A and B are block numbers.
// Algorithm: Puts block A onto block B after returning any blocks that are stacked on top of blocks A and B to their initial positions.
// Parms: A pointer to the array of block stacks, the actual number of blocks that are stored in the entire 'block world', block A and block B.
void moveOnto(vector<int>* stacks, int num_stacks, int a, int b)
{
	for (int stack_num = 0; stack_num < num_stacks; stack_num++)
	{
		for (unsigned int block_num = 0; block_num < stacks[stack_num].size(); block_num++)
		{
			if ( stacks[stack_num][block_num] == a )
			{
				// Move all the blocks that are stacked on top of block A to their initial positions
				moveToInitialPosition( stacks, stack_num, a );

				// Remove block A from its position
				// Note: Schematically and if and only if block A number is less than that of block B number then 
				// block A might not have been stacked on top of block B yet, but programatically it does not matter
				// as it will take place before the moveOnto function finishes its operation and exits its scope
				stacks[stack_num].pop_back();

				break;
			}
			
			if ( stacks[stack_num][block_num] == b )
			{
				// Move all the blocks that are stacked on top of block B to their initial positions
				moveToInitialPosition( stacks, stack_num, b );

				// Put block A on top of block B
				// Note: Schematically and if and only if block A number exceeds that of block B number then 
				// in the current implementation block A might still have blocks stacked on it, but programatically it does not matter
				stacks[stack_num].push_back( a );

				break;
			}
		}
	}
}

// Purpose: To move block A over B where A and B are block numbers.
// Algorithm: Puts block A onto the top of the stack containing block B, after returning any blocks that are stacked on top of block A to their initial positions.
// Parms: A pointer to the array of block stacks, the actual number of blocks that are stored in the entire 'block world', block A and block B.
void moveOver(vector<int>* stacks, int num_stacks, int a, int b)
{
	for (int stack_num = 0; stack_num < num_stacks; stack_num++ )
	{
		for (unsigned int block_num = 0; block_num < stacks[stack_num].size(); block_num++)
		{
			if ( stacks[stack_num][block_num] == a ) 
			{
				// Move all the blocks that are stacked on top of block A to their initial positions
				moveToInitialPosition( stacks, stack_num, a );
				
				// Remove block A from its position
				stacks[stack_num].pop_back();
				
				break;
			}
      
			if ( stacks[stack_num][block_num] == b )
			{
				// Put block A on top of the stack containing block B
				stacks[stack_num].push_back( a );

				break;
			}
		}
	}
}

// Purpose: To move the pile of blocks consisting of block A, and any blocks that are stacked above block A, onto block B. 
// Algorithm: All blocks on top of block B are moved to their initial positions prior to the pile taking place. 
//            The blocks stacked above block A retain their order when moved.
// Parms: A pointer to the array of block stacks, the actual number of blocks that are stored in the entire 'block world', block A and block B.
void pileOnto(vector<int>* stacks, int num_stacks, int a, int b)
{
	vector<int> stack;

	// Shortcut intended to optimize performance
	bool not_found = true;
  
	// Find the stack with the block A
	for (int stack_num = 0; not_found && (stack_num < num_stacks); stack_num++)
	{
		for (unsigned int block_num = 0; block_num < stacks[stack_num].size(); block_num++)
		{
			if ( stacks[stack_num][block_num] == a )
			{
				// Move all the blocks that are stacked above block A including the block A onto a temporary stack
				stack = vector<int>( stacks[stack_num].begin() + block_num, stacks[stack_num].end() );

				// Remove the blocks that are stacked above block A including the block A from the current stack
				stacks[stack_num].erase( stacks[stack_num].begin() + block_num, stacks[stack_num].end() );

				not_found = false;

				break;
			}
		}
	}
  
	not_found = true;

	// Find the stack with the block B
	for (int stack_num = 0; not_found && (stack_num < num_stacks); stack_num++)
	{
		for (unsigned int block_num = 0; block_num < stacks[stack_num].size(); block_num++)
		{
			if ( stacks[stack_num][block_num] == b )
			{
				// Move all the blocks that are stacked on top of block B to their initial positions
				moveToInitialPosition( stacks, stack_num, b );

				// Move all the blocks from the temporary stack (block A and all the blocks on top of it) onto the block B
				stacks[stack_num].insert( stacks[stack_num].end(), stack.begin(), stack.end() );
				
				not_found = false;

				break;
			}
		}
	}
}

// Purpose: To put the pile of blocks consisting of block A, and any blocks that are stacked above block A, onto the top of the stack containing block B. 
// Algorithm: The blocks stacked above block A retain their original order when moved.
// Parms: A pointer to the array of block stacks, the actual number of blocks that are stored in the entire 'block world', block A and block B.
void pileOver(vector<int>* stacks, int num_stacks, int a, int b)
{
	vector<int> stack;

	// Shortcut intended to optimize performance
	bool not_found = true; 
  
	// Find the stack with the block A
	for (int stack_num = 0; not_found && (stack_num < num_stacks); stack_num++)
	{
		for (unsigned int block_num = 0; block_num < stacks[stack_num].size(); block_num++)
		{
			if ( stacks[stack_num][block_num] == a )
			{
				// Move all the blocks that are stacked above block A including the block A onto a temporary stack
				stack = vector<int>( stacks[stack_num].begin() + block_num, stacks[stack_num].end() );
				
				// Remove the blocks that are stacked above block A including the block A from the current stack
				stacks[stack_num].erase( stacks[stack_num].begin() + block_num, stacks[stack_num].end() );

				not_found = false;

				break;
			}
		}
	}

	not_found = true;
  
	// Find the stack with the block B
	for (int stack_num = 0; not_found && (stack_num < num_stacks); stack_num++)
	{
		for (unsigned int block_num = 0; block_num < stacks[stack_num].size(); block_num++)
		{
			if ( stacks[stack_num][block_num] == b )
			{
				// Move all the blocks from the temporary stack (block A and all the blocks on top of it) onto the stack that has block B in it
				stacks[stack_num].insert( stacks[stack_num].end(), stack.begin(), stack.end() );

				not_found = false;

				break;
			}
		}
	}
}

// Purpose: To return all blocks that are stacked on top of a certain block (block_num) to their initial positions.
// Algorithm: Iteratively examines blocks on top of the stack and removes them until the specified block in the argument has been reached.
// Parms: A pointer to the array of block stacks, the stack number from which to remove the blocks and the block number on which the blocks to be removed are stacked.
void moveToInitialPosition( vector<int>* stacks, int stack_num, int block_num )
{
	// Keep removing blocks that are stacked on top of the specified block until the specified block is encountered
	while ( stacks[stack_num].back() != block_num )
	{
		// Retrieve the block from the top of the stack
		int block_to_return = stacks[stack_num].back();

		// Return the retrieved block to its initial position
		// Note: the block number effectively identifies the stack number it maps to
		stacks[block_to_return].push_back( block_to_return );

		// Remove the virtually returned block from the top of the stack it was on
		stacks[stack_num].pop_back();
	}
}

// Purpose: To check if both blocks are currently in the same stack.
// Algorithm: Iteratively steps over the block stacks one stack at a time and examines if both blocks in question reside in any given stack; time complexity: O(n^2) 
// Parms: A pointer to the array of block stacks, the actual number of blocks that are stored in the entire 'block world', block A and block B.
bool areInSameStack(vector<int>* stacks, int num_stacks, int a, int b)
{
	bool has_a = false, has_b = false;
	
	// Step over the block stacks stack at a time
	for (int stack_num = 0; stack_num < num_stacks; stack_num++)
	{
		// Step over the blocks in the block stack one block at a time
		for (unsigned int block_num = 0; block_num < stacks[stack_num].size(); block_num++)
		{
			if ( stacks[stack_num][block_num] == a ) 
				has_a = true;
			
			if ( stacks[stack_num][block_num] == b ) 
				has_b = true;
		}
    
		// Shortcut intended to optimize performance
		if (has_a || has_b) 
			return has_a && has_b;
	}

	return has_a && has_b;
}

// Purpose: To print out the layout of the simulated block world.
// Algorithm:  Steps over the block stacks (implemented as array of vectors) and prints out the contents of each stack in the reverse order, 
// i.e. bottom-up as opposed to the stack's natural order top-down.
// Parms: A pointer to the array of block stacks and the actual number of blocks that are stored in the entire 'block world'.
void print( vector<int>* stacks, int num_stacks )
{
	cout << endl;

	// Step over the blocks one block container at a time
	// Time complexity: O(n^2)
	for (int stack_num = 0; stack_num < num_stacks; stack_num++)
	{
		// Print out sequential block number
		cout << stack_num << ":";
		
		// Step over the blocks in any given block stack and print them out (without physically removing blocks from the container)
		for (unsigned int block_num = 0; block_num < stacks[stack_num].size(); block_num++)
		{
			cout << " " << stacks[stack_num][block_num];
		}
    
		cout << endl;
	}
}
