// Class Process - Implementation file

#include "process.h"

// Constructor
Process::Process( string n, int a, int b, int p ):
  _name( n ), _arrival( a ), _burst( b ), _priority( p )
{
   // Initialize parms
   _wait = 0;
   _quantum = 0;
   _turnaround = _burst;
}

// Get process' name
string Process::getName( ) const
{
   return _name;
}

// Get process' arrival time
int Process::getArrival( ) const
{
   return _arrival;
}

// Get process' execution cycles
int Process::getBurst( ) const
{
   return _burst;
}

// Get process' priority
int Process::getPriority( ) const
{
   return _priority;
}

// Get the number of cycles passed since the last wait() call
int Process::getQuantum( ) const
{
   return _quantum;
}

// Get the total number of cycles spent waiting
int Process::getWaiting( ) const
{
   return _wait;
}

// Get process' turnaround time
int Process::getTurnaround( ) const
{
   return _turnaround;
}

// Increment process' running time by one cycle and check whether 
// the process is still running or has finished
bool Process::Step( ofstream & outLogFile )
{
   // Process is running
   // Increment the number of the cycles executed continuously
   _quantum++;

   // See if execution time left
   if ( --_burst )
   {
      // Anounce process execution
      outLogFile << _name << " steps (" << _burst << ")\n";

      // Execution of the process is not done yet
      return false;
   }
   else
   {
      // Anounce end of process execution
	  outLogFile << _name << " steps (" << _burst << ") and is finished\n";
      // outLogFile << _name << " finished\n";

      // Compute turnaround time
      _turnaround += _wait;

      // Execution of the process is done
      return true;
  }
}

// Increment process' waiting time by one cycle
void Process::Wait( ofstream & outLogFile )
{
   // Anounce process waiting
   outLogFile << _name << " waits (" << _burst << ")\n";

   // Increment wait counter which represents the waiting 
   // time of the process
   _wait++;

   // Reset quantum
   // Note: Resets the number of the cycles executed continuously
   _quantum = 0;
}