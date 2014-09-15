
// Directed Graph : Implementation File ( Dynamic Adjacency List )
// Notes : 1) Delete and new functions are assumed to take constant time
//         2) v represents the number of vertices
//         3) e represents the number of edges


#include "graphal.h"


// Constructor
//
// Time complexity: O(1)
Graph::Graph( int N ) : NumVertices ( 0 ), MaxVertices ( N )
{
   // Dynamically allocate an array of integers of size N
   Vlist = new int [N];

   // Dynamically allocate an array of pointers to EdgeNode objects
   Edge = new EdgeNode* [N];

   // Initialize pointers to point to NULL (no edges)
   for ( int i = 0; i < MaxVertices; i++ )
   {
      Edge[i] = NULL;
   }
}


// Copy constructor
//
// Time complexity: O(v+e)
Graph::Graph( const Graph & Rhs )
{
   *this = Rhs;
}


// Destructor
//
// Time complexity: O(e)
Graph::~Graph( )
{
	delete [] Vlist;

	// Loop through the Edge pointers (i.e. pointers to EdgeNodes)
	for ( int i = 0; i < NumVertices; i++ )
		Clear ( i );

	delete [] Edge;
}


// Copy assignment
//
// Time complexity: O(v+e)
const Graph &
Graph::operator=( const Graph & Rhs )
{
   EdgeNode *P;
   int i;

   if ( this != &Rhs )   // Test for aliasing
   {
      delete [] Vlist;

      // Loop through the Edge pointers
	  for ( i = 0; i < NumVertices; i++ )
	     Clear ( i );

	  delete [] Edge;

      // Update private data members
	  NumVertices = Rhs.NumVertices;
	  MaxVertices = Rhs.MaxVertices;

      // Dynamically allocate memory for Vtype objects
	  Vlist = new int [MaxVertices];

      // Dynamically allocate memory for pointers to EdgeNode structures
	  Edge = new EdgeNode *[MaxVertices];

      // Initialize pointers to point to NULL (no edges)
	  for ( i = 0; i < MaxVertices; i++ )
	     Edge[i] = NULL;

	  for ( i = 0; i < NumVertices; i++ )
	  {
	     Vlist[i] = Rhs.Vlist[i];

		 P = Rhs.Edge[i];

		 // Edge insertions kicks in here
		 while ( P != NULL )
		 {
		    // Dynamically allocate new EdgeNode
			// First, make it store the vertex and make it point to where the Edge
			// pointer points to. Then, make the Edge pointer point at this EdgeNode
			// Note: This way, the edges will be copied (inserted) in the reversed order
			Edge[i] = new EdgeNode ( P->Vertex, Edge[i] );

			P = P->Next;
		 }
	  }
   }
   return *this;
}


// InsertVertex
//
// Time complexity: O(v)
bool
Graph::InsertVertex( const int & V )
{
	// See if there is room for the vertex V and if the vertex V is not in the Vlist yet
	// Note: if statement uses false short-circuit strategy
	if ( NumVertices < MaxVertices && Position( V ) == -1 )
	{
		Vlist[ NumVertices++ ] = V;

		return true;
	}
	else
		return false;
}


// InsertEdge (from V1 to V2)
//
// Time complexity: O(v)
bool
Graph::InsertEdge( const int & V1, const int & V2 )
{
	int i;

	// See if the vertices exist
	if ( (i = Position( V1 )) != -1 && Position( V2 ) != -1 )
	{
		// Insert V2 at the beginning of the adjacency list of V1
		// Note: No checking for parallel edges or loops
		Edge[i] = new EdgeNode ( V2, Edge[i] );

		return true;    // Return success
	}
	else
		return false;   // At least one of the vertices does not exist
}



// Position
//
// Time complexity: O(v)
int
Graph::Position( const int & V ) const
{
   for ( int i = 0; i < NumVertices; ++i )
   {
      if ( Vlist[i] == V )
	     return i;         // Return the index of the found vertex
   }

   return -1;              // Vertex wasn't found
}


// Clear
//
// Time complexity: O(v)
void
Graph::Clear( int i )
{
	EdgeNode* P;

	while ( Edge[i] != NULL )
	{
      // Make P point to the node to be removed
		P = Edge[i];

      // Bypass the node to be removed
		Edge[i] = Edge[i]->Next;

      // Remove the node
		delete P;
	}
}


// getNumVertices
int
Graph::getNumVertices( ) const
{
   return NumVertices;
}


