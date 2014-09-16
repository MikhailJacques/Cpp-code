
// Water Meter class implementation file

#include "WaterMeter.h"

#ifdef IRRIGATION_PROCESS_DEBUG
   #define DebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId) SendDebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId);
#else
   #define DebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId) /**/;
#endif

extern "C" {
	#include "utility.h"
};

// Default constructor
WaterMeter::WaterMeter( ) 
{ 
	// empty body for now
} 

// Constructor
WaterMeter::WaterMeter( DBTask task_id, DBIndex water_meter_id ) : task_id( task_id ), water_meter_id( water_meter_id )
{	
	// Get the memory slot for temporarily storing the Water Meter record
	DBWaterMeter * db_water_meter = (DBWaterMeter *) DB_GetMemorySlot(task_id);

	// Retrieve the Water Meter record from the Database
	if ( DB_GetRecord( DBWATERMETER_TYPE, task_id, water_meter_id, db_water_meter ) )
	{
		// Set Water Meter object properties
		setInputId( db_water_meter->input_id );
		setRelayLocation( db_water_meter->input_location );
		setAllocateeType( db_water_meter->allocatee_type );
		setAllocateeId( db_water_meter->allocatee_id );
		setMainLineId( db_water_meter->main_line_id );	// TO DO: May be redundant !!!
		setRatio( db_water_meter->ratio );
		setIntervalPulses( db_water_meter->interval_pulses );
		setTotalPulses( db_water_meter->total_pulses );
		setFirstPulseTimestamp( db_water_meter->first_pulse_timestamp );	
		setLastPulseTimestamp( db_water_meter->last_pulse_timestamp );	
		setWaterLeakQuantity( db_water_meter->water_leak_quantity );
		setWaterLeakPeriod( db_water_meter->water_leak_period );
		setState( false );

		setMissedPulsesCounter( 0 );
		setNominalFlowRateLitersPerSecond( 0 );
		setMeasuredFlowRateLitersPerSecond( 0 );
		// setElapsedTimeSinceLastPulse( 0 );
		setNominalTimeIntervalBetweenPulses( 0 );
		setMeasuredTimeIntervalBetweenPulses( 0 );
		setMaxAllowedTimeIntervalBetweenPulses( 0 );

		DB_AddRecordListener( DBWATERMETER_TYPE, water_meter_id, task_id );
	}
}

// Destructor
WaterMeter::~WaterMeter( ) { }

/////////////////////////////////////////////////////////////
// Accessor functions
/////////////////////////////////////////////////////////////
DBTask WaterMeter::getTaskId( ) const { return task_id; }
DBIndex WaterMeter::getWaterMeterId( ) const { return water_meter_id; }
byte WaterMeter::getInputId( ) const { return input_id; }
IO_LOCATION WaterMeter::getRelayLocation( ) const { return input_location; }
byte WaterMeter::getAllocateeType( ) const { return allocatee_type; }
DBIndex WaterMeter::getAllocateeId( ) const { return allocatee_id; }
DBIndex WaterMeter::getMainLineId( ) const { return main_line_id; }
float WaterMeter::getRatio( ) const { return ratio; }
byte WaterMeter::getIntervalPulses( ) const { return interval_pulses; }
int WaterMeter::getTotalPulses( ) const { return total_pulses; }
long WaterMeter::getFirstPulseTimestamp( ) const { return first_pulse_timestamp; }	
long WaterMeter::getLastPulseTimestamp( ) const { return last_pulse_timestamp; }	
float WaterMeter::getWaterLeakQuantity( ) const { return water_leak_quantity; }
DBTime WaterMeter::getWaterLeakPeriod( ) const { return water_leak_period; }
bool WaterMeter::getState( ) const { return state; }
bool WaterMeter::operator<( const WaterMeter & water_meter ) const { return water_meter_id < water_meter.getWaterMeterId( ); }
//bool WaterMeter::operator==( const WaterMeter & water_meter ) const
//{
//	// Test for aliasing
//	if( this != &water_meter )      
//	{
//		if ( water_meter_id == water_meter.water_meter_id )
//			return true;
//		else
//			return false;
//	}
//	return true;
//}

