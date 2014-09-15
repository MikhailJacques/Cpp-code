// Input functions - Implementation file

#include "input.h"
#include "scheduling.h"

// Function: inputProcesses
void inputProcesses( deque<Process> &queue )
{
   string processName;
   int processBurst, processPriority, processArrival;

   for ( ; ; )
   {
      cout << "Enter New Process Name (end to quit): ";
      cin >> processName;

	  if ( processName == "end" )
	  	 break;

      cout << "Enter Process Burst Time: ";
      cin >> processBurst;

      // See if the process does not take any clock cycles;
	  // if it does not, then ignore it
      if ( processBurst < 1 )
      {
		 cout << "*** Process Burst Time Must be Greater than 0! ***\n";
         continue;
      }

      cout << "Enter Process Priority: ";
      cin >> processPriority;

	  // See if the process priority is not negative;
	  // if it is, then ignore it
      if ( processPriority < 0 )
      {
		 cout << "*** Process Priority Can't be Negative! ***\n";
         continue;
      }

      cout << "Enter Process Arrival Time: ";
      cin >> processArrival;

	  // See if the process' arrival time is not negative;
	  // if it is, then ignore it
      if ( processArrival < 0 )
      {
		 cout << "*** Process Arrival Time Can't be Negative! ***\n";
         continue;
      }

      // Initialize the process with the valid user defined parameters
      Process tmp( processName, processArrival, processBurst, processPriority );

      // Put the process into the event priority queue
      // Note: In the event priority queue the processes will be sorted 
	  //       by their arrival times (i.e. by processArrival parameter)
      queue.push_back( tmp );
   }
}

// Function: *inputScheduler() 
Process *( *inputScheduler( ) )( deque<Process> & )
{
   char choice;

   cout << "[F]irst Come, First Serve\n"
        << "[S]hortest Job First\n"
        << "[R]ound Robin\n"
        << "[P]reemptive Priority\n"
        << "Your choice: ";

   cin >> choice;

   choice = toupper( choice );

   switch ( choice )
   {
      case 'F': return fcfs;  // Return a pointer to the fcfs function

      case 'S': cout << "Preemptive [y|n]? ";
                cin >> choice;

                choice = toupper( choice );

                if ( choice == 'Y' )
                {
                   return psjf;  // Return a pointer to the psjf function
                }
                else
                {
                   return sjf;  // Return a pointer to the sjf function
                }

      case 'R': return rr;  // Return a pointer to the rr function

      case 'P': return pp;  // Return a pointer to the pp function
   }
}
