
// Irrigation Program class interface file

#ifndef IRRIGATION_PROGRAM_H
#define IRRIGATION_PROGRAM_H

#include "DB.h"
#include "DBBase.h"
#include "DBDateTime.h"
#include "ValveGroup.h"
#include "ValveSequence.h"
#include "NMC_MML_Platform.h"
#include "DBIrrigationPrograms.h"

extern "C" {
	#include "tasks.h"
	#include "utility.h"
	#include "cxfuncs.h"
	#include "DBBitField.h"
};                        

#include "string.h"
#include "EASTL\algorithm.h"
#include "EASTL\fixed_map.h"
#include "EASTL\fixed_vector.h"

using namespace eastl;

class IrrigationProgram : private DBIrrigationProgram
{
	public:

		// Default constructor
		explicit IrrigationProgram( );
		explicit IrrigationProgram( DBTask task_id );
		// explicit IrrigationProgram( const IrrigationProgram & program );
		~IrrigationProgram( );
		// const IrrigationProgram & operator=( const IrrigationProgram & program );

		/////////////////////////////////////////////////////////////
		// Accessor functions
		/////////////////////////////////////////////////////////////
		DBIndex getProgramId( ) const;
		DBTask getTaskId( ) const;
		DBIndex getRecordId( ) const;
		const Recurrence * getRecurrences( ) const;
		const fixed_vector<ValveSequence, MAX_VALVE_SEQUENCES_PER_PROGRAM, false> & getSequences( ) const;
		float getNominalFlow( ) const;
		int getNextGroupNumber( );
		int getNextSequenceNumber( );

		// Inherited from the static instance of an irrigation program
		ProgramPriority getPriority( ) const;
		ProgramMode getMode( ) const;
		DBDate getStartDate( ) const;
		DBDate getEndDate( ) const;
		DBTime getStartTime( ) const;
		long getStartTimeInSeconds( ) const;
		DBTime getEndTime( ) const;
		long getEndTimeInSeconds( ) const;
		DBTime getDelay( ) const;
		byte getNumberDays( ) const;
		const byte * getRecurrence( ) const;
		byte getNumberSequences( ) const;
		const byte * getSequence( ) const;
		IrrigationMethod getIrrigationMethod( ) const;		

		UIrrigationProgramProperties specific_properties { ICyclesProgramProperties cycles, ISensorProgramProperties sensor, IETProgramProperties et }
		
		// UIrrigationProgramProperties specific_properties:

		DBTime getMaxRestTime( ) const;
		DBIndex getSensorId( ) const;
		SensorType getSensorType( ) const;
		float getSetPoint( ) const;
		DBIndex getConditionId( ) const;
		DBIndex getETSensorId( ) const;
		int getETCropFactor( ) const;
		int getNumberCycles( ) const;
		// byte getETNumberCycles( ) const;

		ICurrentDay getCurrentDay( ) const;

		ICurrentDay current_day {byte day; Date last_change;}
		
		// const IrrigationProgram & getProgram( ) const;
		// bool operator==( const IrrigationProgram & program ) const;

		/////////////////////////////////////////////////////////////
		// Mutator functions
		////////////////////////////////////////////////////////////
		void setProgramId( DBIndex program_id );
		void setTaskId( DBTask task_id );
		void setRecordId( DBIndex record_id );
		void setRecurrences( const Recurrence * recurrences );
		void setSequences( const fixed_vector<ValveSequence, MAX_VALVE_SEQUENCES_PER_PROGRAM, false> & sequences );
		
		// Inherited from the running instance of an irrigation program
		void setProgramState( IrrigationSequenceState state );
		void setManagerRequest( IrrigationSequenceRequest request );
		void setAlarmId( DBIndex alarm_id );
		void setMessageBoxId( byte message_box_id );

		// Inherited from the static instance of an irrigation program
		void setPriority( ProgramPriority priority );
		void setMode( ProgramMode mode );
		void setStartDate( DBDate start_date );
		void setEndDate( DBDate end_date );
		void setStartTime( DBTime start_time );
		void setStartTimeInSeconds( long start_time_in_seconds );
		void setEndTime( DBTime end_time );
		void setEndTimeInSeconds( long end_time_in_seconds );
		void setDelay( DBTime delay );
		void setNumberDays( byte number_of_days );
		void setRecurrence( const byte * recurrence );
		void setNumberSequences( byte number_of_sequences );
		void setSequence( const byte * sequence  );
		void setIrrigationMethod( IrrigationMethod irrigation_method );

		
		// UIrrigationProgramProperties specific_properties;
		
		void setNumberCycles( int number_of_cycles );
		void setSensorId( DBIndex sensor_id );
		void setMaxRestTime( DBTime max_rest );
		void setSensorType( SensorType sensor_type );
		void setSetPoint( float set_point );
		void setConditionId( DBIndex condition_id );
		void setETSensorId( DBIndex sensor_id );
		void setETCropFactor( int crop_factor );
		void setETNumberCycles( byte number_of_cycles );

