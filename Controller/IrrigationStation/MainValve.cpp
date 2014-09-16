
// MainValve class implementation file

#include "MainValve.h"

#ifdef IRRIGATION_PROCESS_DEBUG
   #define DebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId) SendDebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId);
#else
   #define DebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId) /**/;
#endif

// Default constructor
MainValve::MainValve( ) 
{ 
	// empty body for now
} 

// Constructor
MainValve::MainValve( DBTask task_id, DBIndex valve_id ) : task_id( task_id ), valve_id( valve_id )
{	
	// Get the memory slot for temporarily storing the Main Valve record
	DBMainValve * db_valve = (DBMainValve *) DB_GetMemorySlot(task_id);

	// Retrieve record from the Database and store it in the db_valve structure
	if ( DB_GetRecord( DBMAINVALVE_TYPE, task_id, valve_id, db_valve ) )
	{
		// Set Main Valve object properties
		setRelayId( db_valve->relay_id );
		setRelayLocation( db_valve->relay_location );
		setLineId( db_valve->line_id );
		setState( db_valve->state );	
	}
}

// Destructor
MainValve::~MainValve( ) { }

/////////////////////////////////////////////////////////////
// Accessor functions
/////////////////////////////////////////////////////////////
DBTask MainValve::getTaskId( ) const { return task_id; }
DBIndex MainValve::getValveId( ) const { return valve_id; }
byte MainValve::getRelayId( ) const { return relay_id; }
IO_LOCATION MainValve::getRelayLocation( ) const { return relay_location; }
DBIndex MainValve::getLineId( ) const { return line_id; }
// DBIndex MainValve::getWaterMeterId( ) const { return water_meter_id; }
// int MainValve::getValveGroupCounter( ) const { return valve_group_counter; }
bool MainValve::getState( ) const { return state; }


/////////////////////////////////////////////////////////////
// Mutator functions
////////////////////////////////////////////////////////////
void MainValve::setTaskId( DBTask id ) { task_id = id; }
void MainValve::setValveId( DBIndex id ) { valve_id = id; }
void MainValve::setRelayId( byte id ) { relay_id = id; }
void MainValve::setRelayLocation( IO_LOCATION relay ) { relay_location = relay; }
void MainValve::setLineId( DBIndex id ) { line_id = id; }
// void MainValve::setWaterMeterId( DBIndex id ) { water_meter_id = id; }
// void MainValve::setValveGroupCounter( int counter ) { valve_group_counter = counter; }
void MainValve::setState( bool state ) { this->state = state; }

bool MainValve::Initialize( DBTask task_id, DBIndex valve_id )
{
	// DBMainValve db_valve;	// Serves as a "bucket" to store information retrieved from the Database
	bool operation_status = true;
	DBMainValve * db_valve = (DBMainValve *) DB_GetMemorySlot(task_id);
	// memset( &main_valve_variables, 0, sizeof(main_valve_variables) );

	// Retrieve record from the Database and store it in the db_valve structure
	if ( DB_GetRecord( DBMAINVALVE_TYPE, task_id, valve_id, db_valve ) )
	{
		// Set Main Valve object properties
		setTaskId( task_id );
		setValveId( valve_id );
		setRelayId( db_valve->relay_id );
		setRelayLocation( db_valve->relay_location );
		setLineId( db_valve->line_id );
		// setWaterMeterId( db_valve->water_meter_id );
		// setValveGroupCounter( 0 );

		// Ensure the Main Valve is closed; the valve state must be off by default
		setState( false );		
	}
	else
	{
		// Retrieve the last Database error code associated with the task id
		error_code = DB_GetLastErrorCode( task_id );

		// TO DO: Handle the error !!!
	}

	return operation_status;
}

bool MainValve::openValve( )
{
	// Check to see if the Main Valve has already been opened by an Irrgiation Valve in the Valve Group
	// Each Irrigation Valve group must open its respective Main Valves ONLY once 

	// Check to see if the valve is closed
	if ( !state )										
	{
		// Mark the valve as being opened
		setState( true );									
		
		// Update the state of the valve in the Database and check whether the update operation was successful
		if ( DB_UpdateField( DBMAINVALVE_TYPE, task_id, valve_id, DBMAINVALVE_STATE, &state ) )
		{
			DebugOutputRequest( 4, SEQUENCE_OPEN_MAIN_VALVE, task_id, valve_id );

			// valve_group_counter++;
		}
		else // The opening of the valve operation has failed	
		{	
			// Retrieve the last Database error code associated with the task id
			error_code = DB_GetLastErrorCode( task_id );

			// TO DO: Handle the error !!!

			// Mark the valve back as being closed
			setState( false );								
		}
	}

	return state;
}

bool MainValve::closeValve( )
{
	// Check to see if the valve is open
	if ( state )										
	{
		// Mark the valve as being closed
		setState( false );									
		
		// Update the state of the valve in the Database and check whether the update operation was successful
		if ( DB_UpdateField( DBMAINVALVE_TYPE, task_id, valve_id, DBMAINVALVE_STATE, &state ) )
		{
			DebugOutputRequest( 4, SEQUENCE_CLOSE_MAIN_VALVE, task_id, valve_id );

			// valve_group_counter--;
		}
		else // The opening of the valve operation has failed	
		{	
			// Retrieve the last Database error code associated with the task id
			error_code = DB_GetLastErrorCode( task_id );

			// TO DO: Handle the error !!!

			// Mark the valve back as being opened
			setState( true );								
		}
	}

	return state;
}

//void MainValve::setValve( const MainValve & valve )
//{
//	// Perform aliasing test
//	if ( this != &valve )    
//	{
//		setTaskId( valve.task_id );
//		setValveId( valve.valve_id );
//		setRelayId( valve.relay_id );
//		setRelayLocation( valve.relay_location );
//		setLineId( valve.line_id );
//		// setWaterMeterId( valve.water_meter_id );
//		setState( valve.state );
//		setValveGroupCounter( valve.valve_group_counter );
//	}
//}