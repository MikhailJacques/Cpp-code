
// Irrigation Program class implementation file

#include "IrrigationProgram.h"

// Global array of indices that is stored in the ProcessExecution_Manager.c file
extern DBIndex IrrigationProgramID[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
extern DBIndex IrrigationSequenceID[MAX_CONCURRENT_IRRIGATION_SEQUENCES];

// Default constructor
IrrigationProgram::IrrigationProgram( ) 
{ 	
	// Empty body for now
}

// Constructor
IrrigationProgram::IrrigationProgram( DBTask task_id ) : task_id( task_id )
{	
	// First calculate program index in the programs' array (Range [0 - 9]) 
	// and then retrieve program record Database table index (Range [0 -49])
	program_id = IrrigationProgramID[task_id - MLINE1_SLOT]; 

	// Calculate the message box id (MLINE1_SLOT = 7 and MBOX_IRR_PROG_1 = 11 defined in tasks.h file)
	// message_box_id = task_id - MLINE1_SLOT + MBOX_IRR_PROG_1; 

	// Set message box - called only once
	// Tells that a task with the task_id has message box with the message_box_id where it receives messages
	// DB_SetMessageBox( task_id, message_box_id ); 

	// Register listener (task_id) with a particular record (record_id) in a particular table (table_type) 
	// in order to be able to check (i.e. listen) to any changes in the record's field values
	// bool DB_AddRecordListener(DBType type, DBIndex index, DBTask listener);
	// DB_AddRecordListener( DBIRRIGATIONPROGRAM_TYPE, record_id, task_id );

	// Reset Trinity
	//for ( int sequence_id = 0; sequence_id < MAX_VALVE_SEQUENCES; ++sequence_id )
	//	for ( int valve_id = 0; valve_id < MAX_DEVICES; ++valve_id )
	//		Trinity[sequence_id][valve_id] = 100; // Valid Group Id range: [0 - 99]

	// group_number = 0;
	// group_itr = valve_group_number.begin( );

	// sequence_number = 0;
	// sequence_itr = sequences.begin( );
} 

// Destructor
IrrigationProgram::~IrrigationProgram( )  {  }

/////////////////////////////////////////////////////////////
// Accessor functions
/////////////////////////////////////////////////////////////
byte IrrigationProgram::getProgramId( ) const { return program_id; }
DBTask IrrigationProgram::getTaskId( ) const { return task_id; }
DBIndex IrrigationProgram::getRecordId( ) const { return record_id; }
const Recurrence * IrrigationProgram::getRecurrences( ) const { return recurrences; }
// const fixed_vector<ValveSequence, MAX_VALVE_SEQUENCES_PER_PROGRAM, false> & IrrigationProgram::getSequences( ) const { return sequences; }
// byte IrrigationProgram::getNumberSequences( ) const { return number_of_sequences; }
IrrigationProgramState IrrigationProgram::getProgramState( ) const { return state; }
ProgramPriority IrrigationProgram::getPriority( ) const { return priority; }
ProgramMode IrrigationProgram::getMode( ) const { return mode; }
DBDate IrrigationProgram::getStartDate( ) const { return start_date; }
DBDate IrrigationProgram::getEndDate( ) const { return end_date; }
DBTime IrrigationProgram::getStartTime( ) const { return start_time; }
long IrrigationProgram::getStartTimeInSeconds( ) const { return start_time_in_seconds; }
DBTime IrrigationProgram::getEndTime( ) const { return end_time; }
long IrrigationProgram::getEndTimeInSeconds( ) const { return end_time_in_seconds; }
DBTime IrrigationProgram::getDelay( ) const { return delay; }
byte IrrigationProgram::getNumberDays( ) const { return number_of_days; }

// TO DO: VERIFY THAT IT WORKS PROPERLY !!!
const byte * IrrigationProgram::getRecurrence( ) const { return recurrence; }

// TO DO: VERIFY THAT IT WORKS PROPERLY !!!
const byte * IrrigationProgram::getSequence( ) const { return sequence; }

IrrigationMethod IrrigationProgram::getIrrigationMethod( ) const { return method; }
int IrrigationProgram::getNumberCycles( ) const { return specific_properties.cycles.number_of_cycles; }
DBIndex IrrigationProgram::getSensorId( ) const { return specific_properties.sensor.sensor_id; }
DBTime IrrigationProgram::getMaxRestTime( ) const{ return specific_properties.sensor.max_rest; }
SensorType IrrigationProgram::getSensorType( ) const { return specific_properties.sensor.sensor_type; }
float IrrigationProgram::getSetPoint( ) const { return specific_properties.sensor.condition.set_point; }
DBIndex IrrigationProgram::getConditionId( ) const { return specific_properties.sensor.condition.condition_id; }
DBIndex IrrigationProgram::getETSensorId( ) const { return specific_properties.et.sensor_id; }
int IrrigationProgram::getETCropFactor( ) const { return specific_properties.et.crop_factor; }
byte IrrigationProgram::getETNumberCycles( ) const { return specific_properties.et.number_of_cycles; }
ICurrentDay IrrigationProgram::getCurrentDay( ) const { return current_day; }
// typedef struct { byte day; DBDate last_change; } ICurrentDay;

const IrrigationProgram & IrrigationProgram::getProgram( ) const { return *this;}

/////////////////////////////////////////////////////////////
// Mutator functions
////////////////////////////////////////////////////////////
void IrrigationProgram::setProgramId( byte program_id ) { this->program_id = program_id; }
void IrrigationProgram::setTaskId( DBTask task_id ) { this->task_id = task_id; }
void IrrigationProgram::setRecordId( DBIndex record_id ) { this->record_id = record_id; }
void IrrigationProgram::setRecurrences( const Recurrence * recurrences ) 
{ 
	memcpy( this->recurrences, recurrences, sizeof(this->recurrences) );
	//for ( int i = 0; i < number_of_days; i++ )
	//	this->recurrences[i] = recurrences[i];
}

//void IrrigationProgram::setSequences( const fixed_vector<ValveSequence, MAX_VALVE_SEQUENCES_PER_PROGRAM, false> & sequences )
//{
//	// Copy all of sequence object into this->sequence object
//	copy( sequences.begin(), sequences.end(), this->sequences.begin() );    
//}

// void IrrigationProgram::setNumberSequences( byte number ) { number_of_sequences = number; }
void IrrigationProgram::setProgramState( IrrigationProgramState state ) { this->state = state; }
void IrrigationProgram::setPriority( ProgramPriority priority ) { this->priority = priority; }
void IrrigationProgram::setMode( ProgramMode mode ) { this->mode = mode; }
void IrrigationProgram::setStartDate( DBDate date ) { start_date = date; }
void IrrigationProgram::setEndDate( DBDate date ) { end_date = date; }
void IrrigationProgram::setStartTime( DBTime time ) { start_time = time; }
void IrrigationProgram::setStartTimeInSeconds( long time ) { start_time_in_seconds = time; }
void IrrigationProgram::setEndTime( DBTime time ) { end_time = time; }
void IrrigationProgram::setEndTimeInSeconds( long time ) { end_time_in_seconds = time; }
void IrrigationProgram::setDelay( DBTime delay ) { this->delay = delay; }
void IrrigationProgram::setNumberDays( byte number_of_days ) { number_of_days = days; }
void IrrigationProgram::setRecurrence( const byte * recurrence )
{
	memcpy( this->recurrence, recurrence, sizeof(this->recurrence) );
	//for ( int i = 0; i < number_of_days; i++ )
	//	this->recurrence[i] = recurrence[i];
}

void IrrigationProgram::setSequence( const byte * sequence )
{
	memcpy( this->sequence, sequence, sizeof(this->sequence) )
	//for ( int i = 0; i < number_of_sequences; i++ )
	//	this->sequence[i] = sequence[i];
}

void IrrigationProgram::setIrrigationMethod( IrrigationMethod method ) { this->method = method; }
void IrrigationProgram::setNumberCycles( int number_of_cycles ) { specific_properties.cycles.number_of_cycles = number_of_cycles; }
void IrrigationProgram::setSensorId( DBIndex sensor_id ) { specific_properties.sensor.sensor_id = sensor_id; }
void IrrigationProgram::setMaxRestTime( DBTime max_rest ) { specific_properties.sensor.max_rest = max_rest; }
void IrrigationProgram::setSensorType( SensorType sensor_type ) { specific_properties.sensor.sensor_type = sensor_type; }
void IrrigationProgram::setSetPoint( float set_point ) { specific_properties.sensor.condition.set_point = set_point; }
void IrrigationProgram::setConditionId( DBIndex condition_id ) { specific_properties.sensor.condition.condition_id = condition_id; }
void IrrigationProgram::setETSensorId( DBIndex sensor_id ) { specific_properties.et.sensor_id = sensor_id; }
void IrrigationProgram::setETCropFactor( int crop_factor ) { specific_properties.et.crop_factor = crop_factor; }
void IrrigationProgram::setETNumberCycles( byte number_of_cycles ) { specific_properties.et.number_of_cycles = number_of_cycles; }

bool IrrigationProgram::Initialize( )
{  
	// FOR DEBUGGING PURPOSES:
	// int db_program_size = sizeof(DBIrrigationProgram);	// 64

	bool operation_status = true;

	// Get the memory slot for temporarily storing the Irrigation Program record
	DBIrrigationProgram * db_program = (DBIrrigationProgram *) DB_GetMemorySlot(task_id);

	// Retrieve Irrigation Program record from the database
	DB_GetRecord( DBIRRIGATIONPROGRAM_TYPE, task_id, record_id, db_program );
	
	// Set Irrigation Program object properties:
	setPriority( db_program->priority );
	setMode( db_program->mode );
	setStartDate( db_program->start_date );
	setEndDate( db_program->end_date );
	setStartTime( db_program->start_time );
	setEndTime( db_program->end_time );
	
	setDelay( db_program->delay );	
	setNumberDays( db_program->number_of_days );
	setRecurrence( db_program->recurrence );
	setNumberSequences( MAX_VALVE_SEQUENCES_PER_PROGRAM );
	setSequence( db_program->sequence );
	setIrrigationMethod( db_program->method );

	// The following properties are specific to an Irrigation Program type:
	switch ( method )
	{
		case IRRIGATION_METHOD_CYCLES:

			setNumberCycles( db_program->specific_properties.cycles.number_of_cycles );
			break;

		case IRRIGATION_METHOD_SENSOR:

			setSensorId( db_program->specific_properties.sensor.sensor_id );
			setMaxRestTime( db_program->specific_properties.sensor.max_rest );
			setSensorType( db_program->specific_properties.sensor.sensor_type );

			if ( getSensorType( ) == SENSOR_TYPE_VPD || getSensorType( ) == SENSOR_TYPE_RAD_SUM )
				setSetPoint( db_program->specific_properties.sensor.condition.set_point );
			else
				setConditionId( db_program->specific_properties.sensor.condition.condition_id );
			
			break;

		case IRRIGATION_METHOD_ET:

			setETSensorId( db_program->specific_properties.et.sensor_id );
			setETCropFactor( db_program->specific_properties.et.crop_factor );
			setETNumberCycles( db_program->specific_properties.et.number_of_cycles );
			break;

		default: // IRRIGATION_METHOD_LOOP:

			// none
			break;
	}

	ConstructRecurrences( );

	// setProgramState( PROGRAM_INITIALIZED );

	return operation_status;
}

// Constructs recurrence 
void IrrigationProgram::ConstructRecurrences( )
{ 
	// Step over and process Recurrences one day at a time: 0 = None, 1 = Water, 2 = Dosing
	for ( int day = 0; day < number_of_days; day++ )
	{
		if ( recurrence[day] == 1 )
			recurrences[day] = RECURRENCE_WATER;
		else if ( recurrence[day] == 2 )
			recurrences[day] = RECURRENCE_DOSING;
		else
			recurrences[day] = RECURRENCE_NONE;
	}
}


//void IrrigationProgram::setProgram( const IrrigationProgram & program )
//{
//	// Perform aliasing test
//	if ( this != &program )    
//	{
//		// Set Irrigation Program properties
//
//		// The following properties are common to all types of Irrigation Programs:
//		setTaskId( program.getTaskId( ) );
//		setRecordId( program.getRecordId( ) );
//		setNumberDays( program.getNumberDays( ) );
//		setRecurrences( program.getRecurrences( ) );
//		// setSequences( program.getSequences( ) );
//		// setNumberSequences( program.getNumberSequences( ) );
//		setProgramState( program.getProgramState( ) );
//		setPriority( program.getPriority( ) );
//		setMode( program.getMode( ) );
//		setStartDate( program.getStartDate( ) );
//		setEndDate( program.getEndDate( ) );
//		setStartTime( program.getStartTime( ) );
//		setStartTimeInSeconds( program.getStartTimeInSeconds( ) );
//		setEndTime( program.getEndTime( ) );
//		setEndTimeInSeconds( program.getEndTimeInSeconds( ) );
//		setDelay( program.getDelay( ) );
//		setRecurrence( program.getRecurrence( ) );
//		setSequence( program.getSequence( ) );
//		setIrrigationMethod( program.getIrrigationMethod( ) );
//
//		// The following properties are specific to Irrigation Program type:
//
//		switch ( method )
//		{
//			case IRRIGATION_METHOD_CYCLES:
//
//				setNumberCycles( program.getNumberCycles( ) );
//				break;
//
//			case IRRIGATION_METHOD_SENSOR:
//
//				setSensorId( program.getSensorId( ) );
//				setMaxRestTime( program.getMaxRestTime( ) );
//				setSensorType( program.getSensorType( ) );
//
//				if ( getSensorType( ) == SENSOR_TYPE_VPD || getSensorType( ) == SENSOR_TYPE_RAD_SUM )
//					setSetPoint( program.getSetPoint( ) );
//				else
//					setConditionId( program.getConditionId( ) );				
//				
//			case IRRIGATION_METHOD_ET:
//
//				setETSensorId( program.getETSensorId( ) );
//				setETCropFactor( program.getETCropFactor( ) );
//				setETNumberCycles( program.getETNumberCycles( ) );
//				break;
//
//			default: // IRRIGATION_METHOD_LOOP:
//				// none
//				break;
//		}
//	}
//} 

//// Constructs sequences of irrigation valve groups
//void IrrigationProgram::ConstructValveSequences( )
//{ 
//	bool last_valve;				// Keeps track of the last valve in the group/sequence
//	int group_id;					// Keeps track of the valve group id; Range [0 - 99]
//	// int number_of_valves;		// Keeps track of the number of valves in each sequence; Range [0 - 99]
//	// int sequence_nominal_flow;	// Keeps track of the sum total of the nominal flows of all the valves in the sequence
//	// DBValveSequence program_variables.db_sequence;	// Temporarily stores contents of the Database Valve Sequence structure	
//	// DBValveSequence * program_variables.db_sequence = (DBValveSequence *) DB_GetMemorySlot(task_id);
//
//	// FOR DEBUGGING PURPOSES:
//	int program_variables.valve_sequence_size_1 = sizeof(DBValveSequence);				// 399
//	int program_variables.valve_sequence_size_2 = sizeof(program_variables.program_variables.db_sequence);	// 399
//
//	// Step over and process Valve Sequences one sequence at a time
//	for ( int s = 0; s < MAX_VALVE_SEQUENCES_PER_PROGRAM; s++ )
//	{
//		// Reset tracking program_variables
//		group_id = 0;
//		// number_of_valves = 0;
//		// sequence_nominal_flow = 0;
//			
//		// byte sequence[10]; Range [0 - 149]
//		int sequence_id = sequence[s]; 
//
//		// Retrieve Valve Sequence record from the Database and store it in the program_variables.db_sequence structure
//		DB_GetRecord( DBVALVESEQUENCE_TYPE, task_id, sequence_id, &program_variables.program_variables.db_sequence );
//
//		// CHECK FOR DB ERRORS !!!
//
//		// Process only the NON-EMPTY valve sequences
//		if ( program_variables.program_variables.db_sequence.valves[0] != 0 )
//		{			
//			// MOVED TO GLOBAL MEMORY SPACE:
//
//			// Create temporary group of irrigation valves; Range [0 - 99]
//			// fixed_vector<IrrigationValve, MAX_VALVES_PER_SEQUENCE, false> temp_group_vector;
//
//			// Create temporary sequence of irrigation valve groups; Range [0 - 99]
//			// fixed_vector<ValveGroup, MAX_GROUPS_PER_SEQUENCE, false> * temp_sequence_vector;
//
//			// FOR DEBUGGING PURPOSES:
//			int valve_group_vector_size = sizeof(program_variables.valve_group_vector);		// 256
//			int sequence_vector_size = sizeof(program_variables.sequence_vector);			// 1072
//
//			// Iterate over all the irrigation valves in the sequence; Range [0 - 99]
//			for ( int v = 0; program_variables.program_variables.db_sequence.valves[v] != 0; v++ )
//			{
//				// Convert the binary valve id value into the decimal one
//				DBIndex valve_id = static_cast<DBIndex>( program_variables.program_variables.db_sequence.valves[v] );
//
//				// Construct an Irrigation Valve object - retrieve the respective Irrigation Valve from the database
//                program_variables.valve.Initialize( task_id, valve_id - 1 ); 
//
//				// Update Sequence, Valve, Valve Group matrix
//				Trinity[sequence_id][valve_id - 1] = group_id;
//
//				int valve_size_1 = sizeof(IrrigationValve);				// 60
//				int valve_size_2 = sizeof(program_variables.valve);		// 60
//
//				// sequence_nominal_flow += valve.getNominalFlow( );
//
//				// Add the valve to the temporary Valve Group
//				program_variables.valve_group_vector.push_back( program_variables.valve );
//
//				// FOR DEBUGGING PURPOSES:
//				int group_size = program_variables.valve_group_vector.size( );
//
//				// Group valves within the sequence
//				// The sign bit value "0" denotes "+", which means there are more valves in the group
//				// The sign bit value "1" denotes ",", which means there are no more valves in the group
//				// bool GetBit(char* bitfield, DBIndex index);
//				// Check to see if it is the last valve in the group
//				last_valve = (bool) GetBit( program_variables.program_variables.db_sequence.signs, v );
//				
//				// Check to see if it is the last valve in the sequence
//				// This check is needed until Iliya fixes the code in such a way that 
//				// the user-defined sequence of valves will allways end ","
//				if ( (v + 1) < MAX_VALVES_PER_SEQUENCE )
//				{
//					if ( program_variables.program_variables.db_sequence.valves[v + 1] == 0 )
//						last_valve = true;
//				}
//				else
//					last_valve = true;
//
//
//				if ( last_valve )
//				{
//					// Initialize a valve group object
//					program_variables.valve_group.Initialize( program_variables.valve_group_vector, group_id );
//
//					// FOR DEBUGGING PURPOSES:
//					int valve_group_size_1 = sizeof(ValveGroup);						// 264
//					int valve_group_size_2 = sizeof(program_variables.valve_group);		// 264
//						
//					// AT THIS POINT WE HAVE A FULLY CONSTRUCTED GROUP OF VALVES
//
//					// FOR TESTING PURPOSES:
//					int size_before_1 = program_variables.sequence_vector.size( );
//
//					// Add the valve group to the temporary valve sequence	
//					// KAPUT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//					// fixed_vector<ValveGroup, MAX_GROUPS_PER_SEQUENCE, false> sequence_vector;
//					// ValveGroup valve_group;
//					program_variables.sequence_vector.push_back( program_variables.valve_group );
//
//					// FOR TESTING PURPOSES:
//					int size_after_1 = program_variables.sequence_vector.size( ); // 3011295
//
//					// FOR TESTING PURPOSES:
//					int size_before_2 = program_variables.valve_group_vector.size( );
//
//					// Empty the temporary group of valves
//					program_variables.valve_group_vector.erase( program_variables.valve_group_vector.begin(), program_variables.valve_group_vector.end() );
//
//					// FOR TESTING PURPOSES:
//					int size_after_2 = program_variables.valve_group_vector.size( );
//
//					group_id++;
//
//					last_valve = false;
//				} 
//
//				// Save the number of valves in the sequence
//				// number_of_valves = v;
//
//				// Save the number of valve groups in the sequence
//				// number_of_groups = group_id;
//			}
//
//			// Update number of valves in the sequence
//			// number_of_valves = number_of_valves + 1; 
//
//			// Initialize a Valve Sequence object
//			program_variables.program_variables.valve_sequence.Initialize( program_variables.sequence_vector, s );
//
//			// FOR DEBUGGING PURPOSES:
//			int program_variables.valve_sequence_size1 = sizeof(ValveSequence);						// 6660
//			int program_variables.valve_sequence_size2 = sizeof(program_variables.program_variables.valve_sequence);	// 6660
//
//			// FOR TESTING PURPOSES:
//			int size_before_3 = program_variables.sequence_vector.size( );
//
//			// Empty the temporary sequence of valve groups since it has been copied to the valve sequence object
//			program_variables.sequence_vector.erase( program_variables.sequence_vector.begin(), program_variables.sequence_vector.end() );
//			
//			// FOR TESTING PURPOSES:
//			int size_after_3 = program_variables.sequence_vector.size( );
//
//			// Set all other Valve Sequence properties
//			// temp_program_variables.valve_sequence.setNumberValves( number_of_valves );
//			// temp_program_variables.valve_sequence.setNominalFlow( sequence_nominal_flow );
//			program_variables.program_variables.valve_sequence.setDosingStationId( program_variables.program_variables.db_sequence.dosing_station_id );
//			program_variables.program_variables.valve_sequence.setDosingProgramId( program_variables.program_variables.db_sequence.dosing_program_id );
//			program_variables.program_variables.valve_sequence.setIrrigationType( program_variables.program_variables.db_sequence.irrigation_type );
//
//			// IFertigationRecipe recipe[8];
//			// void setFertigationRecipe( IFertigationRecipe * recipe );
//
//			// ENSURE THAT IT WORKS PROPERLY AS IT DOES NOT SEEM TO PASS ARRAY program_variables.db_sequence->recipe !!!!!
//			program_variables.program_variables.valve_sequence.setFertigationRecipe( program_variables.program_variables.db_sequence.recipe );
//
//			// Set the appropriate irrigation quantity/duration value based on the user-defined irrigation type
//			switch ( program_variables.program_variables.valve_sequence.getIrrigationType( ) )
//			{
//				case IRRIGATION_BY_DEPTH:
//					
//					program_variables.program_variables.valve_sequence.setIrrigationDepthSet( program_variables.program_variables.db_sequence.set.depth );
//					program_variables.program_variables.valve_sequence.setIrrigationDepthLeft( program_variables.program_variables.db_sequence.left.depth );
//					break;
//
//				case IRRIGATION_BY_QUANTITY:
//					
//					program_variables.program_variables.valve_sequence.setIrrigationQuantitySet( program_variables.program_variables.db_sequence.set.quantity );
//					program_variables.program_variables.valve_sequence.setIrrigationQuantityLeft( program_variables.program_variables.db_sequence.left.quantity );
//					break;
//		
//				case IRRIGATION_BY_DURATION:
//					
//					program_variables.program_variables.valve_sequence.setIrrigationDurationSet( program_variables.program_variables.db_sequence.set.duration );
//					program_variables.program_variables.valve_sequence.setIrrigationDurationLeft( program_variables.program_variables.db_sequence.left.duration );
//					break;
//
//				default:	// none
//			}
//
//			program_variables.program_variables.valve_sequence.setState( false );
//
//			// AT THIS POINT WE HAVE A FULLY CONSTRUCTED SEQUENCE OF IRRIGATION VALVE GROUPS
//
//			// Add the sequence object to the vector of irrigation valves sequences in this Irrigation Program
//			sequences.push_back( program_variables.program_variables.valve_sequence );
//      
//			// FOR DEBUGGING PURPOSES
//			int sequence_size = sequences.size(); 
//		}
//	} 
//}