		void setProgram( const IrrigationProgram & program );

		bool Initialize( );					// Initializes Irrigation Program object

	private:

		void ConstructRecurrences( );		// Constructs recurrences 

		byte Trinity[MAX_VALVE_SEQUENCES][MAX_DEVICES];

		WORD error_code;					// Serves as a bucket to store Database error code
		ValveGroup active_valve_group;		// Serves as a bucket to store Valve Group

		// Array of active valve groups with as many members as there are Valve Sequences in the program
		// ValveGroup active_valve_groups[MAX_VALVE_SEQUENCES];

		fixed_vector<ValveSequence, MAX_VALVE_SEQUENCES_PER_PROGRAM, false> sequences;
		fixed_vector<ValveSequence, MAX_VALVE_SEQUENCES_PER_PROGRAM, false>::iterator sequence_itr;

		// Allocate memory on stack for the following variables:
		DBTask task_id;									// Task id; Range [? - ?]
		DBIndex program_id;								// Irrigation Program id; Range [0 - 49]
		
		Recurrence recurrences[MAX_RECURRENT_DAYS];	// Array of Enum Recurrence; Range [RECURRENCE_NONE, RECURRENCE_WATER, RECURRENCE_DOSING]
		byte number_of_sequences;
		
		// THE FOLLOWING RUNNING AND USER-DEFINED PROPERTIES ARE DECLARED IN THE DBIrrigationProgram STRUCTURE:
		
		// RUNNING PROPERTIES OF AN IRRIGATION PROGRAM INSTANCE:
		// IrrigationProgramState state;				// Irrigation program state 
		// IrrigationProgramRequest manager_request;	// Request for irrigation program operation received from the system manager
		// DBIndex alarm_id;							// Id of the irrigation program alarm record

		// USER-DEFINED PROPERTIES OF AN IRRIGATION PROGRAM INSTANCE:
		// ProgramPriority priority;					// Program priority; Range [PROGRAM_PRIORITY_NORMAL, PROGRAM_PRIORITY_HIGH, PROGRAM_PRIORITY_SUPERIOR]
		// ProgramMode mode;							// Enum Program mode; Range [PROGRAM_ENABLE, PROGRAM_DISABLE, PROGRAM_START, PROGRAM_STOP, PROGRAM_PAUSE, PROGRAM_RESUME]
		// DBDate start_date;							// Start date of the program; Range [dd/mm/yyyy]
		// DBDate end_date;								// End date of the program; Range [dd/mm/yyyy]
		// DBTime start_time;							// Start time of the program; Range [00:00 - 23:59]
		// DBTime end_time;								// End time of the program; Range [00:00 - 23:59]
		// DBTime delay;								// Time delay between irrigation cycles; Range [00:00 - 23:59]
		// byte number_of_days;							// Number of days in the recurrence; Range [0 - 13]
		// byte recurrence[14];							// None = 0, Water = 1, Dosing = 2
		// byte sequence[10];							// Array of indices into the DBValveSequence table; Range [0 - 9]
		// IrrigationMethod method;						// Enum IrrigationMethod; Range [IRRIGATION_METHOD_LOOP, IRRIGATION_METHOD_CYCLES, IRRIGATION_METHOD_RADSUM, IRRIGATION_METHOD_VPD, IRRIGATION_METHOD_CONDITIONS, IRRIGATION_METHOD_ET]
		// UIrrigationProgramProperties specific_properties;
		// ICurrentDay current_day;

		// UIrrigationProgramProperties specific_properties:

		// The following property is specific to Cycles Irrigation Program:
		// int number_of_cycles;								// Number of cycles to run between the dates and within the time window of opportunity

		// The following properties are common Sensor (VPD, RadSum, Conditions) Irrigation Program:
		// specific_properties.sensor.sensor_id;				// Sensor id; Range [??? - ???]
		// specific_properties.sensor.max_rest;					// Maximum resting time between the irrigation cycles (hh:mm); Range [00:00 - 23:59]
		// specific_properties.sensor.sensor_type;				// Sensor type [SENSOR_TYPE_VPD, SENSOR_TYPE_RAD_SUM, SENSOR_TYPE_CONDITIONS]
		// specific_properties.sensor.condition.set_point;		// Set point
		// specific_properties.sensor.condition.condition_id;	// Condition id

		// The following properties are specific to ET Irrigation Program:					
		// specific_properties.et.sensor_id;					// Sensor id; Range [??? - ???]
		// specific_properties.et.crop_factor;					// Crop factor
		// specific_properties.et.number_of_cycles;				// Number of cycles
};

#endif