// Scheduling algorithms - Implementation file

#include "scheduling.h"
#include <algorithm>
using std::sort;

// The following three classes act like functions
// They are required for using the STL sorting routines to make a priority queue
// because STL sorting routines expect an object as their third parameter.
// Note: If there was sorting required on only one parameter, then the
// overloading of an operator< in Process class would do the job.
// However, since there are three different sorting algorithms, we need to use
// function objects which are passed to the sorting routines

// This class is used to sort processes based on the number of execution cycles
// that it takes to execute a process to completion
class BurstSort
{
   public:

     // Overloaded operator()
     // Purpose: Used by STL sorting routines
     bool operator( )( const Process & x, const Process & y )
     {
        return x.getBurst( ) < y.getBurst( );
     }

} burstSort;

// This class is used to sort processes based on their specified priorities
// Note: User defines a priority for each process
class PrioritySort
{
   public:

      // Overloaded operator()
      // Purpose: Used by STL sorting routines
      bool operator( )( const Process & x, const Process & y )
      {
         return x.getPriority( ) < y.getPriority( );
      }

} prioritySort;

// Function: fcfs (First Come First Serve)
Process *fcfs( deque<Process> &queue )
{
   // Declare static process
   static Process process;

   // See if the queue is not empty
   if ( !queue.empty( ) )
   {
      // Get the first (scheduled to run) process
	  process = *queue.begin( );

      queue.pop_front( );
   }
   else
	  return NULL;

   // Return pointer to the process
   return &process;
}

// Function: sjs (Shortest Job First)
Process *sjf( deque<Process> &queue )
{
   // Sort the processes based on their burst cycles
   // Place the sortest process at the beginning of the queue
   // Note: sort is an STL function, and burstSort is a burst comparison 
   //       user-defined function-object
   sort( queue.begin( ), queue.end( ), burstSort );

   return fcfs( queue );
}

// Function: rr (Round Robin)
Process *rr( deque<Process> &queue )
{
   return fcfs( queue );
}

// Function: psjf (Preemptive Shortest Job First)
Process *psjf( deque<Process> &queue )
{
   // Sort the processes based on their burst times
   // Note: Checking for pre-emption is done in main
   return sjf( queue );
}

// Function: pp (Preemptive Priority)
Process *pp( deque<Process> &queue )
{
   // Sort the processes based on their priorities (ranks)
   // Note: sort is an STL function, and psort is a priority comparison 
   // user-defined function-object
   sort( queue.begin( ), queue.end( ), prioritySort );

  return fcfs( queue );
}