// ConstructGraph
//
// Time complexity: O(n^2)
void
Graph::ConstructGraph( const Graph & resourceAllocation, const Graph & resourceRequest )
{
   MaxVertices = resourceRequest.MaxVertices;
   EdgeNode *requestPtr, *allocationPtr;
   int process, resource, i;


   // Traverse all adjacency lists
   for ( i = 0; i < resourceRequest.NumVertices; i++ )
   {
      // Save pointer to the first node in the ith list of resource request graph
      requestPtr = resourceRequest.Edge[i];

      // Traverse ith adjacency list and look for requested processes, if any
	  while ( requestPtr != NULL )
	  {
         // Save resource name
         resource = requestPtr->Vertex;

         // Save pointer to the first node in the "mysterious" list of resource
         // allocation graph
         allocationPtr = resourceAllocation.Edge[resourceAllocation.Position( resource )];

		 // See if the resource is allocated
         if ( allocationPtr != NULL )
         {
            // Save process name
            process = allocationPtr->Vertex;

            // Insert edge
            InsertEdge( resourceRequest.Vlist[i], process );
         
		 }  // end of if statement

         // Advance to the next node, if any
		requestPtr = requestPtr->Next;
      
	  } // end of while loop
   
   } // end of for loop
}


// PrintAllocatedResources
//
// Time complexity: O( v^2 )
void
Graph::PrintAllocatedResources( ofstream & outLogFile ) const
{
   for ( int i = 0; i < NumVertices; ++i )
   {
      if ( Edge[i] != NULL )
	  {
		 outLogFile << "Resource[" << Vlist[i] + 1 << "] is allocated to "
			        << "Process[" << Edge[i]->Vertex + 1 << "]\n";
	  }
      else
         outLogFile << "Resource[" << Vlist[i]  + 1 << "] is free and bored\n";
   }

   outLogFile << endl;
}



// PrintRequestedResources
//
// Time complexity: O( v^2 )
void
Graph::PrintRequestedResources( ofstream & outLogFile ) const
{
   EdgeNode *P;

   for ( int i = 0; i < NumVertices; ++i )
   {
      // Save pointer to the first node in the ith list
      P = Edge[i];

	  if ( P == NULL )
	     outLogFile << "Process[" << Vlist[i] + 1 << "] does not request Resources ";
	  else
	  {
		 outLogFile << "Process[" << Vlist[i] + 1 << "] requests ";

         // Traverse ith adjacency list and look for resorces
	     while ( P != NULL )
		 {
		    outLogFile << "Resource[" << P->Vertex + 1 << "] ";
		 
            // Advance to the next node, if any
		    P = P->Next;
		 }
	  }

	  outLogFile << endl;
   }

   outLogFile << endl;
}


// PrintWaitForProcesses
//
// Time complexity: O( v^2 )
void
Graph::PrintWaitForProcesses( ofstream & outLogFile ) const
{
   EdgeNode *P;

   for ( int i = 0; i < NumVertices; ++i )
   {
      // Save pointer to the first node in the ith list
      P = Edge[i];

	  if ( P == NULL )
	  {
	     outLogFile << "Process[" << Vlist[i] + 1 << "] is not waiting "
			        << "for any Processes and is Happy ";
	  }
	  else
	  {
		 outLogFile << "Process[" << Vlist[i] + 1 << "] is waiting for ";

         // Traverse ith adjacency list and look for resorces
	     while ( P != NULL )
		 {
		    outLogFile << "Process[" << P->Vertex + 1 << "] ";
		 
            // Advance to the next node, if any
		    P = P->Next;
		 }
	  }

	  outLogFile << endl;
   }

   outLogFile << endl;
}



// Function: DFS
//
// Time complexity: O(n^2)
bool
Graph::DFS( ) const
{
   bool flag = false;
   bool *visited = new bool [NumVertices];
   int i, j;

   // Run cycle detection for each vertex
   for ( i = 0; i < NumVertices; i++ )
   {
      // Reset the array
      for ( j = 0; j < NumVertices; j++ )
         visited[j] = false;
	  
	  DFS( Vlist[i], visited, flag );

	  // See if the cycle has been detected
      if ( flag ) 
	  {
	     return true;   // Cycle has been detected
	  }
   }
   
   // No cycles
   return false;
}



// Function: DFS
//
// Time complexity: O(n^2)
void
Graph::DFS( const int & V, bool visited[], bool & flag ) const
{
   // Mark process as being visited 
   visited[Position( V )] = true;

   // Save pointer to the first node in the ith list
   EdgeNode *P = Edge[Position( V )];

   // Traverse ith adjacency list and look for cycles
   while ( P != NULL && !flag )
   {
	  if ( visited[Position( P->Vertex )] )
	  {
         flag = true;   // cycle has been detected
	  }
      else
         DFS( P->Vertex, visited, flag );

	  // Advance pointer
	  P = P->Next;
   }
   
   // Reset only if no cycle has been detected
   if ( !flag )
   {
      visited[Position( V )] = false;
   }
}


