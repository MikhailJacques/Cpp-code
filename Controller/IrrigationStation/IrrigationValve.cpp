
// IrrigationValve class implementation file

#include "IrrigationValve.h"

#ifdef IRRIGATION_PROCESS_DEBUG
   #define DebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId) SendDebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId);
#else
   #define DebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId) /**/;
#endif

extern "C" {
	#include "utility.h"
};

// Default constructor
IrrigationValve::IrrigationValve( )
{
	// Empty body for now
}

// Default constructor
IrrigationValve::IrrigationValve( DBTask task_id, DBIndex valve_id )
{
	// Get the memory slot for temporarily storing the Irrigation Valve record
	DBIrrigationValve * db_valve = (DBIrrigationValve *) DB_GetMemorySlot(task_id);

	// Retrieve Irrigation Valve record from the Database
	if ( DB_GetRecord( DBIRRIGATIONVALVE_TYPE, task_id, valve_id, db_valve ) )
	{
		// Set Irrigation Valve object properties
		setTaskId( task_id );
		setValveId( valve_id );
		setRelayId( db_valve->relay_id );
		setRelayLocation( db_valve->relay_location );
		setLineId( db_valve->line_id );
		setWaterMeterId( db_valve->water_meter_id );
		setIrrigationAreaUnits( db_valve->irrigation_area_units );
		setIrrigationArea( db_valve->irrigation_area );
		setNominalFlowRateCubicMetersPerHour( db_valve->nominal_flow );
		setOnDelay( db_valve->on_delay );
		setOffDelay( db_valve->off_delay );
		// setStaggerDelay( db_valve->stagger_delay );
		// setOverlapTime( db_valve->overlap_time );
		setDeviceState( DEVICE_STATE_NONE );
		setNumberCyclesElapsed( 0 );
		// Ensure the Irrigation Valve is closed; the valve's state must be off by default
		setState( false );
	}
	else
		error_code = DB_GetLastErrorCode( task_id );	// Get the last Database error code
}

// Destructor
IrrigationValve::~IrrigationValve( ) { }

/////////////////////////////////////////////////////////////
// Accessor functions
/////////////////////////////////////////////////////////////
DBTask IrrigationValve::getTaskId( ) const { return task_id; }
DBIndex IrrigationValve::getValveId( ) const { return valve_id; }
byte IrrigationValve::getRelayId( ) const { return relay_id; }
IO_LOCATION IrrigationValve::getRelayLocation( ) const { return relay_location; }
DBIndex IrrigationValve::getLineId( ) const { return line_id; }
DBIndex IrrigationValve::getWaterMeterId( ) const { return water_meter_id; }
IrrigationAreaUnits IrrigationValve::getIrrigationAreaUnits( ) const { return irrigation_area_units; }
float IrrigationValve::getIrrigationArea( ) const { return irrigation_area; }
float IrrigationValve::getNominalFlowRateCubicMetersPerHour( ) const { return nominal_flow; }
float IrrigationValve::getNominalFlowRateLitersPerSecond( ) const { return nominal_flow_rate_liters_per_second; }
// float IrrigationValve::getNominalFlowGallonsPerSecond( ) const { return nominal_flow_rate_gallons_per_second; }
DBTimePP IrrigationValve::getOnDelay( ) const 
{ 
	DBTimePP t;
	t.second = on_delay / 1000.0;
	t.minute = t.second / 60.0;
	t.hour = t.minute / 60.0;
	return t; 
}
DBTimePP IrrigationValve::getOffDelay( ) const 
{ 
	DBTimePP t;
	t.second = off_delay / 1000.0;
	t.minute = t.second / 60.0;
	t.hour = t.minute / 60.0;
	return t; 
}
// DBTimePP IrrigationValve::getStaggerDelay( ) const { return stagger_delay; }
// DBTimePP IrrigationValve::getOverlapTime( ) const { return overlap_time; }
// DBTimePP IrrigationValve::getOpenDelay( ) const { return open_delay; }
// DBTimePP IrrigationValve::getCloseDelay( ) const { return close_delay; }
DeviceState IrrigationValve::getDeviceState( ) const { return device_state; }
int IrrigationValve::getNumberCyclesElapsed( ) const { return number_cycles_elapsed; }
bool IrrigationValve::getState( ) const { return state; }
// const IrrigationValve & IrrigationValve::getValve( ) const { return *this; }
//bool IrrigationValve::operator==( const IrrigationValve & valve ) const
//{
//	if( this != &valve )
//	{
//		if( task_id == valve.task_id && valve_id == valve.valve_id )
//			return true;
//		else
//			return false;
//	}
//
//	return true;
//}

