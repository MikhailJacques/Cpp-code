
// ValveGroup class implementation file

#include "ValveGroup.h"

#ifdef IRRIGATION_PROCESS_DEBUG
   #define DebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId) SendDebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId);
#else
   #define DebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId) /**/;
#endif

static DBTimePP time_zero(0,0,0);

/////////////////////////////////////////////////////////////////////////////////////////////
//  PUBLIC FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
ValveGroup::ValveGroup( ) { }

ValveGroup::~ValveGroup( ) 
{ 
	removeClosedValves( );
}


/////////////////////////////////////////////////////////////
// ACCESSOR FUNCTIONS
/////////////////////////////////////////////////////////////
DBIndex ValveGroup::getTaskId( ) const { return task_id; }
int ValveGroup::getGroupNumber( ) const { return group_number; }
int ValveGroup::getNumberGroups( ) const { return number_of_groups; }
int ValveGroup::getNumberValves( ) const { return valves.size(); }
DBIndex ValveGroup::getDosingStationId( ) const { return dosing_station_id; }
IrrigationType ValveGroup::getIrrigationType( ) const { return irrigation_type; }
float ValveGroup::getIrrigationArea( ) const { return irrigation_area; }
long ValveGroup::getStartTime( ) const { return start_time; }
long ValveGroup::getEndTime( ) const { return end_time; }
long ValveGroup::getTimeStamp( ) const { return time_stamp; }
bool ValveGroup::getState( ) const { return state; }
const ValveGroup & ValveGroup::getGroup( ) const { return *this; }
// IFertigationRecipe * ValveGroup::getFertigationRecipe( ) { return recipe; }

// IRRIGATION BY (DEPTH, QUANTITY, DURATION)
// IIrrigationSetLeft ValveGroup::getIrrigationSet( ) const { return set; }
// IIrrigationSetLeft ValveGroup::getIrrigationLeft( ) const { return left; }
IIrrigationSetLeft ValveGroup::getNominalIrrigationLeft( ) const { return nominal_left; }
IIrrigationSetLeft ValveGroup::getMeasuredIrrigationLeft( ) const { return measured_left; }

// IRRIGATION BY DEPTH
float ValveGroup::getDepthSet( ) const { return depth_set; }
float ValveGroup::getNominalDepthLeft( ) const { return nominal_depth_left; }
float ValveGroup::getMeasuredDepthLeft( ) const { return measured_depth_left; }

// IRRIGATION BY QUANTITY
float ValveGroup::getQuantitySet( ) const { return quantity_set; }
float ValveGroup::getNominalQuantityLeft( ) const { return nominal_quantity_left; }
float ValveGroup::getMeasuredQuantityLeft( ) const { return measured_quantity_left; }

// IRRIGATION BY DURATION
long ValveGroup::getDurationSet( ) const { return duration_set; }						
long ValveGroup::getNominalDurationLeft( ) const { return nominal_duration_left; }		
long ValveGroup::getMeasuredDurationLeft( ) const { return measured_duration_left; }
		
// WATER FLOW
// TO DO: Decide when and under which circumstances to return either nominal or measured flow rate to return !!!
float ValveGroup::getFlowRateLitersPerSecond( ) const { return nominal_flow_rate_liters_per_second; }
float ValveGroup::getFlowRateCubicMetersPerHour( ) const { return nominal_flow_rate_cubic_meters_per_hour; }

float ValveGroup::getNominalFlowRateLitersPerSecond( ) const { return nominal_flow_rate_liters_per_second; }
float ValveGroup::getMeasuredFlowRateLitersPerSecond( ) const { return measured_flow_rate_liters_per_second; }

float ValveGroup::getNominalFlowRateCubicMetersPerHour( ) const { return nominal_flow_rate_cubic_meters_per_hour; }
float ValveGroup::getMeasuredFlowRateCubicMetersPerHour( ) const { return measured_flow_rate_cubic_meters_per_hour; }

/////////////////////////////////////////////////////////////
// MUTATOR FUNCTIONS
/////////////////////////////////////////////////////////////
void ValveGroup::setTaskId( DBTask id ) { task_id = id; }
void ValveGroup::setGroupNumber( int number ) { group_number = number; }
void ValveGroup::setNumberGroups( int number ) { number_of_groups = number; }
void ValveGroup::setValves( const fixed_vector<IrrigationValve, MAX_VALVES_PER_GROUP, false> & valves ) { this->valves = valves; }
void ValveGroup::setDosingStationId( DBIndex id ) { dosing_station_id = id; }
void ValveGroup::setIrrigationType( IrrigationType type ) { irrigation_type = type; }
void ValveGroup::setIrrigationArea( float area ) { irrigation_area = area; }
void ValveGroup::setStartTime( long time ) { start_time = time; }
void ValveGroup::setEndTime( long time ) { end_time = time; }
void ValveGroup::setTimeStamp( long time ) { time_stamp = time; }
void ValveGroup::setState( bool state ) { this->state = state; }

// IRRIGATION BY (DEPTH, QUANTITY, DURATION)
void ValveGroup::setIrrigationSet( IIrrigationSetLeft set ) 
{ 
	depth_set = set.depth;
	quantity_set = set.quantity;
	duration_set = ( set.duration.hour * 3600000 + set.duration.minute * 60000 + set.duration.second * MILLISECONDS_IN_SECOND );
}

//void ValveGroup::setIrrigationLeft( IIrrigationSetLeft left ) 
//{ 
//	nominal_depth_left = left.depth;
//	nominal_quantity_left = left.quantity;
//	nominal_duration_left = ( left.duration.hour * 3600000 + left.duration.minute * 60000 + left.duration.second * MILLISECONDS_IN_SECOND );
//
//	measured_depth_left = nominal_depth_left;
//	measured_quantity_left = nominal_quantity_left;
//	measured_duration_left = nominal_duration_left;
//}

// void ValveGroup::setNominalIrrigationDelivered( IIrrigationSetLeft delivered ) { nominal_delivered = delivered; }
// void ValveGroup::setMeasuredIrrigationDelivered( IIrrigationSetLeft delivered ) { measured_delivered = delivered; }
// void ValveGroup::setNominalIrrigationLeft( IIrrigationSetLeft left ) { nominal_left = left; }
// void ValveGroup::setMeasuredIrrigationLeft( IIrrigationSetLeft left ) { measured_left = left; }

// IRRIGATION BY DEPTH
void ValveGroup::setDepthSet( float set ) { depth_set = set; }
void ValveGroup::setNominalDepthLeft( float left ) { nominal_depth_left = left; }
void ValveGroup::setMeasuredDepthLeft( float left ) { measured_depth_left = left; }

// IRRIGATION BY QUANTITY
void ValveGroup::setQuantitySet( float set ) { quantity_set = set; }
void ValveGroup::setNominalQuantityLeft( float left ) { nominal_quantity_left = left; }
void ValveGroup::setMeasuredQuantityLeft( float left ) { measured_quantity_left = left; }

// IRRIGATION BY DURATION
void ValveGroup::setDurationSet( long set ) { duration_set = set; }
void ValveGroup::setNominalDurationLeft( long left ) { nominal_duration_left = left; }
void ValveGroup::setMeasuredDurationLeft( long left ) { measured_duration_left = left; }