int WaterMeter::getMissedPulsesCounter( ) const { return missed_pulses_counter; }
float WaterMeter::getNominalFlowRateLitersPerSecond( ) const { return nominal_flow_rate_liters_per_second; }
float WaterMeter::getMeasuredFlowRateLitersPerSecond( ) const { return measured_flow_rate_liters_per_second; }
// long WaterMeter::setElapsedTimeSinceLastPulse(  ) const { return elapsed_time_since_last_pulse; }
long WaterMeter::getNominalTimeIntervalBetweenPulses( ) const { return nominal_time_interval_between_pulses; }
long WaterMeter::getMeasuredTimeIntervalBetweenPulses( ) const { return measured_time_interval_between_pulses; }	
long WaterMeter::getMaxAllowedTimeIntervalBetweenPulses( ) const { return max_allowed_time_interval_between_pulses; }		


/////////////////////////////////////////////////////////////
// Mutator functions
////////////////////////////////////////////////////////////
void WaterMeter::setTaskId( DBTask id ) { task_id = id; }
void WaterMeter::setWaterMeterId( DBIndex id ) { water_meter_id = id; }
void WaterMeter::setInputId( byte id ) { input_id = id; }
void WaterMeter::setRelayLocation( IO_LOCATION relay ) { input_location = relay; }
void WaterMeter::setAllocateeType( byte type ) { allocatee_type = type; }
void WaterMeter::setAllocateeId( DBIndex id ) { allocatee_id = id; }
void WaterMeter::setMainLineId( DBIndex id ) { main_line_id = id; }
void WaterMeter::setRatio( float value ) { ratio = value; }
void WaterMeter::setIntervalPulses( byte value ) { interval_pulses = value; }
void WaterMeter::addIntervalPulses( int value ) { interval_pulses += value; }
void WaterMeter::setTotalPulses( int value ) { total_pulses = value; }
void WaterMeter::setFirstPulseTimestamp( long time ) { first_pulse_timestamp = time; }	
void WaterMeter::setLastPulseTimestamp( long time ) { last_pulse_timestamp = time; }
void WaterMeter::setWaterLeakQuantity( float quantity ) { water_leak_quantity = quantity; }
void WaterMeter::setWaterLeakPeriod( DBTime time ) { water_leak_period = time; }
void WaterMeter::setState( bool state ) { this->state = state; }
void WaterMeter::setMissedPulsesCounter( int counter ) { missed_pulses_counter = counter; }
void WaterMeter::setNominalFlowRateLitersPerSecond( float flow_rate ) { nominal_flow_rate_liters_per_second = flow_rate; }
// void WaterMeter::setElapsedTimeSinceLastPulse( long milliseconds ) { elapsed_time_since_last_pulse = milliseconds; } 
void WaterMeter::setMeasuredFlowRateLitersPerSecond( float flow_rate ) { measured_flow_rate_liters_per_second = flow_rate; }
void WaterMeter::setNominalTimeIntervalBetweenPulses( long time ) { nominal_time_interval_between_pulses = time; }
void WaterMeter::setMeasuredTimeIntervalBetweenPulses( long time ) { measured_time_interval_between_pulses = time; }
void WaterMeter::setMaxAllowedTimeIntervalBetweenPulses( long time ) { max_allowed_time_interval_between_pulses = time; }		

void WaterMeter::setNominalAndMaximumAllowedTimeIntervalsBetweenPulses( )
{
	byte flow_alarm = 0;

	// Given the Water Meter ratio (Liters/Pulse) and given the nominal water flow rate we can estimate 
	// the frequency at which pulses must arrive and thus supervise the proper functioning of the system

	// Example:
	// Ratio = 10 Liters per Pulse
	// Nominal flow rate through this Water Meter = 20 Liters per Second
	// Expected number of pulses = 20 Liters per Second / 10 Liters per Pulse = 2 Pulses per Second
	// Expected number of seconds between pulses = 10 Liters per Pulse / 20 Liters per Second = 0.5 Seconds
	// Expected time interval between pulses = 0.5 Seconds * 25 / 100 = 0.625 seconds

	// Based on the nominal flow rate calculate nominal (expected) time interval between pulses
	if ( nominal_flow_rate_liters_per_second != 0 )
	{
		nominal_time_interval_between_pulses = ( ratio / nominal_flow_rate_liters_per_second ) * MILLISECONDS_IN_SECOND; // in milliseconds

		// Note: Currently, the flow alarm is configured in the main line. However, the Water Meter can be installed on another 
		// system's component. Hence, the applicability of calculating the maximum allowed time interval beltween pulses
		// based solely on the main line flow alarm configuration is questionable !!!
		
		// Check to see if the Water Meter is sitting on the Main Line
		if ( main_line_id != 0 )
		{
			if ( DB_GetField(DBMAINLINE_TYPE, task_id, main_line_id, DBMAINLINE_FLOW_ALARM, &flow_alarm) )
			{
				if ( flow_alarm != 0 )
					max_allowed_time_interval_between_pulses = nominal_time_interval_between_pulses + ( nominal_time_interval_between_pulses * flow_alarm / 100 );
				else
					max_allowed_time_interval_between_pulses = nominal_time_interval_between_pulses;
			}
			else
			{
				WaterMeter::error_code = DB_GetLastErrorCode( task_id );
			}
		}
	}
	else
	{
		nominal_time_interval_between_pulses = 2147483647;
		max_allowed_time_interval_between_pulses = 2147483647;
	}
}

