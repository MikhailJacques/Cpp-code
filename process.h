#ifndef PROCESS_HPP
#define PROCESS_HPP

// Class Process - Header file

#include <string>
using std::string;

#include <iostream>
using std::ostream;
using std::cout;
using std::cin;
using std::endl;

#include <fstream>
using std::ofstream;

class Process
{
   public:

      // Default constructor
      Process( ) {}

      // Constructor
      Process( string, int, int, int );

      Process( const Process & Rhs )
      {
         *this = Rhs;
      }

	  // Overloaded assignment operator
      const Process & operator=( const Process & Rhs )
      {
         if ( this != &Rhs )
         {
            _name = Rhs._name;
            _burst = Rhs._burst;
            _priority = Rhs._priority;
            _arrival = Rhs._arrival;
            _wait = Rhs._wait;
            _quantum = Rhs._quantum;
            _turnaround = Rhs._turnaround;
         }

         return *this;
      }

      // Accessor functions

      // Purpose: To get the name of the process
      string getName( ) const;

      // Purpose: To get the arrival time of the process
      int getArrival( ) const;

      // Purpose: To get the number of clock cycles required for the
      //          process to execute
      int getBurst( ) const;

      // Purpose: To get the priority of the process
      int getPriority( ) const;

      // Purpose: To get the number of cycles passed since the last wait() call
      // Note: Only relevant for Round-Robin Scheduling
      int getQuantum( ) const;

      // Purpose: To get the total number of cycles spent waiting
      int getWaiting( ) const;

      // Purpose: To get process' turnaround time (completionTime - arrivalTime)
      int getTurnaround( ) const;

      // Once a process has been submitted, every CPU cycle either step() or wait()
      // call will be executed for the process

      // Purpose: To increment process' running time by one cycle and to see
      // whether the process is still running or has finished
      // Note: Step() gets activated after the processes has been submitted and
      // is executing (i.e. each active cycle)
      // Output: true if the process has finished; false otherwise
      bool Step( ofstream & );

      // Purpose: To increment process' waiting time by one cycle
      // Note: Wait() gets activated after the processes has been submitted but
      // is not executing (i.e. each wait cycle)
      // Output: None
      void Wait( ofstream & );

      // string _name;    // Name of the process

   private:

      string _name;    // Name of the process
      int _burst;      // Number of clock cycles required to execute the process
      int _priority;   // Priority of the process (user defined rank)
      int _arrival;    // Arrival time of the process
      int _wait;       // Total number of cycles spent waiting
      int _quantum;    // Number of cycles since the last wait() call - timeslice
                       // That is, the quantum stores is how long the process
                       // has been executing continuously
      int _turnaround; // Time between arrival and completion (running + waiting)
};

#endif