/////////////////////////////////////////////////////////////
// Mutator functions
////////////////////////////////////////////////////////////
void IrrigationValve::setTaskId( DBTask id ) { task_id = id; }
void IrrigationValve::setValveId( DBIndex id ) { valve_id = id; }
void IrrigationValve::setRelayId( byte id ) { relay_id = id; }
void IrrigationValve::setRelayLocation( IO_LOCATION location ) { relay_location = location; }
void IrrigationValve::setLineId( DBIndex id ) { line_id = id; }
void IrrigationValve::setWaterMeterId( DBIndex id ) { water_meter_id = id; }
void IrrigationValve::setIrrigationAreaUnits( IrrigationAreaUnits units ) { irrigation_area_units = units; }
void IrrigationValve::setIrrigationArea( float area ) { irrigation_area = area; }

void IrrigationValve::setNominalFlowRateCubicMetersPerHour( float flow )
{
	nominal_flow = flow;

	// Irrigation Process and ALL its components receive and process ALL data in metric units of measurement
	// ACRE		-> gallon/min or 0.0631 liters/second
	// HECTARE  -> m3/hour or 16.7 liters/minute or 0.2777 liters/second
}

// void IrrigationValve::setNominalFlowLitersPerSecond( float nominal_flow ) { nominal_flow_rate_liters_per_second = nominal_flow / 3600; }
// void IrrigationValve::setNominalFlowGallonsPerSecond( float nominal_flow ) { nominal_flow_rate_gallons_per_second = nominal_flow / 60; }
void IrrigationValve::setOnDelay( DBTimePP time ) { on_delay = time.ToMilliseconds( ); }
void IrrigationValve::setOffDelay( DBTimePP time ) { off_delay = time.ToMilliseconds( ); }
// void IrrigationValve::setStaggerDelay( DBTimePP time ) { stagger_delay = time; }
// void IrrigationValve::setOverlapTime( DBTimePP time ) { overlap_time = time; }
// void IrrigationValve::setOpenDelay( DBTimePP time ) { open_delay = time; }
// void IrrigationValve::setCloseDelay( DBTimePP time ) { close_delay = time; }
void IrrigationValve::setDeviceState( DeviceState state ) { device_state = state; }
void IrrigationValve::setNumberCyclesElapsed( int number ) { number_cycles_elapsed = number; }
void IrrigationValve::incrementNumberCyclesElapsed( ) { ++number_cycles_elapsed; }
void IrrigationValve::setState( bool state ) { this->state = state; }

//void IrrigationValve::setValve( const IrrigationValve & valve )
//{
//	// Perform aliasing test
//	if ( this != &valve )
//	{
//		setTaskId( valve.task_id );
//		setValveId( valve.valve_id );
//		setRelayId( valve.relay_id );
//		setRelayLocation( valve.relay_location );
//		setLineId( valve.line_id );
//		setWaterMeterId( valve.water_meter_id );
//		setIrrigationAreaUnits( valve.irrigation_area_units );
//		setIrrigationArea( valve.irrigation_area );
//		setNominalFlowRateCubicMetersPerHour( valve.nominal_flow );
//		setOnDelay( valve.on_delay );
//		setOffDelay( valve.off_delay );
//		// setStaggerDelay( valve.stagger_delay );
//		// setOverlapTime( valve.overlap_time );
//		// setOpenDelay( valve.open_delay );
//		// setCloseDelay( valve.close_delay );
//		setDeviceState( valve.device_state );
//		setNumberCyclesElapsed( valve.number_cycles_elapsed );
//		setState( valve.state );
//	}
//}