void WaterMeter::updateMeasurements( )
{
	float measured_water_quantity_liters;
	long current_time, elapsed_time_since_last_pulse;

	// Check to see if new pulses have been registered
	// Note: If the number of pulses is 0, then the current measured water flow is 0 as well
	if ( interval_pulses )
	{
		// Calculate measured quantity of water (Liters)
		// Multiply the ratio by the number of pulses to get the water flow - quantity of water delivered during the measured time interval
		// Quantity of Irrigation Water = Ratio (Liters/Pulse) * Number of Water Meter Pulses (Pulses)
		measured_water_quantity_liters = ratio * interval_pulses;

		// Calculate the measured time interval during which the pulses were accumulated (milliseconds)
		
		// TO DO: Consult with the management with respect to the following point:
		// I believe it is better to use the last_pulse_timestamp rather than the current_time because if 
		// the current_time was used instead it would 'steal' time from the subsequent pulse 
		
		measured_time_interval_between_pulses = last_pulse_timestamp - first_pulse_timestamp;

		// Calculate measured water flow rate during the measured time interval (Liters/Second)
		if ( measured_time_interval_between_pulses > 0 )
		{
			measured_flow_rate_liters_per_second = ( measured_water_quantity_liters / measured_time_interval_between_pulses ) * MILLISECONDS_IN_SECOND;
		}
		else 
		{
			measured_flow_rate_liters_per_second = 2147483647; // max = 2147483647; 
		}

		// Reset pulses
		interval_pulses = 0;
	}
	else	// No new pulses have been registered since the last invocation of this function 
	{
		// Check to see if there have NEVER been any pulses registered at all
		// Note: When the Irrigation just started, there could NOT be any pulses registered right away
		if ( ( first_pulse_timestamp == 0 ) && ( last_pulse_timestamp == 0 ) )
		{
			first_pulse_timestamp = get_current_time(IN_MILLISECONDS);
			last_pulse_timestamp = first_pulse_timestamp;
			elapsed_time_since_last_pulse = 0;
		}

		// Check to see if the nominal time interval during which a pulse was expected to be registered has expired
		
		// TO DO: 
		// 1. Consult with the management if the nominal_time_interval_between_pulses should in fact be max_allowed_time_interval_between_pulses?
		// 2. Which of the below options to adopt?
		// 3. When to activate the alarm?
		if ( time_interval_expired_in_milliseconds( last_pulse_timestamp, nominal_time_interval_between_pulses ) )
		{
			current_time = get_current_time(IN_MILLISECONDS);
			elapsed_time_since_last_pulse = current_time - last_pulse_timestamp;

			// OPTION 1: 
			// Reduce the measured flow by half
			measured_flow_rate_liters_per_second /= 2;

			// OPTION 2:
			// Check to see if the number of the maximum allowed missed pulses from the Water Meter has been reached
			if ( ++missed_pulses_counter >= MAX_WATER_METER_MISSED_PULSES )
			{
				// Reset the missed pulse counter
				missed_pulses_counter = 0;

				// Recalculate the measured flow rate
				// The Water Meter Ratio value [Liters/Pulse] is a fixed parameter of the Water Meter. 
				// If the pulse from the Water Meter was not acquired within the expected time interval (nominal_time_interval_between_pulses) 
				// then the elapsed_time_since_last_pulse is treated as the time it would take a pulse to arrive.
				if ( elapsed_time_since_last_pulse != 0 )
					measured_flow_rate_liters_per_second = ratio / ( elapsed_time_since_last_pulse / MILLISECONDS_IN_SECOND );
			}
		}

		// Update the measured time interval with the time during which the pulses were NOT accumulated (Milliseconds)
		measured_time_interval_between_pulses = elapsed_time_since_last_pulse;

		//// Check to see if the maximum allowed time interval has expired 
		//if ( time_interval_expired_in_milliseconds( first_pulse_timestamp, max_allowed_time_interval_between_pulses ) )
		//{
		//	// Reduce the measured flow by half
		//	measured_flow_rate_liters_per_second /= measured_flow_rate_liters_per_second;

		//	// Activate the alarm !!!
		//}

	}

	// Calculate measured flow rate in cubic meters per hour
	// measured_flow_rate_cubic_meters_per_hour = ( measured_flow_rate_liters_per_second * 18 ) / 5;


	// AT THIS POINT WE KNOW HOW FREQUENTLY THE PULSES SHOULD ARRIVE

	if ( ( first_pulse_timestamp != 0 ) && ( last_pulse_timestamp != 0 ) )
	{
		// Compare between the measured and the nominal time intervals between the pulses
		if ( measured_time_interval_between_pulses > nominal_time_interval_between_pulses )
		{
			// Insufficient flow
		}
		else if ( measured_time_interval_between_pulses < nominal_time_interval_between_pulses )
		{
			// Overflow
		}

		// Compare between the measured and the nominal flow rates
		if ( measured_flow_rate_liters_per_second < nominal_flow_rate_liters_per_second )
		{
			// Insufficient flow
		}
		else if ( measured_flow_rate_liters_per_second > nominal_flow_rate_liters_per_second )
		{
			// Overflow
		}
	}
}


