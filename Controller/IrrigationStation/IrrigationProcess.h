
// Irrigation Process interface file

#ifndef IRRIGATION_PROCESS_H
#define IRRIGATION_PROCESS_H

#include "DBBase.h"
#include "IrrigationIncludes.h"

// extern "C" is meant to be recognized by a C++ compiler and to notify the compiler that the noted function is (or to be) compiled in C style.
#ifdef __cplusplus
extern "C" {
#endif

// Purpose: To initialize, start, pause, resume, preempt, stop and finish an Irrigation and Dosing Programs
//			ProcessExecution_Manager.c invokes these functions
IrrigationSequenceState Irrigation_Sequence_Handler( DBTask task_id,  IrrigationSequenceState state );
// DosingProgramState Dosing_Program_Handler( DBTask task_id, DosingProgramState state );

#ifdef __cplusplus
};                        
#endif 

#endif