
#ifndef GRAPHAL_HPP
#define GRAPHAL_HPP

// Directed Graph: Header File ( Dynamic Adjacency List )
// Assumptions: 1) Vertex names are unique
//              2) No parallel edges

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <fstream>
using std::ofstream;

class Graph
{
	public:
	  // Constructor
	  //
	  // Input  : Integer N
	  // Purpose: To create an empty Graph with a maximum of N vertices
	  // Output : None
	  Graph( int N );

	  // Copy constructor
	  //
	  // Input  : Graph G
	  // Purpose: To initialize Graph to G
	  // Output : None
	  Graph( const Graph & G );

	  // Destructor
	  //
	  // Input  : None
	  // Purpose: To free memory of Graph
	  // Output : None
	  ~Graph( );

	  // Copy assignment
	  //
	  // Input  : Graph G
	  // Purpose: To assign G to current Graph
	  // Output : Current Graph
	  const Graph & operator=( const Graph & G );

	  // InsertVertex
	  //
	  // Input  : Vertex V
	  // Purpose: To insert V into Graph
	  // Assume : No duplicate vertices are allowed
	  // Output : true if successful; false otherwise
	  bool InsertVertex( const int & V );

	  // InsertEdge     (from V1 to V2)
	  //
	  // Input  : Edge E = (V1,V2)
	  // Purpose: To insert E into Graph
	  // Output : true if successful; false otherwise
	  bool InsertEdge( const int & V1, const int & V2 );

      // getNumVertices
      // Input: None
      // Purpose: To return the current number of vertices in the graph
      // Output: current number of vertices in the graph
      int getNumVertices( ) const;

	  // Function: PrintAllocatedResources
	  // Input: File output object
      // Purpose: To print out allocated resources to processes 
	  // Output: None
	  void PrintAllocatedResources( ofstream & outLogFile ) const;

	  // Function: PrintRequestedResources
	  // Input: File output object
      // Purpose: To print out requested resources by processes 
	  // Output: None
	  void PrintRequestedResources( ofstream & outLogFile ) const;

	  // Function: PrintWaitForProcesses
	  // Input: File output object
      // Purpose: To print out which processes wait for which processes
	  // Output: None
	  void PrintWaitForProcesses( ofstream & outLogFile ) const;

     // Function: ConstructGraph
	 // Assumption: The input graphs are valid
     // Input: Two Graph objects
     // Purpose: To construct a Wait-For Graph
     // Output: None
     void ConstructGraph( const Graph & resourceAllocation,
                          const Graph & resourceRequest );

     // Function: DFS
     // Purpose: To determine if there is a cycle in the graph
     // Input: None
     // Output: true if there is a cycle; false otherwise
     bool DFS( ) const;

   private:
	  // Position
	  //
	  // Input  : Vertex V
	  // Purpose: To map V into an index of VList
	  // Output : The index if successful; -1 otherwise
	  int Position( const int & V ) const;

	  // Clear
	  //
	  // Input  : Integer i
	  // Purpose: To free the adjacency list of vertex V[i]
	  // Output : None
	  void Clear( int i );

	  // Purpose: To determine if there is a cycle in the graph
      // Input: vertex name, array of bool values, and flag
      // Output: None
      void DFS( const int & V, bool visited[], bool & flag ) const;

	  // Data Members

	  // Dynamic array to store the vertex names of Graph
	  int *Vlist;

	  // A singly-linked list of endpoints
	  struct EdgeNode
	  {
		  int Vertex;

		  EdgeNode *Next;

		  // EdgeNode constructors
		  EdgeNode ( ) : Next (NULL) { }

		  EdgeNode ( const int & V, EdgeNode *P = NULL ) :
						 Vertex ( V ), Next ( P ) { }
	  };

	  // Dynamic array where each position i stores the pointer
	  // to the first adjacent vertex of V[i]
	  EdgeNode **Edge;

	  // Current and maximum number of vertices in Graph
	  int NumVertices, MaxVertices;
};

#endif