// Returns the number of pulses counted. If DB update failed then "-1" is returned
int WaterMeter::retrieveIntervalPulses( ) const
{
	int interval_pulses = 0, zero = 0;

	// The difference between the DB_GetField and the DB_GetFieldForUpdate functions is that when the DB_GetFieldForUpdate is used
	// the subsequent update of the field in the database will generate the RECORD_CHANGED_SINCE_LAST_READ error if the respective
	// field was updated by another task between the calls to DB_GetFieldForUpdate and DB_UpdateField
	DB_GetFieldForUpdate(DBWATERMETER_TYPE, task_id, water_meter_id, DBWATERMETER_INTERVAL_PULSES, &interval_pulses);

	// Check to see if the record update has NOT been successfull
	while (!DB_UpdateField(DBWATERMETER_TYPE, task_id, water_meter_id, DBWATERMETER_INTERVAL_PULSES, &zero) )
	{
		// Check to see if the reason that the record update has NOT been successfull was NOT the RECORD_CHANGED_SINCE_LAST_READ
		if ( DB_GetLastErrorCode(task_id) != RECORD_CHANGED_SINCE_LAST_READ )
		{
			interval_pulses = -1;
			break;
		}

		DB_GetFieldForUpdate(DBWATERMETER_TYPE, task_id, water_meter_id, DBWATERMETER_INTERVAL_PULSES, &interval_pulses);
	}

	return interval_pulses;
}

//void WaterMeter::setWaterMeter( const WaterMeter & water_meter )
//{
//	// Perform aliasing test
//	if ( this != &water_meter )    
//	{
//		setTaskId( water_meter.task_id );
//		setWaterMeterId( water_meter.water_meter_id );
//		setInputId( water_meter.input_id );
//		setRelayLocation( water_meter.input_location );
//		setAllocateeType( water_meter.allocatee_type );
//		setAllocateeId( water_meter.allocatee_id );
//		setMainLineId( water_meter.main_line_id );
//		setRatio( water_meter.ratio );
//		setIntervalPulses( water_meter.interval_pulses );
//		setTotalPulses( water_meter.total_pulses );
//		setFirstPulseTimestamp( water_meter.first_pulse_timestamp );	
//		setLastPulseTimestamp( water_meter.last_pulse_timestamp );
//		setWaterLeakQuantity( water_meter.water_leak_quantity );
//		setWaterLeakPeriod( water_meter.water_leak_period );
//		setState( water_meter.state );
//
//		setMissedPulsesCounter( water_meter.missed_pulses_counter );
//		setNominalFlowRateLitersPerSecond( water_meter.nominal_flow_rate_liters_per_second );
//		setMeasuredFlowRateLitersPerSecond( water_meter.measured_flow_rate_liters_per_second );
//		// setElapsedTimeSinceLastPulse( water_meter.elapsed_time_since_last_pulse );
//		setNominalTimeIntervalBetweenPulses( water_meter.nominal_time_interval_between_pulses );
//		setMeasuredTimeIntervalBetweenPulses( water_meter.measured_time_interval_between_pulses );
//		setMaxAllowedTimeIntervalBetweenPulses( water_meter.max_allowed_time_interval_between_pulses );
//		setNominalAndMaximumAllowedTimeIntervalsBetweenPulses( );
//	}
//}