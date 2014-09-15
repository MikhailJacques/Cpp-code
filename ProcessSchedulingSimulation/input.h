#ifndef INPUT_HPP
#define INPUT_HPP

// Input functions - Header file

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;

#include <deque>
using std::deque;

#include "process.h"

// Function: inputProcesses
// Purpose: To get process' parameters from the user
// Input: queue
// Output: queue of process sorted in the chronological order of their arrivals 
void inputProcesses( deque<Process> & );

// Function: *inputScheduler() 
// Purpose: to ask the user for the type of scheduling algorithm
// Return: pointer to a function that returns pointer to a Process object,
//         where inputScheduler() is the name of that mysterious function
// Input: pointer to a queue as its parameter
Process *( *inputScheduler( ) )( deque<Process> & );

#endif