bool IrrigationValve::Initialize( DBTask task_id, DBIndex valve_id )
{
	bool operation_status = true;
	DBIrrigationValve * db_valve = (DBIrrigationValve *) DB_GetMemorySlot(task_id);

	// DBIrrigationValve db_valve;
	// memset( &valve_variables, 0, sizeof(valve_variables) );

	// FOR DEBUGING PURPOSES:
	// int db_valve_size = sizeof(db_valve);			// 31

	// Retrieve record from the Database and store it in the db_valve structure
	if ( DB_GetRecord( DBIRRIGATIONVALVE_TYPE, task_id, valve_id, db_valve ) )
	{
		// Set Irrigation Valve object properties
		setTaskId( task_id );
		setValveId( valve_id );
		setRelayId( db_valve->relay_id );
		setRelayLocation( db_valve->relay_location );
		setLineId( db_valve->line_id );
		setWaterMeterId( db_valve->water_meter_id );
		setIrrigationAreaUnits( db_valve->irrigation_area_units );
		setIrrigationArea( db_valve->irrigation_area );
		setNominalFlowRateCubicMetersPerHour( db_valve->nominal_flow );
		setOnDelay( db_valve->on_delay );
		setOffDelay( db_valve->off_delay );
		// setStaggerDelay( db_valve->stagger_delay );
		// setOverlapTime( db_valve->overlap_time );
		setDeviceState( DEVICE_STATE_NONE );
		setNumberCyclesElapsed( 0 );
		setState( false );				// Ensure the Irrigation Valve is closed; the valve state must be off by default

		// TEMPORARY WORKAROUND THE CURRENT BIZARRE REQUIREMENTS
		// DBTimePP time_zero(0,0,0);
		// setCloseDelay( time_zero );
		// setOpenDelay( time_zero );
		// convertDBTimesToSeconds( );
	}
	else
		error_code = DB_GetLastErrorCode( task_id );	// Get the last Database error code

	return operation_status;
}

bool IrrigationValve::openValve( )
{
	// Check to see if the valve is closed
	if ( !state )
	{
		// Check to see if the delay to open the valve has NOT been set
		if ( on_delay == 0 )
			device_state = DEVICE_STATE_ON;							// Set the device to be ready to be activated

		switch ( device_state )
		{
			case DEVICE_STATE_NONE:									// The valve is in neutral state

				timestamp = get_current_time(IN_MILLISECONDS); // DB_TimeStamp( );
				device_state = DEVICE_STATE_WAIT_ON;				// Commence the countdown until the device's open delay expiration
				break;

			case DEVICE_STATE_WAIT_ON:								// The device is waiting to be activated

				if ( time_interval_expired_in_milliseconds( timestamp, on_delay ) )
					device_state = DEVICE_STATE_ON;					// The device can now be activated

				break;

			case DEVICE_STATE_ON:									// The device is ready to be activated

				setState( true );									// Mark the valve as being opened

				// Update the state of the valve in the Database and check whether the update operation was successful
				if ( DB_UpdateField( DBIRRIGATIONVALVE_TYPE, task_id, valve_id, DBIRRIGATIONVALVE_STATE, &state ) )
				{
					start_time = DB_TimeNow( ); // get_current_time(IN_MILLISECONDS);
					DebugOutputRequest( 4, SEQUENCE_OPEN_IRRIGATION_VALVE, task_id, valve_id );
				}
				else // The opening of the valve operation has failed
				{
					error_code = DB_GetLastErrorCode( task_id );	// Get the last Database error code

					// Handle the error !!!

					setState( false );								// Mark the open valve operation as being unsuccessful
				}

				break;

			case DEVICE_STATE_WAIT_OFF:								// The valve is waiting to be closed

				device_state = DEVICE_STATE_NONE;
				break;

			case DEVICE_STATE_OFF:									// The valve is closed

				device_state = DEVICE_STATE_NONE;
				break;

			default: // nothing
		}

	} // end of the first if statement that checks the valve's state
	// else	{ // state = true - the valve was already in an open state }

	return state;
}

