// Operating System Process Scheduling Simulator
// The program simulates the execution of processes in a single processor 
// environment under a number of various scheduling algorithms
// Author: Mikhail Jacques (0128039)
// Date: June 2002

#include <ctime>

#include <cctype>
#include <algorithm>

#include <deque>
using std::deque;

#include <fstream>
using std::ofstream;

#include <iostream>
using std::ios;

#include "input.h"
#include "process.h"
#include "scheduling.h"
// using namespace std;

// This class is used to sort processes based on their arrival times
class ArrivalSort
{
   public:

      // Overloaded operator()
      // Purpose: Used by STL sorting routines
      bool operator( )( const Process & x, const Process & y )
      {
         return x.getArrival( ) < y.getArrival( );
      }

} arrivalSort;

int main( )
{
   // Declare three queues which will store processes:
   // readyQueue stores processes that are ready ready for execution.
   // The processes have arrived and may have run for a while but they are
   // not currently executing.
   // arrivalQueue stores processes that have arrived and will arrive over time
   // doneQueue stores processes that have finished their executions
   deque<Process> arrivalQueue, readyQueue, doneQueue;

   // Declare pointer to currently running Process
   Process *running = NULL, process;

   // Scheduler is a dynamically changable function that takes a pointer
   // to a queue as its only parameter and returns a pointer to a Process
   // (*Scheduler) is a pointer to a function because the actual function
   // used will be determined at runtime based on user's input
   Process *( *Scheduler )( deque<Process> & );

   // Declare three iterators that will be used to iterate through the queues
   // Note: Iterator acts like a pointer to a single element in the queue
   std::deque<Process>::iterator arrival_iter, ready_iter, done_iter;

   // Declare local variables
   int total = 0, cycle, quantum;

   // Prompt the user for the choice of an allocation method and assign that
   // method to the scheduler function
   Scheduler = inputScheduler( );

   // For outputing logfile creation date
   struct tm *time_now;
   time_t secs_now;
   char date[80];

   tzset( );
   time( &secs_now );
   time_now = localtime( &secs_now );

   strftime( date, 80, "%Y-%m-%d %H:%M:%S", time_now );

   // Open event log file
   ofstream outLogFile( "logfile.txt", ios::out );

   // Record date and time
   outLogFile << date << "\n\n";

   // If it is a Round-Robin allocation method, then an extra piece of information
   // is required - the timeslice size between reallocation
   if ( Scheduler == rr )
   {
      do
	  {
		 // Prompt for quantum
         cout << "Enter quantum: ";
         cin >> quantum;

		 if ( quantum <= 0 )
			cout << "Error. Please try again!";
	  } while ( quantum <= 0 );
   }

   outLogFile << "\n";

   // Get the processes from the user and put them into the arrival queue
   inputProcesses( arrivalQueue );

   // Sort the processes based on their arrival times in the ascending order
   // STL function sort() takes a range of elements in a structure and an object
   // that has an overloaded operator()
   // The object's overloaded operator() takes two elements and returns a boolean
   // The sorting routine somewhere in its guts will use the overloaded operator()
   std::sort( arrivalQueue.begin( ), arrivalQueue.end( ), arrivalSort );

   outLogFile << "\n\n";


   // THIS IS THE MAIN EXECUTION LOOP

   // Loop as long as there are queued processes, either newly arrived or preempted,
   // or a running process
   // Note: Set arrival_iter to point to the first element in the arrival queue
   for ( arrival_iter = arrivalQueue.begin( ), cycle = 0;
       ( arrival_iter != arrivalQueue.end( ) ) || ( readyQueue.size( ) > 0 ) || running;
         cycle++ )
   {
      // Every clock cycle output the status for each process that has arrived
      outLogFile << "On the occasion of clock cycle " << cycle << "...\n";

      // For each element in the arrival queue do the following:
      // (arrival_iter->getArrival() <= cycle) - if the event occured at this
      // time or before, then process it
      // arrival_iter->getArrival()- gets the arival time of the processes
      // Note: Past events are not deleted, because arrivalQueue is sorted so
      // this loop should only be stepping through the events that occur in this
      // cycle and then arrival_iter remains pointing at the first event in the 
	  // next cycle. Rather than deleting old events, the arrivalQueue is left 
	  // unmodified and the state of the iterator is saved at each cycle
      // (this is a functional way of doing things; see LISP closures)
      for ( ; ( arrival_iter != arrivalQueue.end( ) ) &&
              ( arrival_iter->getArrival( ) <= cycle ); arrival_iter++ )
      {
         // See if the process arrives in this cycle
         if ( arrival_iter->getArrival( ) == cycle )
         {
            // Copy process
			process = *arrival_iter;
			
			// Move the process to the readyQueue
            // Note: The state of the process in the arrivalQueue never gets modified
	        readyQueue.push_back( process );
         }
      }

	  // See if there are no processes that are currently running
      // Note: running is a pointer to the currently running process,
      // so if there is a running process, then running != NULL
      if ( running == NULL && readyQueue.size() != 0)
      {
         // Schedule a new process (because there are no running processes)
		 running = Scheduler( readyQueue );
      }

      // See if it is Round-Robin, and then check whether the timeslice is up
      else if ( ( running != NULL ) && 
		        ( Scheduler == rr ) && 
		        ( ( running->getQuantum( ) % quantum ) == 0 ) )
      {
         // Copy process
		 process = *running;
		 
		 // Preempt the process
         readyQueue.push_back( process );

		 // Schedule a new process
         running = Scheduler( readyQueue );
      }
      // See if it is priority scheduling
      // If both conditions are met, then reallocate
      else if ( ( running != NULL ) && 
		        ( Scheduler == psjf || Scheduler == pp ) )
      {
         // Copy process
		 process = *running;
		 
		 // Preempt the process
         readyQueue.push_back( process );

		 // Schedule a new process
         running = Scheduler( readyQueue );
      }

      // See if the process is running
      if ( running != NULL )
      {
         // Run the currently selected process for a clock cycle and
         // see if the process has finished executing
         if ( running->Step( outLogFile ) )
         {
            // Copy process
			process = *running;
			
			// Put the finished process into the doneQueue
	        doneQueue.push_back( process );

            // Reset
	        running = NULL;
         }
      }

      // The rest of the processes (if any) are waiting during this cycle
      // Therefore, run the Wait() for the rest of the processes in the  
	  // readyQueue to update their waiting times
      for ( ready_iter = readyQueue.begin( ); ready_iter != readyQueue.end( ); ready_iter++ )
      {
         // if ( (*ready_iter).getStatus( ) )
         ready_iter->Wait( outLogFile );
      }
   }


   // FINISHED RUNNING ALL THE PROCESSES

	outLogFile << "\n";

   // Declare stats for all finished processes
   for ( done_iter = doneQueue.begin( ); done_iter != doneQueue.end( ); done_iter++ )
   {
      // Output processes' names and their waiting times during their execution lifetimes
      outLogFile << (*done_iter).getName( ) << " has spent " 
		         << (*done_iter).getWaiting( )
                 << " cycles waiting for a turnaround time of "
                 << (*done_iter).getTurnaround( ) << " cycles\n";

      // Update total waiting time
      total += (*done_iter).getWaiting( );
   }

   // If there were no processes at all, then there is no average waiting time
   if ( arrivalQueue.size( ) > 0 )
   {
      outLogFile << "\nProcesses had an average waiting time of "
                 << ( ( float ) total / ( float ) arrivalQueue.size( ) )
                 << " cycles\n";
   }
   else
   {
      outLogFile << "No processes were scheduled\n";
   }
      
   // Close event log file
   outLogFile.close( );

   return 0;
}