IIrrigationSetLeft ValveGroup::updateIrrigationLeft( long elapsed_time )
{
	switch ( irrigation_type )
	{
		case IRRIGATION_BY_DEPTH:		// Measured in mm

			updateNominalDepthLeft( elapsed_time );
			
			if ( water_meters.size( ) )
				updateMeasuredDepthLeft( elapsed_time );
			
			break;

		case IRRIGATION_BY_QUANTITY:	// Measured in m3
			
			updateNominalQuantityLeft( elapsed_time );

			if ( water_meters.size( ) )
				updateMeasuredQuantityLeft( elapsed_time );
			
			break;
		
		case IRRIGATION_BY_DURATION:	// Measured in milliseconds (Old format: hr:mm:ss)
			
			updateNominalDurationLeft( elapsed_time );
			
			if ( water_meters.size( ) )
				updateMeasuredDurationLeft( elapsed_time );
			
			break;

		default:	// none

			break;

	} // end switch ( irrigation_type )


	// TO DO: Calculate actual flow (prior to next pulse aquiry)

	// Compare between the nominal and measured flows
	if ( nominal_flow_rate_liters_per_second < measured_flow_rate_liters_per_second )
	{
		// Insufficient
		bool insufficient = true;
	}
	else if ( nominal_flow_rate_liters_per_second > measured_flow_rate_liters_per_second  )
	{
		// Overflow
		bool overflow = true;
	}
	else // nominal_flow_rate_liters_per_second = measured_flow_rate_liters_per_second
	{
		bool perfect = true;
	}

	// Note: Until Water Meters start functioning the irrigation is done by nominal
	return nominal_left;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////

// UPDATES
void ValveGroup::updateNominalDepthLeft( long elapsed_time )
{ 
	// float quantity_delivered_in_liters = nominal_flow_rate_liters_per_second * elapsed_time.ToSeconds( );
	// depth is measured in mm; MILLISECONDS_IN_SECOND = 1000
	nominal_depth_delivered += ( ( ( nominal_flow_rate_liters_per_second * elapsed_time ) / MILLISECONDS_IN_SECOND ) ) / irrigation_area; 
	nominal_depth_left = depth_set - nominal_depth_delivered;

	if ( nominal_depth_left <= 0 )
	{
		nominal_depth_left = 0;
		nominal_quantity_left = 0;
		nominal_duration_left = 0;

		nominal_left.depth = 0;
		nominal_left.quantity = 0;
		nominal_left.duration = time_zero;
	}
	else
	{
		// Calculate and update nominal water quantity and nominal duration left based on the nominal depth left
		setNominalQuantityLeft( convertDepthIntoQuantity( nominal_depth_left ) );
		setNominalDurationLeft( convertQuantityIntoDuration( nominal_quantity_left, nominal_flow_rate_cubic_meters_per_hour ) );

		nominal_left.depth = nominal_depth_left;
		nominal_left.quantity = nominal_quantity_left;
		nominal_left.duration.second = ( nominal_duration_left / MILLISECONDS_IN_SECOND ) % 60;
		nominal_left.duration.minute = ( nominal_duration_left / 60000 ) % 60;
		nominal_left.duration.hour = ( nominal_duration_left / 3600000 ) % 24;
	}
}

void ValveGroup::updateMeasuredDepthLeft( long elapsed_time )
{ 
	updateMeasuredFlowRate( elapsed_time );

	// MILLISECONDS_IN_SECOND = 1000 - devide by 1000 because the elapsed time is measured in milliseconds
	measured_depth_delivered += ( ( measured_flow_rate_liters_per_second * elapsed_time ) / MILLISECONDS_IN_SECOND ) / irrigation_area;
	measured_depth_left = depth_set - measured_depth_delivered;

	if ( measured_depth_left <= 0 )
	{
		measured_depth_left = 0;
		measured_quantity_left = 0;
		measured_duration_left = 0;

		measured_left.depth = 0;
		measured_left.quantity = 0;
		measured_left.duration = time_zero;
	}
	else
	{
		// Calculate and update measured water quantity left and measured duration left based on the measured depth left
		setMeasuredQuantityLeft( convertDepthIntoQuantity( measured_depth_left ) );
		setMeasuredDurationLeft( convertQuantityIntoDuration( measured_quantity_left, nominal_flow_rate_cubic_meters_per_hour ) );

		measured_left.depth = measured_depth_left;
		measured_left.quantity = measured_quantity_left;
		measured_left.duration.second = ( measured_duration_left / MILLISECONDS_IN_SECOND ) % 60;
		measured_left.duration.minute = ( measured_duration_left / 60000 ) % 60;
		measured_left.duration.hour = ( measured_duration_left / 3600000 ) % 24;
	}
}

void ValveGroup::updateNominalQuantityLeft( long elapsed_time )
{
	nominal_quantity_delivered += ( ( nominal_flow_rate_liters_per_second * elapsed_time ) / MILLISECONDS_IN_SECOND ) / 1000; // in m3
	nominal_quantity_left = quantity_set - nominal_quantity_delivered;

	if ( nominal_quantity_left <= 0 )
	{
		nominal_depth_left = 0;
		nominal_quantity_left = 0;
		nominal_duration_left = 0;

		nominal_left.depth = 0;
		nominal_left.quantity = 0;
		nominal_left.duration = time_zero;
	}
	else
	{
		// Calculate and update nominal duration left and nominal depth left based on the nominal water quantity left
		setNominalDepthLeft( convertQuantityIntoDepth( nominal_quantity_left ) );
		setNominalDurationLeft( convertQuantityIntoDuration( nominal_quantity_left, nominal_flow_rate_cubic_meters_per_hour ) );

		nominal_left.depth = nominal_depth_left;
		nominal_left.quantity = nominal_quantity_left;
		nominal_left.duration.second = ( nominal_duration_left / MILLISECONDS_IN_SECOND ) % 60;
		nominal_left.duration.minute = ( nominal_duration_left / 60000 ) % 60;
		nominal_left.duration.hour = ( nominal_duration_left / 3600000 ) % 24;
	}
}

void ValveGroup::updateMeasuredQuantityLeft( long elapsed_time )
{
	updateMeasuredFlowRate( elapsed_time );

	// MILLISECONDS_IN_SECOND = 1000 - devide by 1000 because the elapsed time is measured in milliseconds
	measured_quantity_delivered += ( ( measured_flow_rate_liters_per_second * elapsed_time ) / MILLISECONDS_IN_SECOND ) / 1000; // in m3
	measured_quantity_left = quantity_set - measured_quantity_delivered;

	if ( measured_quantity_left <= 0 )
	{
		measured_depth_left = 0;
		measured_quantity_left = 0;
		measured_duration_left = 0;

		measured_left.depth = 0;
		measured_left.quantity = 0;
		measured_left.duration = time_zero;
	}
	else
	{
		// Calculate and update measured duration left and measured depth left based on the measured water quantity left
		setMeasuredDepthLeft( convertQuantityIntoDepth( measured_quantity_left ) );
		setMeasuredDurationLeft( convertQuantityIntoDuration( measured_quantity_left, nominal_flow_rate_cubic_meters_per_hour ) );

		measured_left.depth = measured_depth_left;
		measured_left.quantity = measured_quantity_left;
		measured_left.duration.second = ( measured_duration_left / MILLISECONDS_IN_SECOND ) % 60;
		measured_left.duration.minute = ( measured_duration_left / 60000 ) % 60;
		measured_left.duration.hour = ( measured_duration_left / 3600000 ) % 24;
	}
}

void ValveGroup::updateNominalDurationLeft( long elapsed_time ) 
{ 
	nominal_duration_elapsed += elapsed_time;
	nominal_duration_left = duration_set - nominal_duration_elapsed;

	if ( nominal_duration_left > 0 )
	{
		// Calculate and update nominal quantity left and nominal depth left based on the nominal duration left
		setNominalQuantityLeft( convertDurationIntoQuantity( nominal_duration_left, nominal_flow_rate_cubic_meters_per_hour ) );
		setNominalDepthLeft( convertQuantityIntoDepth( nominal_duration_left ) );

		// TO DO: Address the problem of rounding down error !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		nominal_left.depth = nominal_depth_left;
		nominal_left.quantity = nominal_quantity_left;
		nominal_left.duration.second = ( nominal_duration_left / MILLISECONDS_IN_SECOND ) % 60;
		nominal_left.duration.minute = ( nominal_duration_left / 60000 ) % 60;
		nominal_left.duration.hour = ( nominal_duration_left / 3600000 ) % 24;
	}
	else 
	{
		nominal_depth_left = 0;
		nominal_quantity_left = 0;
		nominal_duration_left = 0;

		nominal_left.duration.second = 0;
		nominal_left.duration.minute = 0;
		nominal_left.duration.hour = 0;
	}
}

void ValveGroup::updateMeasuredDurationLeft( long elapsed_time )
{ 
	updateMeasuredFlowRate( elapsed_time );
	measured_duration_elapsed += elapsed_time;
	measured_duration_left = duration_set - measured_duration_elapsed;

	if ( measured_duration_left > 0 )
	{
		// Calculate and update measured quantity left and measured depth left based on the measured duration left
		setMeasuredQuantityLeft( convertDurationIntoQuantity( measured_duration_left, nominal_flow_rate_cubic_meters_per_hour ) );
		setMeasuredDepthLeft( convertQuantityIntoDepth( measured_quantity_left ) );

		measured_left.depth = measured_depth_left;
		measured_left.quantity = measured_quantity_left;
		measured_left.duration.second = ( measured_duration_left / MILLISECONDS_IN_SECOND ) % 60;
		measured_left.duration.minute = ( measured_duration_left / 60000 ) % 60;
		measured_left.duration.hour = ( measured_duration_left / 3600000 ) % 24;
	}
	else 
	{	
		measured_depth_left = 0;
		measured_quantity_left = 0;
		measured_duration_left = 0;

		measured_left.duration.second = 0;
		measured_left.duration.minute = 0;
		measured_left.duration.hour = 0;
	}
}

// Assumption: This function is invoked no more frequently than once a second
float ValveGroup::updateMeasuredFlowRate( long elapsed_time )
{
	DBIndex water_meter_id;

	// CALCULATE AND RETURN ACTUAL FLOW
	// CURRENT SIMPLIFIED ALGORITHM FOR READING WATER METERS IS BASED ON THE FOLLOWING ASSUMPTION: 
	// Every Valve Group has ONLY one Water Meter installed in such a manner that its measurements should 
	// ideally approximate the calculated (in advance) nominal flow rate
	// In other words, the reportedly measured quantity of water delivered should ideally be equvivalent to 
	// the calculated nominal quantity of water that must be delivered
	// The algorithm uses measured water flow rate if Water Meter is installed and uses calculated nominal water flow rate 
	// if Water Meter is not installed.
	
	// Step over all Water Meters in the Valve Group
	// Each Water Meter object has all the necessary information to compare between its nominal and measured flow rates
	for ( water_meters_itr = water_meters.begin(); water_meters_itr != water_meters.end(); ++water_meters_itr )
	{
		// water_meters_itr->updateMeasurements( );
		water_meter_id = water_meters_itr->first;
		water_meters[water_meter_id].updateMeasurements( );

		// The following properties are now available:
		// water_meters[water_meter_id].getRatio( );
		// water_meters[water_meter_id].getPulses( );
		// water_meters[water_meter_id].getPulseCounter( );
		// water_meters[water_meter_id].getFirstPulseTimestamp( );					
		// water_meters[water_meter_id].getLastPulseTimestamp( );
		// water_meters[water_meter_id].getNominalFlowRateLitersPerSecond( );
		// water_meters[water_meter_id].getNominalTimeIntervalBetweenPulses( );
		// water_meters[water_meter_id].getMaxAllowedTimeIntervalBetweenPulses( );
		// water_meters[water_meter_id].getWaterLeakQuantity( );
		// water_meters[water_meter_id].getWaterLeakPeriod( );
		// water_meters[water_meter_id].getState( );
	}

	// TO DO: 
	// ( Water Meter Ratio * # Pulses ) / ( Time * 3600 )
	// As the pulses do NOT arrive, the time interval between the pulses grows bigger and consequently the flow rate gets progressively smaller
	// At each missing arrival of the pulse, increase the time respectively and re-calculate the measured flow rate

	// TO DO: Build a timing control that monitors pulse frequency
	// TO DO: Resolve deciding when to update the irrigation left by nominal and when by measured !!!
	// TO DO: Develop algorithm for overseeing and supervising the measured flow rate based on the nominal flow rate !!!
}

void ValveGroup::setNominalAndMeasuredLeft( )
{
	nominal_left.depth = depth_set;
	nominal_left.quantity = quantity_set;
	nominal_left.duration.second = ( duration_set / MILLISECONDS_IN_SECOND ) % 60;
	nominal_left.duration.minute = ( duration_set / 60000 ) % 60;
	nominal_left.duration.hour = ( duration_set / 3600000 ) % 24;

	measured_left.depth = nominal_left.depth;
	measured_left.quantity = nominal_left.quantity;
	measured_left.duration.second = nominal_left.duration.second;
	measured_left.duration.minute = nominal_left.duration.minute;
	measured_left.duration.hour = nominal_left.duration.hour;
}

// CONVERSIONS

void ValveGroup::convertIrrigationAreaIntoSquareMeters( )
{
	IrrigationAreaUnits units = valves[0].getIrrigationAreaUnits( );

	// Convert Acres into Hectares (if necessary)
	if ( units == ACRE )
		irrigation_area *= 0.404686; 

	// Convert Hectares into Square Meters
	irrigation_area *= SQUARE_METERS_IN_HECTARE;
}

void ValveGroup::convertFlowRateCubicMetersPerHourIntoLitersPerSecond( )
{
	nominal_flow_rate_liters_per_second = ( nominal_flow_rate_cubic_meters_per_hour * 5 ) / 18;
}

float ValveGroup::convertDepthIntoQuantity( float depth )
{
	// Water Quantity (m3) = Irrigation Area (m2) * Depth (mm)
	return irrigation_area * ( depth / MILLIMETERS_IN_METER );
}

float ValveGroup::convertQuantityIntoDepth( float quantity )
{
	// Depth (mm) = Water Quantity (m3) / Irrigation Area (m2)
	float depth;

	if ( irrigation_area != 0 )
		depth = ( quantity / irrigation_area ) * MILLIMETERS_IN_METER;
	else
		depth = 0;

	return depth;
}

long ValveGroup::convertQuantityIntoDuration( float quantity, float flow )
{
	// Irrigation Duration (hr) = Water Quantity (m3) / Flow (m3/hr)
	//  Irrigation Duration (milliseconds) = Water Quantity (m3) / Flow (m3/hr) * 3600000;
	long duration;

	if ( flow != 0 )
		duration = ( ( quantity / flow ) * SECONDS_IN_HOUR ) * MILLISECONDS_IN_SECOND;
	else
		duration = 2147483647;

	return duration;
}

float ValveGroup::convertDurationIntoQuantity( long duration, float flow )
{
	// Water Quantity (m3) = Water Duration (sec) * Flow (m3/sec)
	// Note: The following order of precedence of arithmetic operations is important as it ensures the division by 0 does NOT take place
	return ( ( duration * flow ) / SECONDS_IN_HOUR ) / MILLISECONDS_IN_SECOND;
}

void ValveGroup::addMainValveToTheCurrentValveGroup( DBIndex main_valve_id )
{
	// Ensure that the same Main Valve is NOT added more than once
	if ( !doesMainValveExistInTheCurrentValveGroup( main_valve_id ) )
		main_valves.push_back( MainValve( task_id, main_valve_id ) );
}

void ValveGroup::addMainValveToTheNextValveGroup( DBIndex main_valve_id )
{
	if ( !doesMainValveExistInTheNextValveGroup( main_valve_id ) )
		main_valve_ids.push_back( main_valve_id );
}

void ValveGroup::addIrrigationValveToTheCurrentValveGroup( DBIndex valve_id )
{
	// TO DO: Consider changing valves from vector to set, 
	// thus effectively eliminating the need for the doesIrrigationValveExistInTheCurrentValveGroup function !!!
	// Ensure that the same Irrigation Valve is NOT added more than once
	// Retrieve the respective Irrigation Valve from the Database, construct an Irrigation Valve object
	// and add the Irrigation Valve to the active Valve Group
	if ( !doesIrrigationValveExistInTheCurrentValveGroup( valve_id ) )
		valves.push_back( IrrigationValve( task_id, valve_id ) );
}

void ValveGroup::addIrrigationValveToTheNextValveGroup( DBIndex valve_id )
{
	if ( !doesIrrigationValveExistInTheNextValveGroup( valve_id ) )
		valve_ids.push_back( valve_id );
}

void ValveGroup::addWaterMeter( const WaterMeter & water_meter ) 
{ 
	DBIndex id = water_meter.getWaterMeterId( );
	// water_meters.insert( id - 1, water_meter );
	water_meters[id - 1] = water_meter;		
}

// Note: The message is expected to be generated as soon as the Water Meter generates a pulse
void ValveGroup::processWaterMeterMessage( const DB_LISTENER_MESSAGE_P db_message ) 
{ 
	int interval_pulses;
	long first_pulse_timestamp, last_pulse_timestamp;

	DBIndex water_meter_id = db_message->index;

	// Read the necessary properties from the appropriate Database record

	// TO DO: 
	// There is a need to lock the Database field because a new measurement might arrive from another thread between 
	// the time the pulses field value was retreived and the resetting of that field by the current thread
	// The difference between the DB_GetField and the DB_GetFieldForUpdate is that when using the DB_GetFieldForUpdate function
	// subsequent update of the field in the database will produce the RECORD_CHANGED_SINCE_LAST_READ error if the field was updated 
	// by another task between the calls to DB_GetFieldForUpdate and DB_UpdateField

	DB_GetField( DBWATERMETER_TYPE, task_id, water_meter_id, DBWATERMETER_INTERVAL_PULSES, &interval_pulses );
	DB_GetField( DBWATERMETER_TYPE, task_id, water_meter_id, DBWATERMETER_FIRST_PULSE_TIMESTAMP, &first_pulse_timestamp );	
	DB_GetField( DBWATERMETER_TYPE, task_id, water_meter_id, DBWATERMETER_LAST_PULSE_TIMESTAMP, &last_pulse_timestamp );

	// Reset the relevant properties in the respective Database record
	// Specifically, reset the number of pulses counted and reset the timestamp each time the pulses are read.
	// That is, start counting the time until the next read operation from the moment of the current read operation
	interval_pulses = 0;
	DB_UpdateField( DBWATERMETER_TYPE, task_id, water_meter_id, DBWATERMETER_INTERVAL_PULSES, &interval_pulses );
	DB_UpdateField( DBWATERMETER_TYPE, task_id, water_meter_id, DBWATERMETER_LAST_PULSE_TIMESTAMP, &first_pulse_timestamp );

	// Save the properties in the respective Water Meter object
	water_meters[water_meter_id].addIntervalPulses( interval_pulses );
	water_meters[water_meter_id].setFirstPulseTimestamp( first_pulse_timestamp );
	water_meters[water_meter_id].setLastPulseTimestamp( last_pulse_timestamp );

	DebugOutputRequest( 5, SEQUENCE_WATER_METER_MESSAGE, task_id, water_meter_id );
}


// QUERRIES

bool ValveGroup::doesMainValveExistInTheCurrentValveGroup( DBIndex main_valve_id )
{
	bool exists = false;

	// Valid Main Valve Id ranges from 1 to 10
	for ( main_valve_itr = main_valves.begin(); main_valve_itr != main_valves.end() ; ++main_valve_itr )
	{
		if ( main_valve_itr->getValveId( ) == main_valve_id )
		{
			exists = true;
			break;
		}
	}

	return exists;
}

bool ValveGroup::doesMainValveExistInTheNextValveGroup( DBIndex main_valve_id )
{
	bool exists = false;

	for ( main_valve_ids_itr = main_valve_ids.begin(); main_valve_ids_itr != main_valve_ids.end() ; ++main_valve_ids_itr )
	{
		if ( *main_valve_ids_itr == main_valve_id )
		{
			exists = true;
			break;
		}
	}

	return exists;
}


bool ValveGroup::doesIrrigationValveExistInTheCurrentValveGroup( DBIndex valve_id )
{
	bool exists = false;

	// Valid Irrigation Valve Id ranges from 1 to 255
	for ( valve_itr = valves.begin(); valve_itr != valves.end() ; ++valve_itr )
	{
		if ( valve_itr->getValveId( ) == valve_id )
		{
			exists = true;
			break;
		}
	}

	return exists;
}

bool ValveGroup::doesIrrigationValveExistInTheNextValveGroup( DBIndex valve_id )
{
	bool exists = false;

	for ( valve_ids_itr = valve_ids.begin(); valve_ids_itr != valve_ids.end() ; ++valve_ids_itr )
	{
		if ( *valve_ids_itr == valve_id )
		{
			exists = true;
			break;
		}
	}

	return exists;
}

void ValveGroup::setOpenedIrrigationValvesCounter( int counter ) { opened_irrigation_valves_counter = counter; }
void ValveGroup::setOpenedMainValvesCounter( int counter ) { opened_main_valves_counter = counter; }
void ValveGroup::setNominalFlowRateCubicMetersPerHour( float nominal_flow ) { nominal_flow_rate_cubic_meters_per_hour = nominal_flow; }
void ValveGroup::setNominalFlowLitersPerSecond( float nominal_flow ) { nominal_flow_rate_liters_per_second  = nominal_flow; }

void ValveGroup::initIrrigationSetLeftZeroAndTimeZero( )
{
	IIrrigationSetLeft temp;
	temp.depth = 0;
	temp.quantity = 0;
	temp.duration = time_zero;
	set_left_zero = temp;
}


// PUBLIC FUNCTIONS

void ValveGroup::initGroup( DBTask task_id, int number_of_groups, IrrigationType irrigation_type, DBIndex dosing_station_id )
{
	setTaskId( task_id );
	setGroupNumber( 1 );
	setNumberGroups( number_of_groups );
	setDosingStationId( dosing_station_id );
	valves.erase( valves.begin(), valves.end() );
	valve_ids.erase( valve_ids.begin(), valve_ids.end() );
	main_valves.erase( main_valves.begin(), main_valves.end() );
	main_valve_ids.erase( main_valve_ids.begin(), main_valve_ids.end() );
	water_meters.erase( water_meters.begin(), water_meters.end() );
	
	irrigation_area = 0;
	nominal_flow_rate_liters_per_second = 0;
	nominal_flow_rate_cubic_meters_per_hour = 0;
	setIrrigationType( irrigation_type );
	initIrrigationSetLeftZeroAndTimeZero( );

	// IRRIGATION BY (DEPTH, QUANTITY, DURATION)
	setIrrigationSet( set_left_zero );
	// setIrrigationLeft( set_left_zero );

	depth_set = 0;
	nominal_depth_delivered = 0;
	measured_depth_delivered = 0;
	nominal_depth_left = 0;
	measured_depth_left = 0;

	quantity_set = 0;
	nominal_quantity_delivered = 0;
	measured_quantity_delivered = 0;
	nominal_quantity_left = 0;
	measured_quantity_left = 0;

	duration_set = 0;
	nominal_duration_elapsed = 0;
	measured_duration_elapsed = 0;
	nominal_duration_left = 0;
	measured_duration_left = 0;

	start_time = 0;
	end_time = 0;
	time_stamp = 0;
	water_meter_time_stamp = 0;

	opened_main_valves_counter = 0;
	opened_irrigation_valves_counter = 0; 
	nominal_flow_rate_liters_per_second = 0;
	nominal_flow_rate_cubic_meters_per_hour = 0;

	setState( false );
}

void ValveGroup::resetGroup( int group_number )
{
	setGroupNumber( group_number );

	removeClosedValves( );
	valve_ids.erase( valve_ids.begin(), valve_ids.end() );
	main_valve_ids.erase( main_valve_ids.begin(), main_valve_ids.end() );
	water_meters.erase( water_meters.begin(), water_meters.end() );
	
	irrigation_area = 0;
	nominal_flow_rate_liters_per_second = 0;
	nominal_flow_rate_cubic_meters_per_hour = 0;
	

	// IRRIGATION BY (DEPTH, QUANTITY, DURATION)
	setIrrigationSet( set_left_zero );

	depth_set = 0;
	nominal_depth_delivered = 0;
	measured_depth_delivered = 0;
	nominal_depth_left = 0;
	measured_depth_left = 0;

	quantity_set = 0;
	nominal_quantity_delivered = 0;
	measured_quantity_delivered = 0;
	nominal_quantity_left = 0;
	measured_quantity_left = 0;

	duration_set = 0;
	nominal_duration_elapsed = 0;
	measured_duration_elapsed = 0;
	nominal_duration_left = 0;
	measured_duration_left = 0;

	start_time = 0;
	end_time = 0;
	time_stamp = 0;
	water_meter_time_stamp = 0;

	setState( false );
}

// Updates Total Irrigation Area, Total Nominal Flow, registers with Water Meters and applies Delays
void ValveGroup::updateGroup( )
{
	// long stagger_delay( time_zero );
	// DBWaterMeter db_water_meter_record;

	DBIndex irrigation_valve_id = 0, main_line_id = 0, dosing_station_id = 0, filter_station_id = 0, pump_station_id = 0, water_source_id = 0;

	DBIndex water_meter_id = 0, irrigation_valve_water_meter_id = 0, main_line_water_meter_id = 0, dosing_station_water_meter_id = 0, 
		filter_station_water_meter_id = 0, pump_station_water_meter_id = 0, water_source_water_meter_id = 0;
		 
	// Sort the Main Valves in the Valve Group in the ascending order
	sort( main_valves.begin( ), main_valves.end( ) ); 

	// Sort the Irrigation Valves in the Valve Group in the ascending order
	sort( valves.begin( ), valves.end( ) ); 

	// Step over each Irrigation Valve in the Valve Group
	for ( valve_itr = valves.begin(); valve_itr != valves.end() ; ++valve_itr )
	{
		// Apply On and Off Delays.
		// Note: The valves do NOT delay one another. The delays start from Time 0 per cycle
		// valve_itr->setOpenDelay( valve_itr->getOnDelay( ) );
		// valve_itr->setCloseDelay( valve_itr->getOffDelay( ) );

		// APPLY ON DELAYS, OFF DELAYS AND CALCULATE AND APPLY STAGGER DELAYS wherever applicable

		// Commented out as a result of the 2nd change in client requirements:
		// On Delay - Check to see if the valve is NOT in the Head Group
		// if ( group_number != 1 )
		//	valve_itr->setOpenDelay( valve_itr->getOnDelay( ) );

		// Commented out as a result of the 2nd change in client requirements:
		// Off Delay - Check to see if the valve is NOT in the Tail Group
		// if ( group_number != number_of_groups )
		// 	valve_itr->setCloseDelay( valve_itr->getOffDelay( ) );
	
		// Commented out as a result of the 2nd change in client requirements:
		// Stagger Delay - Check to see if the valve is either in the Head Group or in the Tail Group
		// if ( group_number > 1 && group_number < number_of_groups )
		//	valve_itr->setStaggerDelay( time_zero );

		// Commented out as a result of the 1st change in client requirements:
		// First valve in the Head Group
		// if ( ( valve_itr == valves.begin() ) && ( group_number == 1 ) )
		//	valve_itr->setStaggerDelay( time_zero );

		// Commented out as a result of the 1st change in client requirements:
		// First valve in the Tail Group
		// if ( ( valve_itr == valves.begin() ) && ( group_number == number_of_groups ) )
		//	valve_itr->setStaggerDelay( time_zero );

		// Commented out as a result of the 2nd change in client requirements:
		// stagger_delay = stagger_delay + valve_itr->getStaggerDelay( );
			
		// Commented out as a result of the 2nd change in client requirements:
		// Check to see if the valve is in the Head Group
		// if ( group_number == 1 )
		//	valve_itr->setOpenDelay( stagger_delay );

		// Commented out as a result of the 2nd change in client requirements:
		// Check to see if the valve is in the Tail Group
		// if ( group_number == number_of_groups )
		//	valve_itr->setCloseDelay( stagger_delay );


		// UPDATE IRRIGATION VALVES' TOTAL IRRIGATION AREA AND TOTAL NOMINAL FLOW

		irrigation_area += valve_itr->getIrrigationArea( );
		nominal_flow_rate_cubic_meters_per_hour += valve_itr->getNominalFlowRateCubicMetersPerHour( );

		
		// PERFORM CONVERSIONS

		convertIrrigationAreaIntoSquareMeters( );
		convertFlowRateCubicMetersPerHourIntoLitersPerSecond( );


		// FIND OUT IF/WHICH WATER METERS ARE INSTALLED AND REGISTER TO LISTEN ON THEM
		
		// Get all Water Meters for this Valve Group
		// 
		// Case 1: Each Irrigation Valve has a Water Meter
		// Case 2: The Main Line has a Water Meter
		// Case 3: The Dosing Station has a Water Meter
		// Case 4: The Filter Station has a Water Meter
		// Case 5: The Pump Station has a Water Meter
		// Case 6: Each Water Source has a Water Meter
		// Case 7: Some components have Water Meters, while others do not
		// Case 8: None of the components have a Water Meter

		irrigation_valve_id = valve_itr->getValveId( );

		// Register ONLY with those Water Meters that are pertinent to the current active Valve Group
		// TO DO: Develop algorithm for registering only with those Water Meters that are pertinent
		// Current assumption: There is only one Water Meter installed in the system at the root of the hydraulic tree
		// Hence, its readings (measured flow rate) should approximate the nominal flow rate

		// The fertigation is done based on the measurements of the root Water Meter - 
		// TO DO: The closing of the Irrigation Valves is based on (Slide 7) ???
		// TO DO: Verify with the management the final locations of the Water Meters !!!
		// TO DO: Given the latest Netafim's requirement that Water Sources will vary from Valve Group to Valve Group,
		// the following algorithm will require certain modifications
		// if ( !water_source_water_meter )

		// Retrieve Main Line Id
		if ( DB_GetField(DBIRRIGATIONVALVE_TYPE, task_id, irrigation_valve_id, DBIRRIGATIONVALVE_LINE_ID, &main_line_id) )
		{
			// Retrieve Dosing Station Id
			if ( DB_GetField(DBMAINLINE_TYPE, task_id, main_line_id, DBMAINLINE_DOSING_STATION_ID, &dosing_station_id) )
			{
				// Retrieve Filter Station Id
				if ( DB_GetField(DBDOSINGSTATION_TYPE, task_id, dosing_station_id, DBDOSINGSTATION_FILTER_STATION_ID, &filter_station_id) )
				{
					// Retrieve Pump Station Id
					if ( DB_GetField(DBFILTERSTATION_TYPE, task_id, filter_station_id, DBFILTERSTATION_PUMP_STATION_ID, &pump_station_id) )
					{
						// Retrieve Water Source Id
						if ( DB_GetField(DBWATERSOURCE_TYPE, task_id, pump_station_id, DBFILTERSTATION_PUMP_STATION_ID, &water_source_id) )
						{
							// Retrieve Water Source Water Meter Id
							if ( DB_GetField(DBWATERSOURCE_TYPE, task_id, water_source_id, DBWATERSOURCE_WATER_METER_ID, &water_source_water_meter_id) )
							{
								// Check to see if the Water Source has a Water Meter installed and if yes then register to listen to it
								if ( water_source_water_meter_id != 0 )
								{
									; // do nothing
								}
							}
						}

						// Retrieve Pump Station Water Meter Id
						if ( DB_GetField(DBPUMPSTATION_TYPE, task_id, pump_station_id, DBPUMPSTATION_WATER_METER_ID, &pump_station_water_meter_id) )
						{
							// Check to see if the Pump Station has a Water Meter installed and if yes then register to listen to it
							if ( pump_station_water_meter_id != 0 )
							{
								; // do nothing
							}
						}
					} // End if - Retrieve Pump Station Id
				
					// Retrieve Filter Station Water Meter Id
					if ( DB_GetField(DBDOSINGSTATION_TYPE, task_id, dosing_station_id, DBDOSINGSTATION_WATER_METER_ID, &filter_station_water_meter_id) )
					{
						// Check to see if the Dosing Station has a Water Meter installed and if yes then register to listen to it
						if ( filter_station_water_meter_id != 0 )
						{
							; // do nothing
						}
					}

				} // End if - Retrieve Filter Station Id

				// Retrieve Dosing Station Water Meter Id
				if ( DB_GetField(DBDOSINGSTATION_TYPE, task_id, dosing_station_id, DBDOSINGSTATION_WATER_METER_ID, &dosing_station_water_meter_id) )
				{
					// Check to see if the Dosing Station has a Water Meter installed and if yes then register to listen to it
					if ( dosing_station_water_meter_id != 0 )
					{
						; // do nothing
					}
				}
			
			} // End if - Retrieve Dosing Station Id
		
			// Retrieve Main Line Water Meter Id
			if ( DB_GetField(DBMAINLINE_TYPE, task_id, main_line_id, DBMAINLINE_WATER_METER_ID, &main_line_water_meter_id) )
			{
				// Check to see if the Main Line has a Water Meter installed and if yes then register to listen to it
				if ( main_line_water_meter_id != 0 )
				{
					; // do nothing
				}
			}
		
		} // End if - Retrieve Main Line Id

		// Retrieve Irrigation Valve Water Meter Id
		irrigation_valve_water_meter_id = valve_itr->getWaterMeterId( );

	} // end for loop

	// Select ONLY one Water Meter for the Valve Group 
	// Algorithm: Selects the highest Water Meter in the hydraulic system hierarchy
	if ( water_source_water_meter_id )
	{
		water_meter_id = water_source_water_meter_id;
	}
	else if ( pump_station_water_meter_id )
	{
		water_meter_id = pump_station_water_meter_id;
	} 
	else if ( filter_station_water_meter_id )
	{
		water_meter_id = filter_station_water_meter_id;
	}
	else if ( dosing_station_water_meter_id )
	{
		water_meter_id = dosing_station_water_meter_id;
	}
	else if ( main_line_water_meter_id )
	{
		water_meter_id = main_line_water_meter_id;
	}
	else if ( irrigation_valve_water_meter_id )
	{
		water_meter_id = irrigation_valve_water_meter_id;
	}
	else
	{
		// No Water Meters have been installed
	}


	// CREATE, INITIALIZE AND ADD WATER METER TO THE VALVE GROUP AND REGISTER TO LISTEN TO IT

	if ( water_meter_id != 0 )
	{
		WaterMeter water_meter( task_id, water_meter_id );
		water_meter.setMainLineId( main_line_id );
		water_meter.setNominalFlowRateLitersPerSecond( nominal_flow_rate_liters_per_second );
		water_meter.setNominalAndMaximumAllowedTimeIntervalsBetweenPulses( );
		water_meter.setFirstPulseTimestamp( 0 );
		water_meter.setLastPulseTimestamp( 0 );
		addWaterMeter( water_meter );
	}
	

	// SET THE APPROPRIATE IRRIGATION BY DEPTH/QUANTITY/DURATION VALUE BASED ON THE USER-DEFINED IRRIGATION TYPE

	switch ( irrigation_type )
	{
		case IRRIGATION_BY_DEPTH:		// Measured in mm

			// Estimate and then set Irrigation Water Quantity set and left based on the Irrigation Depth
			setQuantitySet( convertDepthIntoQuantity( depth_set ) );
			setNominalQuantityLeft( quantity_set );
			
			// Estimate and then set Irrigation Water Duration set and left based on the Irrigation Quantity
			// Note: It might overflow since there are only 86,400 seconds in 24 hours !!!
			setDurationSet( convertQuantityIntoDuration( quantity_set, nominal_flow_rate_cubic_meters_per_hour ) );
			setNominalDurationLeft( duration_set );

			// Set the Nominal and Measured Depth/Quantity/Duration left to the Depth/Quantity/Duration set
			setNominalAndMeasuredLeft( );

			break;

		case IRRIGATION_BY_QUANTITY:	// Measured in m3

			// Estimate and then set Irrigation Depth set and left based on the Irrigation Water Quantity
			setDepthSet( convertQuantityIntoDepth( quantity_set ) );
			setNominalDepthLeft( depth_set );
			
			// Estimate and then set Irrigation Duration set and left based on the Irrigation Water Quantity
			// Note: It might overflow since there are only 86,400 seconds in 24 hours !!!
			setDurationSet( convertQuantityIntoDuration( quantity_set, nominal_flow_rate_cubic_meters_per_hour ) );
			setNominalDurationLeft( duration_set );

			// Set the Nominal and Measured Depth/Quantity/Duration left to the Depth/Quantity/Duration set
 			setNominalAndMeasuredLeft( );
			break;
		
		case IRRIGATION_BY_DURATION:	// Measured in hr:mm:ss

			// Estimate and then set Irrigation Water Quantity set and left based on the Irrigation Duration
			setQuantitySet( convertDurationIntoQuantity( duration_set, nominal_flow_rate_cubic_meters_per_hour ) );
			setNominalQuantityLeft( quantity_set );	
			
			// Estimate and then set Irrigation Depth set and left based on the Irrigation Water Quantity
			setDepthSet( convertQuantityIntoDepth( quantity_set ) );
			setNominalDepthLeft( depth_set );

			// Set the Nominal and Measured Depth/Quantity/Duration left to the Depth/Quantity/Duration set
			setNominalAndMeasuredLeft( );

			break;

		default:	// none
	} // end switch ( irrigation_type )
}

bool ValveGroup::startPumps( )
{
	bool operation_status = false;
	
	// TO DO: Develop Pump Manager which will have an algorithm for selecting the right number of Pumps and for starting them
	
	// Valve Sequence has Dosing Station Id - INDEX dosing_station_id;
	// Dosing Station has Pump Station Id - INDEX pump_station_id:
	// Currently Pump Station script does NOT include Pumps

	operation_status = true;
		
	return operation_status;
}

bool ValveGroup::stopPumps( )
{
	bool operation_status = false;

	// TO DO: Develop algorithm for stopping the Pumps

	operation_status = true;

	return operation_status;
}

bool ValveGroup::openMainValves( )
{
	bool operation_status;

	// STEP THROUGH THE MAIN VALVES IN THE GROUP AND OPEN ONLY THOSE MAIN VALVES THAT ARE NOT OPEN YET
	for ( main_valve_itr = main_valves.begin(); main_valve_itr != main_valves.end() ; ++main_valve_itr )
	{
		// Check to see if the Main Valve is closed
		DBIndex temp_id = main_valve_itr->getValveId( );
		if ( !main_valve_itr->getState( ) )
		{
			if ( main_valve_itr->openValve( ) )
			{
				opened_main_valves_counter++;
			}
		}
	}

	if ( opened_main_valves_counter == main_valves.size() )
		operation_status = true;
	else
		operation_status = false;

	return operation_status;
}

bool ValveGroup::closeMainValves( )
{
	bool operation_status;
	int main_valves_left_open = 0;

	// STEP THROUGH THE MAIN VALVES IN THE GROUP AND CLOSE ONLY THOSE MAIN VALVES THAT DO NOT APPEAR IN THE NEXT VALVE GROUP
	for ( main_valve_itr = main_valves.begin(); main_valve_itr != main_valves.end() ; ++main_valve_itr )
	{
		DBIndex temp_id = main_valve_itr->getValveId( );
		if ( doesMainValveExistInTheNextValveGroup( temp_id ) )
		{
			main_valves_left_open++;
		}
		else
		{
			// Check to see if the Main Valve is open
			if ( main_valve_itr->getState( ) )
			{
				if ( !main_valve_itr->closeValve( ) )
				{
					opened_main_valves_counter--;
				}
			}
		}
	}

	if ( ( opened_main_valves_counter - main_valves_left_open ) == 0 )
		operation_status = true;
	else
		operation_status = false;

	return operation_status;
}

bool ValveGroup::openIrrigationValves( )
{
	// STEP THROUGH THE IRRIGATION VALVES IN THE VALVE GROUP AND OPEN THEM ONE VALVE AT A TIME
	for ( valve_itr = valves.begin(); valve_itr != valves.end() ; ++valve_itr )
	{
		// Check to see if the Irrigation Valve is closed
		DBIndex temp_id = valve_itr->getValveId( );
		if ( !valve_itr->getState( ) )
		{
			if ( valve_itr->openValve( ) )
			{
				opened_irrigation_valves_counter++;
			}
		}
	}

	if ( opened_irrigation_valves_counter == valves.size() )
		setState( true );		// All Irrigation Valves in the Valve Group are "opened"
	
	return state;
}

bool ValveGroup::closeIrrigationValves( )
{
	int irrigation_valves_left_open = 0;

	// STEP THROUGH THE IRRIGATION VALVES IN THE GROUP AND CLOSE ONLY THOSE IRRIGATION VALVES THAT WILL NOT BE USED IN THE NEXT VALVE GROUP
	for ( valve_itr = valves.begin(); valve_itr != valves.end() ; ++valve_itr )
	{
		DBIndex temp_id = valve_itr->getValveId( );

		if ( doesIrrigationValveExistInTheNextValveGroup( temp_id ) )
		{
			irrigation_valves_left_open++;
		}
		else
		{
			// Check to see if the Irrigation Valve is open
			if ( valve_itr->getState( ) )
			{
				if ( !valve_itr->closeValve( ) )
				{
					opened_irrigation_valves_counter--;
				}
			}
		}
	}

	if ( ( opened_irrigation_valves_counter - irrigation_valves_left_open ) == 0 )
		setState( false ); 	// All Irrigation Valves in the Valve Group are "closed"	

	return state;
}

void ValveGroup::removeClosedValves( )
{
	DBIndex water_meter_id; // , main_line_id;

	valve_itr = valves.begin();

    while ( valve_itr != valves.end() )
    {
        if ( !valve_itr->getState( ) )
		{
			water_meter_id = valve_itr->getWaterMeterId( );

			if ( water_meter_id != 0 )
				DB_RemoveRecordListener( DBWATERMETER_TYPE, water_meter_id, task_id );

            valve_itr = valves.erase(valve_itr);

		}
        else
           ++valve_itr;
    }

	// TO DO: Implement de-registration of listeners on the Main Line's Water Meter when the Main Line is no longer used !!!!
	// Consider adding Main Line objects to Valve Group

	main_valve_itr = main_valves.begin();

    while ( main_valve_itr != main_valves.end() )
    {
        if ( !main_valve_itr->getState( ) )
		{
            main_valve_itr = main_valves.erase(main_valve_itr);
		}
        else
           ++main_valve_itr;
    }
}


//void ValveGroup::setGroup( const ValveGroup & group )
//{
//	if ( this != &group )    
//	{
//		setTaskId( group.task_id );
//		setGroupNumber( group.group_number );
//		setNumberGroups( group.number_of_groups );
//		setValves( group.valves );
//		setIrrigationType( group.irrigation_type );
//		setIrrigationArea( group.irrigation_area );
//		setStartTime( group.start_time );
//		setEndTime( group.end_time );
//		setTimeStamp( group.time_stamp );
//		setState( group.state );
//		setDosingStationId( group.dosing_station_id );
//		// setFertigationRecipe( const IFertigationRecipe recipe[] );
//
//		// IRRIGATION BY (DEPTH, QUANTITY, DURATION)
//		setIrrigationSet( group.set );
//		setNominalIrrigationDelivered( group.nominal_delivered );
//		setMeasuredIrrigationDelivered( group.measured_delivered );
//		setNominalIrrigationLeft( group.nominal_left );
//		setMeasuredIrrigationLeft( group.measured_left );
//
//		setDurationSet( group.duration_set );
//		setNominalDurationLeft( group.nominal_duration_left );
//		setMeasuredDurationLeft( group.measured_duration_left );
//		
//		setOpenedMainValvesCounter( group.opened_main_valves_counter );
//		setOpenedIrrigationValvesCounter( group.opened_irrigation_valves_counter );
//		setNominalFlowRateCubicMetersPerHour( group.nominal_flow_rate_cubic_meters_per_hour );
//		setNominalFlowLitersPerSecond( group.nominal_flow_rate_liters_per_second );
//	}
//}

//ValveGroup::ValveGroup( const ValveGroup & group )
//{ 
//	if ( this != &group ) 
//		setGroup( group ); 
//}

/*

void ValveGroup::convertNominalDepthIntoNominalQuantity( )
{
	// Irrigation Water Quantity (m3) = Irrigation Area (m2) * Depth (m)
	// quantity_set = irrigation_area * ( depth_set / 1000 );
	quantity_set = irrigation_area / 1000 * depth_set;
	nominal_quantity_left = quantity_set;

	// Note: All calculations are performed in Metric units ONLY				
	// Irrigation Rate = the amount of water given as follows: 		
	// Each Irrigation Valve's nominal flow is defined in m3/hr
	// Hence, 1000 liters per hour = 0.277 liters per second

	// ALGORITHM:
	// Sum up Irrigation Valves' Total Irrigation Area (TIA)
	// Convert TIA into Hectares (1 Acre = 0.404686 Hectares or 1 Hectare = 2.47105 Acres)
	// Convert Hectares into Square Meters
	// Calculate Irrigation Quantity by multiplying Area by Depth

	// Depth = Volume   /  Area
	// 1mm   = 1 m3    per 1000 m2
	// 1mm   = 1 liter per 1    m2

	// EXAMPLE:
	// Depth Set = 5 mm
	// Number of Irrigation Valves = 10
	// Each Valve irrigates 1 Acre of land
	// Total Irrigation Area (TIA) = 10 Acres = 4.04686 Hectares = 40,468.6 m2
	// 0.005 m = Quantity / 40,468.6 m2
	// Quantity = 40,468.6 m2 * 0.005 m = 202.343 m3

	// REVERSE EXAMPLE:
	// Quantity Left = 202.343 m3
	// Total Irrigation Area = 40,468.65 m2
	// Depth Left = Quantity Left / Total Irrigation Area = 202.343 m3 / 40,468.6 m2 = 0.005 m
}

void ValveGroup::convertMeasuredDepthIntoMeasuredQuantity( )
{
	quantity_set = irrigation_area / 1000 * depth_set;
	measured_quantity_left = quantity_set;
}

void ValveGroup::convertNominalQuantityIntoNominalDuration( )
{
	// nominal_flow_cubic_meters_per_second = nominal_flow_rate_cubic_meters_per_hour / 3600;
	// float duration_in_seconds = ( quantity_set / nominal_flow_rate_cubic_meters_per_hour ) * 3600;
	// long duration( duration_in_seconds );

	// Irrigation Duration (hr) = Irrigation Quantity (m3) / Nominal Flow (m3/hr)
	long duration( ( quantity_set / getNominalFlowRateCubicMetersPerHour( ) ) * 3600 );

	duration_set = duration;
	nominal_duration_left = duration;
}

void ValveGroup::convertMeasuredQuantityIntoMeasuredDuration( )
{
	// Irrigation Duration (hr) = Irrigation Quantity (m3) / Measured Flow (m3/hr)
	long duration( ( quantity_set / getMeasuredFlowRateCubicMetersPerHour( ) ) * 3600 );

	duration_set = duration;
	measured_duration_left = duration;
}

void ValveGroup::convertNominalDurationIntoNominalQuantity( )
{
	// Irrigation Quantity (m3) = Irrigation Duration (sec) * Nominal Flow (m3/sec)
	quantity_set = duration_set.ToSeconds( ) * ( getNominalFlowRateCubicMetersPerHour( ) / 3600 );
	nominal_quantity_left = quantity_set;
}

void ValveGroup::convertMeasuredDurationIntoMeasuredQuantity( )
{
	// Irrigation Quantity (m3) = Irrigation Duration (sec) * Measured Flow (m3/sec)
	quantity_set = duration_set.ToSeconds( ) * ( getMeasuredFlowRateCubicMetersPerHour( ) / 3600 );
	measured_quantity_left = quantity_set;
}

void ValveGroup::convertNominalQuantityIntoNominalDepth( )
{
	// Depth (m) = Irrigation Quantity (m3) / Irrigation Area (m2)
	depth_set = quantity_set / irrigation_area;
	nominal_depth_left = depth_set;
}

void ValveGroup::convertMeasuredQuantityIntoMeasuredDepth( )
{
	// Depth (m) = Irrigation Quantity (m3) / Irrigation Area (m2)
	depth_set = quantity_set / irrigation_area;
	measured_depth_left = depth_set;
}

*/


// void ValveGroup::setFertigationRecipe( const IFertigationRecipe recipe[] ) { memcpy( this->recipe, recipe, sizeof(this->recipe) ); }

//// IRRIGATION BY (DEPTH, QUANTITY, DURATION)
//void ValveGroup::setIrrigationSet( IIrrigationSetLeft _set ) 
//{ 
//	set = _set;
//
//	// Set the appropriate irrigation by depth/quantity/duration set value based on the irrigation type
//	switch ( irrigation_type )
//	{
//		case IRRIGATION_BY_DEPTH:		// Measured in mm
//
//			setDepthSet( depth_set );
//			setQuantitySet( convertDepthIntoQuantity( depth_set ) );
//			setDurationSet( convertQuantityIntoDuration( quantity_set, this->nominal_flow_rate_cubic_meters_per_hour ) );
//			break;
//
//		case IRRIGATION_BY_QUANTITY:	// Measured in m3
//
//			setQuantitySet( quantity_set );
//			setDepthSet( convertQuantityIntoDepth( quantity_set ) );
//			setDurationSet( convertQuantityIntoDuration( quantity_set, this->nominal_flow_rate_cubic_meters_per_hour ) );
//			break;
//		
//		case IRRIGATION_BY_DURATION:	// Measured in hr:mm:ss
//
//			setDurationSet( duration_set );
//			setQuantitySet( convertDurationIntoQuantity( duration_set, this->nominal_flow_rate_cubic_meters_per_hour ) );
//			setDepthSet( convertQuantityIntoDepth( quantity_set ) );
//			break;
//
//		default:	// none
//			break;
//	} // end switch ( irrigation_type )
//}
//
//void ValveGroup::setNominalIrrigationLeft( IIrrigationSetLeft left ) 
//{ 
//	nominal_left = left;
//
//	// Set the appropriate irrigation by depth/quantity/duration set value based on the irrigation type
//	switch ( irrigation_type )
//	{
//		case IRRIGATION_BY_DEPTH:		// Measured in mm
//
//			setNominalDepthLeft( nominal_depth_left );
//			setNominalQuantityLeft( convertDepthIntoQuantity( nominal_depth_left ) );
//			setNominalDurationLeft( convertQuantityIntoDuration( nominal_quantity_left, this->nominal_flow_rate_cubic_meters_per_hour ) ); 
//			break;
//
//		case IRRIGATION_BY_QUANTITY:	// Measured in m3
//
//			setNominalQuantityLeft( nominal_quantity_left );
//			setNominalDepthLeft( convertQuantityIntoDepth( nominal_quantity_left ) );
//			setNominalDurationLeft( convertQuantityIntoDuration( nominal_quantity_left, this->nominal_flow_rate_cubic_meters_per_hour ) );	
//			break;
//		
//		case IRRIGATION_BY_DURATION:	// Measured in hr:mm:ss
//
//			setNominalDurationLeft( nominal_duration_left );		
//			setNominalQuantityLeft( convertDurationIntoQuantity( nominal_duration_left, this->nominal_flow_rate_cubic_meters_per_hour ) );
//			setNominalDepthLeft( convertQuantityIntoDepth( nominal_quantity_left ) );
//			break;
//
//		default:	// none
//			break;
//	} // end switch ( irrigation_type )
//}
//
//void ValveGroup::setMeasuredIrrigationLeft( IIrrigationSetLeft left ) 
//{ 
//	measured_left = left;
//
//	// Set the appropriate irrigation by depth/quantity/duration set value based on the irrigation type
//	switch ( irrigation_type )
//	{
//		case IRRIGATION_BY_DEPTH:		// Measured in mm
//
//			setMeasuredDepthLeft( measured_depth_left );
//			setMeasuredQuantityLeft( convertDepthIntoQuantity( measured_depth_left ) );
//			setMeasuredDurationLeft( convertQuantityIntoDuration( measured_quantity_left, this->measured_flow_rate_cubic_meters_per_hour ) ); // TO DO: Resolve flow 
//			break;
//
//		case IRRIGATION_BY_QUANTITY:	// Measured in m3
//
//			setMeasuredQuantityLeft( measured_quantity_left );
//			setMeasuredDepthLeft( convertQuantityIntoDepth( measured_quantity_left ) );
//			setMeasuredDurationLeft( convertQuantityIntoDuration( measured_quantity_left, this->measured_flow_rate_cubic_meters_per_hour ) );	// TO DO: Resolve flow
//			break;
//		
//		case IRRIGATION_BY_DURATION:	// Measured in hr:mm:ss
//
//			setMeasuredDurationLeft( measured_duration_left );		
//			setMeasuredQuantityLeft( convertDurationIntoQuantity( measured_duration_left, this->measured_flow_rate_cubic_meters_per_hour ) );	// TO DO: Resolve flow 
//			setMeasuredDepthLeft( convertQuantityIntoDepth( measured_quantity_left ) );
//			break;
//
//		default:	// none
//			break;
//	} // end switch ( irrigation_type )
//}

//IIrrigationSetLeft ValveGroup::updateNominalIrrigationLeft( long elapsed_time )
//{
//	switch ( irrigation_type )
//	{
//		case IRRIGATION_BY_DEPTH:
//
//			nominal_depth_left = updateNominalDepthLeft( elapsed_time );
//			break;
//
//		case IRRIGATION_BY_QUANTITY:
//
//			nominal_quantity_left = updateNominalQuantityLeft( elapsed_time );
//			break;
//
//		case IRRIGATION_BY_DURATION:
//
//			nominal_duration_left = updateNominalDurationLeft( elapsed_time );
//			break;
//		
//		default:
//	}
//
//	return nominal_left;
//}
//
//IIrrigationSetLeft ValveGroup::updateMeasuredIrrigationLeft( long elapsed_time )
//{
//	switch ( irrigation_type )
//	{
//		case IRRIGATION_BY_DEPTH:
//
//			measured_depth_left = updateMeasuredDepthLeft( elapsed_time );
//			break;
//
//		case IRRIGATION_BY_QUANTITY:
//
//			measured_quantity_left = updateMeasuredQuantityLeft( elapsed_time );
//			break;
//
//		case IRRIGATION_BY_DURATION:
//
//			measured_duration_left = updateMeasuredDurationLeft( elapsed_time );
//			break;
//		
//		default:
//	}
//
//	return measured_left;
//}

// Retrieve Main Line Id
//if ( DB_GetField(DBIRRIGATIONVALVE_TYPE, task_id, valve_id, DBIRRIGATIONVALVE_LINE_ID, &main_line_id) )
//{
//	// Retrieve Water Meter Id
//	if ( DB_GetField(DBMAINLINE_TYPE, task_id, main_line_id, DBMAINLINE_WATER_METER_ID, &water_meter_id) )
//	{
//		// Check to see if the Main Line has a Water Meter installed and if yes then register to listen to it
//		if ( water_meter_id != 0 )
//		{
//			DB_AddRecordListener( DBWATERMETER_TYPE, water_meter_id, task_id );
//		}
//	}
//}
//
//// Retrieve Main Line Id
//if ( DB_GetField(DBIRRIGATIONVALVE_TYPE, task_id, valve_id, DBIRRIGATIONVALVE_LINE_ID, &main_line_id) )
//{
//	// Retrieve Dosing Station Id
//	if ( DB_GetField(DBMAINLINE_TYPE, task_id, main_line_id, DBMAINLINE_DOSING_STATION_ID, &dosing_station_id) )
//	{
//		// Retrieve Water Meter Id
//		if ( DB_GetField(DBDOSINGSTATION_TYPE, task_id, dosing_station_id, DBDOSINGSTATION_WATER_METER_ID, &water_meter_id) )
//		{
//			// Check to see if the Dosing Station has a Water Meter installed and if yes then register to listen to it
//			if ( water_meter_id != 0 )
//			{
//				DB_AddRecordListener( DBWATERMETER_TYPE, water_meter_id, task_id );
//			}
//		}
//	}
//}
//
//// Retrieve Main Line Id
//if ( DB_GetField(DBIRRIGATIONVALVE_TYPE, task_id, valve_id, DBIRRIGATIONVALVE_LINE_ID, &main_line_id) )
//{
//	// Retrieve Dosing Station Id
//	if ( DB_GetField(DBMAINLINE_TYPE, task_id, main_line_id, DBMAINLINE_DOSING_STATION_ID, &dosing_station_id) )
//	{
//		// Retrieve Filter Station Id
//		if ( DB_GetField(DBDOSINGSTATION_TYPE, task_id, dosing_station_id, DBDOSINGSTATION_FILTER_STATION_ID, &filter_station_id) )
//		{
//			// Retrieve Water Meter Id
//			if ( DB_GetField(DBFILTERSTATION_TYPE, task_id, filter_station_id, DBFILTERSTATION_WATER_METER_ID, &water_meter_id) )
//			{
//				// Check to see if the Filter Station has a Water Meter installed and if yes then register to listen to it
//				if ( water_meter_id != 0 )
//				{
//					DB_AddRecordListener( DBWATERMETER_TYPE, water_meter_id, task_id );
//				}
//			}
//		}
//	}
//}

// IRRIGATION BY DEPTH
// setDepthSet( group.depth_set );
// setNominalDepthLeft( group.nominal_depth_left );
// setMeasuredDepthLeft( group.measured_depth_left );

// IRRIGATION BY QUANTITY
// setQuantitySet( group.quantity_set );
// setNominalQuantityLeft( group.nominal_quantity_left );
// setMeasuredQuantityLeft( group.measured_quantity_left );

// IRRIGATION BY DURATION
// setDurationSet( group.duration_set );
// setNominalDurationLeft( group.nominal_duration_left );
// setMeasuredDurationLeft( group.measured_duration_left );

//// IRRIGATION BY DEPTH
//setDepthSet( 0 );
//setNominalDepthLeft( 0 );
//setMeasuredDepthLeft( 0 );

//// IRRIGATION BY QUANTITY
//setQuantitySet( 0 );
//setNominalQuantityLeft( 0 );
//setMeasuredQuantityLeft( 0 );

//// IRRIGATION BY DURATION
//setDurationSet( 0 );
//setNominalDurationLeft( 0 );
//setMeasuredDurationLeft( 0 );

// IRRIGATION BY DEPTH
// setDepthSet( 0 );
// setNominalDepthLeft( 0 );
// setMeasuredDepthLeft( 0 );

// IRRIGATION BY QUANTITY
// setQuantitySet( 0 );
// setNominalQuantityLeft( 0 );
// setMeasuredQuantityLeft( 0 );

// IRRIGATION BY DURATION
// setDurationSet( 0 );
// setNominalDurationLeft( 0 );
// setMeasuredDurationLeft( 0 );

// Conversion formulas: 
// flow_rate_liters_per_second = ( flow_rate_cubic_meters_per_hour * 5 ) / 18;
// flow_rate_cubic_meters_per_hour = ( flow_rate_liters_per_second * 18 ) / 5;

// DBType table_type = water_meter_messages_itr->type;
// DBIndex record_id = water_meter_messages_itr->index;
// DBColumn column_id = water_meter_messages_itr->column;
// DBOperation operation_type = water_meter_messages_itr->operation;

// DBWaterMeter:
// DBWATERMETER_PULSE_COUNTER,
// DBWATERMETER_PULSES,
// DBWATERMETER_WATER_LEAK_PERIOD,
// DBWATERMETER_STATE,
// DBWATERMETER_LAST_PULSE_TIMESTAMP,
// DBWATERMETER_FIRST_PULSE_TIMESTAMP,
// DBWATERMETER_ALLOCATEE_TYPE,
// DBWATERMETER_INPUT_ID,
// DBWATERMETER_INPUT_LOCATION,
// DBWATERMETER_WATER_LEAK_QUANTITY,
// DBWATERMETER_RATIO,
// DBWATERMETER_ALLOCATEE_ID,
// DBWATERMETER_MAIN_LINE_ID


//// Check to see if ...
//if ( true )										
//{
//	// Check to see if the delay to start counting the time interval between the pulses has not been set
//	if ( nominal_time_interval_between_pulses == 0 )
//		water_meter_pulse_timeout = WATER_METER_PULSE_TIMEOUT_STATE_NONE;		// Set the alarm to be ready to be activated
//
//	switch ( water_meter_pulse_timeout )
//	{
//		case WATER_METER_PULSE_TIMEOUT_STATE_NONE:								// The valve is in neutral state
//
//			water_meter_time_stamp = DB_TimeNow( );
//			water_meter_pulse_timeout = WATER_METER_PULSE_TIMEOUT_STATE_WAIT_ON;// Commence the countdown until the expected time interval between the pulses expires
//			break;
//
//		case WATER_METER_PULSE_TIMEOUT_STATE_WAIT_ON:							// The device is waiting to be activated
//
//			if ( time_interval_expired( water_meter_time_stamp, nominal_time_interval_between_pulses ) )
//				water_meter_pulse_timeout = WATER_METER_PULSE_TIMEOUT_STATE_ON;	// The alarm can now be activated
//
//			break;
//
//		case WATER_METER_PULSE_TIMEOUT_STATE_ON:								// The alarm is ready to be activated
//
//			water_meter_pulse_timeout = true;									// Mark the alarm as being opened
//		
//			break;
//
//		case WATER_METER_PULSE_TIMEOUT_STATE_WAIT_OFF:							// The alarm is waiting to be de-activated
//
//			water_meter_pulse_timeout = WATER_METER_PULSE_TIMEOUT_STATE_NONE;
//			break;
//
//		case WATER_METER_PULSE_TIMEOUT_STATE_OFF:								// The alarm is de-activated
//
//			water_meter_pulse_timeout = WATER_METER_PULSE_TIMEOUT_STATE_NONE;
//			break;
//			
//		default: // nothing
//	}
//} 