bool IrrigationValve::closeValve( )
{
	// Check to see if the valve is open
	if ( state )
	{
		// Check to see if the delay to close the valve has NOT been set
		if ( off_delay == 0 )
			device_state = DEVICE_STATE_OFF;						// Set the device to be ready to be deactivated

		switch ( device_state )
		{
			case DEVICE_STATE_NONE:									// The valve is in neutral state

				timestamp = get_current_time(IN_MILLISECONDS); // DB_TimeStamp( );
				device_state = DEVICE_STATE_WAIT_OFF;				// Commence the countdown until the device's close delay expiration
				break;

			case DEVICE_STATE_WAIT_OFF:								// The device is waiting to be deactivated

				if ( time_interval_expired_in_milliseconds( timestamp, off_delay ) )
					device_state = DEVICE_STATE_OFF;				// The device can now be deactivated

				break;

			case DEVICE_STATE_OFF:									// The device is ready to be deactivated

				setState( false );									// Mark the valve as being closed

				// Update the state of the valve in the Database and check whether the update operation was successful
				if ( DB_UpdateField( DBIRRIGATIONVALVE_TYPE, task_id, valve_id, DBIRRIGATIONVALVE_STATE, &state ) )
				{
					incrementNumberCyclesElapsed( );

					DebugOutputRequest( 4, SEQUENCE_CLOSE_IRRIGATION_VALVE, task_id, valve_id );

					// bool DB_CreateRecord(DBType type, DBTask creator, DBIndex index, bool force);
					// DBIndex DB_CreateAutoRecord(DBType type, DBTask creator, bool force);

					DBIrrigationLog db_log;
					DBIndex record_id = DB_CreateAutoRecord(DBIRRIGATIONLOG_TYPE, task_id, true);

					if ( !DB_GetRecordForUpdate(DBIRRIGATIONLOG_TYPE, task_id, record_id, &db_log) )
						error_code = DB_GetLastErrorCode( task_id );

					DBTimePP end_time = DB_TimeNow( );
					DBTimePP irrigation_duration = end_time - start_time;

					trigger = IRRIGATION_FINISH_TRIGGER_TIME;

					// Fill the record with data and update
					db_log.valve_id = valve_id;
					db_log.log_date = DB_DateNow( );
					db_log.log_time = end_time;
					db_log.trigger = trigger;
					db_log.flow = nominal_flow;																// m3/hr
					db_log.irrigation_duration = irrigation_duration;										// for now
					db_log.quantity_delivered = (nominal_flow / 3600) * irrigation_duration.ToSeconds( );	// m3/hr
					db_log.activation_number = number_cycles_elapsed;

					if ( !DB_UpdateRecord(DBIRRIGATIONLOG_TYPE, task_id, record_id, &db_log) )
						error_code = DB_GetLastErrorCode( task_id );

					// FOR DEBUGGING PURPOSES ONLY
					// DBIrrigationLog db_log_test;
					// if ( !DB_GetRecord(DBIRRIGATIONLOG_TYPE, task_id, record_id, &db_log_test) )
					// 	error_code = DB_GetLastErrorCode( task_id );

					// FOR DEBUGGING PURPOSES ONLY
					// byte test = 1;

					// DebugOutputRequest( 4, SEQUENCE_CLOSE_IRRIGATION_VALVE_1, task_id );
					//DB_UpdateField(DBIRRIGATIONLOG_TYPE, task_id, valve_id, DBIRRIGATIONLOG_LOG_DATE, &log_date );
					//DB_UpdateField(DBIRRIGATIONLOG_TYPE, task_id, valve_id, DBIRRIGATIONLOG_LOG_TIME, &log_time );
					//DB_UpdateField(DBIRRIGATIONLOG_TYPE, task_id, valve_id, DBIRRIGATIONLOG_LOG_TIME, &trigger );
					//DB_UpdateField(DBIRRIGATIONLOG_TYPE, task_id, valve_id, DBIRRIGATIONLOG_LOG_TIME, &quantity_delivered );
					//DB_UpdateField(DBIRRIGATIONLOG_TYPE, task_id, valve_id, DBIRRIGATIONLOG_LOG_TIME, &irrigation_duration );
					//DB_UpdateField(DBIRRIGATIONLOG_TYPE, task_id, valve_id, DBIRRIGATIONLOG_LOG_TIME, &flow );
					//DB_UpdateField(DBIRRIGATIONLOG_TYPE, task_id, valve_id, DBIRRIGATIONLOG_LOG_TIME, &elapsed_cycles );

					// closed = true;
				}
				else // The closing of the valve operation has failed
				{
					error_code = DB_GetLastErrorCode( task_id );	// Get the last Database error code

					// Handle the error !!!

					setState( true );								// Mark the close valve operation as being unsuccessful
				}

				break;

			case DEVICE_STATE_WAIT_ON:								// The valve is waiting to be opened

				device_state = DEVICE_STATE_NONE;
				break;

			case DEVICE_STATE_ON:									// The valve is open

				device_state = DEVICE_STATE_NONE;
				break;

			default: // nothing
		}

	}		// end of the first if statement that checks the Irigation Valve's state
	// else	{ // state = false - the valve was already in a closed state }

	return state;
}