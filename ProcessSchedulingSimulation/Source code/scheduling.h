#ifndef SCHEDULING_HPP
#define SCHEDULING_HPP

// Scheduling algorithms - Header file

#include <deque>
using std::deque;

// Include sorting routines
#include <algorithm> 

#include "process.h"

// Function: fcfs (First Come First Serve)
// Purpose: to allocate process on the FCFS basis 
// Input: queue of processes
// Output: pointer to the first process in a queue of processes
Process *fcfs( deque<Process> & );

// Function: sjf (Shortest Job First)
// Purpose: to allocate process on the SJF basis
// Input: queue of processes
// Output: pointer to the shortest process in a queue of processes
Process *sjf( deque<Process> & );

// Function: Round Robin
// Purpose: to allocate process on the RR basis
// Input: queue of processes
// Output: pointer to the next process in a queue of processes 
//         on a Round-Robin basis
Process *rr( deque<Process> & );

// Function: psjf (Preemptive Shortest Job First)
// Purpose: to allocate process on the PSJF basis
// Input: queue of processes
// Output: pointer to the shortest process in a queue of processes,
//         including the newly arrived ones, sorted based on their 
//         execution times. Note: when a new process arrives, 
//         the main throws all the running processes into the waiting 
//         queue, reallocates them and them chooses a new one from the queue
Process *psjf( deque<Process> & );

// Function: pp (Preemptive Priority)
// Purpose: to allocate process on the PP basis
// Input: queue of processes
// Output: pointer to the first process in a queue of processes
//         sorted based on their priorities
Process *pp( deque<Process> & );

#endif