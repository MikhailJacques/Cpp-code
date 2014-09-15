
// Cycle Detection (for single resource instance)
// Author: Mikhail Jacques (0128039) at Trent University
// Date:   Fall 2002
// The program simulates the allocation of resources to processes and 
// requests of resources by processes (for single resource instance) and
// determines whether there is a cycle

// First, the program reads in the number of processes and resources,
// then the program reads in the current allocations of resorces to processes
// and requests for resources by processes.
// Second, the program creates Resourse-Allocation and Resource-Request Graphs.
// Third, the program builds a Wait-For Graph.
// Once, the Wait-For Graph has been constructed the program runs the
// cycle detection algorithm to see whether there are any deadlocked processes.


#include <iostream>
using std::cout;
using std::cin;

#include <fstream>
using std::ofstream;
using std::ostream;
using std::ios;

#include <ctime>

#include "graphal.h"

#define NUM_PROCESSES 15
#define NUM_RESOURCES 15


int main( )
{

   // Declare Graph objects that will be used to keep track of the resource
   // allocations, resource requests, and wait-for processes
   Graph resourceAllocation( NUM_RESOURCES ), resourceRequest( NUM_PROCESSES ),
         waitFor( NUM_PROCESSES );

   int numProcesses, numResources, 
	   numAllocations, numRequests, 
	   resource, process, i;


   // Prompt for input
   cout << "Enter number of processes: ";
   cin >> numProcesses;

   cout << "Enter number of resources: ";
   cin >> numResources;

   do 
   {
      cout << "Enter number of allocations: ";
      cin >> numAllocations;

	  if ( numAllocations > numResources  )
	  {
		 cout << "Error - Number of Allocations CANNOT exceed number of resources\n";
	  }

	  if ( numAllocations < 0 )
	  {
		 cout << "Error - Number of Allocations CANNOT be negative\n";
	  }

   } while ( numAllocations > numResources || numAllocations < 0 );

   do 
   {
      cout << "Enter number of requests: ";
      cin >> numRequests;

	  if ( numRequests < 0 )
	  {
		 cout << "Error - Number of Requests CANNOT be negative\n";
	  }

   } while ( numRequests < 0 );


   int graphSize;

   if ( numResources < numProcesses )
      graphSize = numProcesses;
   else
	  graphSize = numResources;

   // Insert vertices in the graphs
   for ( i = 0; i < graphSize; i++ )
   {
      // Insert resource name (number) as a vertex in the Graph object
	  resourceAllocation.InsertVertex ( i );

      // Insert process name (number) as a vertex in the Graph object
	  resourceRequest.InsertVertex ( i );

      // Insert process name (number) as a vertex in the Graph object
	  waitFor.InsertVertex ( i );
   }

   // Declare dynamic array of integers
   int *resources = new int [numResources];

   // Mark the resources as being unallocated
   for ( i = 0; i < numResources; ++i )
      resources[i] = 0;

   // Allocate resources to processes
   for ( i = 0; i < numAllocations; )
   {
      cout << "Enter allocation (resource to process): ";
      cin >> resource >> process;

	  // Check for appropriate input range
      if ( resource > numResources )
	  {
		 cout << "Error - Invalid Resource number\n";
		 continue;
	  }

	  // Check for appropriate input range
	  if ( process > numProcesses )
	  {
		 cout << "Error - Invalid Process number\n";
		 continue;
	  }

	  // See if the resource is free
	  if ( !resources[resource - 1] )
	  {
	     // Mark resource as being allocated
	     resources[resource - 1] = process;

		 ++i; // Count allocations

         // Insert allocation edge
         resourceAllocation.InsertEdge( resource - 1, process - 1 );
	  }
	  else
	  {
	     cout << "Error - Resource[" << resource << "] has already been allocated "
			  << "to Process[" << resources[resource - 1] << "]\n";
	  }
   }

   // Request resources by processes
   for ( i = 0; i < numRequests; )
   {
      cout << "Enter request (process to resource): ";
      cin >> process >> resource;

	  // Check for appropriate input range
      if ( resource > numResources )
	  {
		 cout << "Error - Invalid Resource number\n";
		 continue;
	  }

	  // Check for appropriate input range
	  if ( process > numProcesses )
	  {
		 cout << "Error - Invalid Process number\n";
		 continue;
	  }

	  // See if the process requesting the resource has not been allocated to it already
	  if ( process != resources[resource - 1] )
	  {
         // Insert request edge
         resourceRequest.InsertEdge( process - 1, resource - 1 );

		 ++i; // Count requests
	  }
	  else
	  {
	     cout << "Error - Process[" << process << "] has already been allocated "
			  << "Resource[" << resource << "]\n";
	  }
   }

   // Create event log file
   ofstream outLogFile( "logfile.txt", ios::out );

   // For outputing logfile creation date
   struct tm *time_now;
   time_t secs_now;
   char date[80];
   tzset( );
   time( &secs_now );
   time_now = localtime( &secs_now );
   strftime( date, 80, "%Y-%m-%d %H:%M:%S", time_now );

   // Record date and time of log file creation
   outLogFile << date << "\n\n";


   // Print out current resource allocations
   resourceAllocation.PrintAllocatedResources( outLogFile );

   // Print out current resource requests
   resourceRequest.PrintRequestedResources( outLogFile );

   // Construct a wait-for graph
   waitFor.ConstructGraph( resourceAllocation, resourceRequest );

   // Print out current waiting for resources processes
   waitFor.PrintWaitForProcesses( outLogFile );

  
   // Run the Depth-First-Search algorithm to 
   // determine whether there are any cycles
   if ( waitFor.DFS( ) )
   {
	  outLogFile << "Alarm! Alarm! - Cycle exists" << endl;
   }
   else
	  outLogFile << "No cycle has been detected. Go to Hell" << endl;

   // Close event log file
   outLogFile.close( );

   cout << "Log file \"logfile.txt\" has been created for your convenience" << endl;

   return 0;
}