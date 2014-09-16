
// Irrigation Sequence class implementation file

#include "IrrigationSequence.h"

// Global arrays of indices that is stored in the ProcessExecution_Manager.c file
extern DBIndex IrrigationProgramID[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
extern DBIndex IrrigationSequenceID[MAX_CONCURRENT_IRRIGATION_SEQUENCES];

IrrigationSequence::IrrigationSequence( ) 
{ 
    // empty body for now
}

IrrigationSequence::IrrigationSequence( DBTask task_id ) : task_id( task_id )
{       
	// Retrieve Irrigation Program Id Database table index (Range [0 - 15])
	program_id = IrrigationProgramID[task_id - MLINE1_SLOT];                

	// Retrieve Irrigation Sequence Id Database table index (Range [0 - 9])
	sequence_id = IrrigationSequenceID[task_id - MLINE1_SLOT]; 
	
	// Calculate the message box id (MLINE1_SLOT = 7 and MBOX_IRR_PROG_1 = 11 defined in tasks.h file)
	// message_box_id = task_id - MLINE1_SLOT + MBOX_IRR_PROG_1;
	
	// Initialize the message box object associated with this Irrigation Sequence
	message_box.Initialize( task_id, ( task_id - MLINE1_SLOT + MBOX_IRR_PROG_1 ) );

	// Register listener (task_id) with a particular record (record_id) in a particular table (table_type) 
	// in order to be able to check (i.e. listen) to any changes in the record's field values
	// bool DB_AddRecordListener(DBType type, DBIndex index, DBTask listener);
	DB_AddRecordListener( DBVALVESEQUENCE_TYPE, sequence_id, task_id );

	// DB_AddRecordListener( DBIRRIGATIONPROGRAM_TYPE, program_id, task_id );

	// Reset Trinity
	for( int v = 0; v < MAX_VALVES_PER_SEQUENCE; ++v )
	{
		Trinity[v].valve_id = 0;							// Valid Valve Id range: [1 - 255]
		Trinity[v].group_number = MAX_GROUPS_PER_SEQUENCE;	// Valid Valve Group Number range: [1 - 100]
	}
} 

IrrigationSequence::~IrrigationSequence( ) 
{ 
	// empty body for now
}

/////////////////////////////////////////////////////////////
// ACCESSOR FUNCTIONS
/////////////////////////////////////////////////////////////
DBTask IrrigationSequence::getTaskId( ) const { return task_id; }
DBIndex IrrigationSequence::getProgramId( ) const { return program_id; }
DBIndex IrrigationSequence::getSequenceId( ) const { return sequence_id; }
DBIndex IrrigationSequence::getDosingStationId( ) const { return dosing_station_id; }
DBIndex IrrigationSequence::getProgramSensorId( ) const { return sensor_id; }
DBIndex IrrigationSequence::getProgramConditionId( ) const { return condition_id; }
DBIndex IrrigationSequence::getAlarmId( ) const { return alarm_id; }
// byte IrrigationSequence::getMessageBoxNumber( ) const;

// IRRIGATION PROGRAM PROPERTIES
DBDatePP IrrigationSequence::getProgramStartDate( ) const { return program_start_date; }
DBDatePP IrrigationSequence::getProgramEndDate( ) const { return program_end_date; }
long IrrigationSequence::getProgramStartTime( ) const { return program_start_time; }
long IrrigationSequence::getProgramEndTime( ) const { return program_end_time; }
long IrrigationSequence::getCycleDelay( ) const { return cycle_delay; };
long IrrigationSequence::getProgramMaxRestTime( ) const { return max_rest_time; }
byte IrrigationSequence::getSequenceNumberCyclesSet( ) const { return number_cycles_set; }
byte IrrigationSequence::getSequenceNumberCyclesElapsed( ) const { return number_of_cycles_elapsed; }
byte IrrigationSequence::getSequenceNumberCyclesLeft( ) const { return number_cycles_left; }
SensorType IrrigationSequence::getProgramSensorType( ) const { return sensor_type; }
float IrrigationSequence::getProgramSetPoint( ) const { return set_point; }
float IrrigationSequence::getAccumulatedSetPoint( ) const { return accumulated_set_point; }
int IrrigationSequence::getProgramCropFactor( ) const { return crop_factor; }

// VALVE (IRRIGATION) SEQUENCE PROPERTIES
IrrigationSequenceState IrrigationSequence::getSequenceState( ) const { return state; }
IrrigationManagerRequest IrrigationSequence::getManagerRequest( ) const { return request; }		
IrrigationSequenceResponse IrrigationSequence::getSequenceResponse( ) const { return response; }	
IrrigationMethod IrrigationSequence::getIrrigationMethod( ) const { return irrigation_method; }
IrrigationType IrrigationSequence::getIrrigationType( ) const { return irrigation_type; }
int IrrigationSequence::getGroupNumber( ) const { return group_number; }
int IrrigationSequence::getNumberGroups( ) const { return number_of_groups; }
int IrrigationSequence::getNumberValves( ) const { return number_of_valves; }
const MessageBox & IrrigationSequence::getMessageBox( ) const { return message_box; }
const IrrigationSequence & IrrigationSequence::getSequence( ) const { return *this; }
// IFertigationRecipe * IrrigationSequence::getFertigationRecipe( ) { return recipe; }

// IRRIGATION BY (DEPTH, QUANTITY, DURATION)
IIrrigationSetLeft IrrigationSequence::getSequenceIrrigationSet( ) const { return set; }
IIrrigationSetLeft IrrigationSequence::getSequenceIrrigationLeft( ) const { return left; }
// IIrrigationSetLeft IrrigationSequence::getSequenceNominalIrrigationLeft( ) const { return nominal_left; }
// IIrrigationSetLeft IrrigationSequence::getSequenceMeasuredIrrigationLeft( ) const { return measured_left; }
// IIrrigationSetLeft IrrigationSequence::getValveGroupIrrigationSet( ) const { return valve_group.getIrrigationSet( ); }

// TO DO: In the end the following two functions should be replaced with the one above
IIrrigationSetLeft IrrigationSequence::getValveGroupNominalIrrigationLeft( ) const { return valve_group.getNominalIrrigationLeft( ); }
IIrrigationSetLeft IrrigationSequence::getValveGroupMeasuredIrrigationLeft( ) const { return valve_group.getMeasuredIrrigationLeft( ); }

// IRRIGATION BY DEPTH
float IrrigationSequence::getSequenceDepthSet( ) const { return set.depth; }
float IrrigationSequence::getSequenceDepthLeft( ) const { return left.depth; }
// float IrrigationSequence::getSequenceNominalDepthLeft( ) const { return nominal_left.depth; }
// float IrrigationSequence::getSequenceMeasuredDepthLeft( ) const { return measured_left.depth; }
// float IrrigationSequence::getValveGroupDepthSet( ) const { return valve_group.getDepthSet( ); }
float IrrigationSequence::getValveGroupNominalDepthLeft( ) const { return valve_group.getNominalDepthLeft( ); }
float IrrigationSequence::getValveGroupMeasuredDepthLeft( ) const { return valve_group.getMeasuredDepthLeft( ); }

// IRRIGATION BY QUANTITY
float IrrigationSequence::getSequenceQuantitySet( ) const { return set.quantity; }
float IrrigationSequence::getSequenceQuantityLeft( ) const { return left.quantity; }
// float IrrigationSequence::getSequenceNominalQuantityLeft( ) const { return nominal_left.quantity; }
// float IrrigationSequence::getSequenceMeasuredQuantityLeft( ) const { return measured_left.quantity; }
// float IrrigationSequence::getValveGroupQuantitySet( ) const { return valve_group.getQuantitySet( ); }
float IrrigationSequence::getValveGroupNominalQuantityLeft( ) const { return valve_group.getNominalQuantityLeft( ); }
float IrrigationSequence::getValveGroupMeasuredQuantityLeft( ) const { return valve_group.getMeasuredQuantityLeft( ); }

// IRRIGATION BY DURATION
long IrrigationSequence::getSequenceDurationSet( ) const { return duration_set; }
long IrrigationSequence::getSequenceDurationLeft( ) const { return duration_left; }
// long IrrigationSequence::getSequenceNominalDurationLeft( ) const { return nominal_left.duration; }
// long IrrigationSequence::getSequenceMeasuredDurationLeft( ) const { return measured_left.duration; }
// long IrrigationSequence::getValveGroupDurationSet( ) const { return valve_group.getDurationSet( ); }
long IrrigationSequence::getValveGroupNominalDurationLeft( ) const { return valve_group.getNominalDurationLeft( ); }
long IrrigationSequence::getValveGroupMeasuredDurationLeft( ) const { return valve_group.getMeasuredDurationLeft( ); }

// TIME STAMPS
long IrrigationSequence::getSequenceStartTime( ) const { return sequence_start_time; }
long IrrigationSequence::getSequenceEndTime( ) const { return sequence_end_time; }
long IrrigationSequence::getSequenceTimeStamp( ) const { return sequence_time_stamp; }

long IrrigationSequence::getCycleStartTime(  ) const { return cycle_start_time; }
long IrrigationSequence::getCycleEndTime( ) const { return cycle_end_time; }
long IrrigationSequence::getCycleTimeStamp( ) const { return cycle_time_stamp; }

long IrrigationSequence::getValveGroupStartTime( ) const { return valve_group.getStartTime( ); }
long IrrigationSequence::getValveGroupEndTime( ) const { return valve_group.getEndTime( ); }
long IrrigationSequence::getValveGroupTimeStamp( ) const { return valve_group.getTimeStamp( ); }

long IrrigationSequence::getIntegralCalculationTimeStamp( )  { return integral_calculation_time_stamp; }

// WATER FLOW
float IrrigationSequence::getValveGroupNominalFlowRateLitersPerSecond( ) const { return valve_group.getNominalFlowRateLitersPerSecond( ); }
float IrrigationSequence::getValveGroupNominalFlowRateCubicMetersPerHour( ) const { return valve_group.getNominalFlowRateCubicMetersPerHour( ); }
float IrrigationSequence::getValveGroupMeasuredFlowRateLitersPerSecond( ) { return valve_group.getMeasuredFlowRateLitersPerSecond( ); }
float IrrigationSequence::getValveGroupMeasuredFlowRateCubicMetersPerHour( ) { return valve_group.getMeasuredFlowRateCubicMetersPerHour( ); }


////////////////////////////////////////////////////////////
// MUTATOR FUNCTIONS
////////////////////////////////////////////////////////////
void IrrigationSequence::setTaskId( DBTask id ) { task_id = id; }
void IrrigationSequence::setProgramId( DBIndex id ) { program_id = id; }
void IrrigationSequence::setSequenceId( DBIndex id ) { sequence_id = id; }
void IrrigationSequence::setDosingStationId( DBIndex id ) { dosing_station_id = id; }
void IrrigationSequence::setProgramSensorId( DBIndex id ) { sensor_id = id; }
void IrrigationSequence::setProgramConditionId( DBIndex id ) { condition_id = id; }
void IrrigationSequence::setAlarmId( DBIndex id ) { alarm_id = id; }
// void IrrigationSequence::setMessageBoxNumber( byte number ) { message_box_number = number; }

// IRRIGATION PROGRAM PROPERTIES
void IrrigationSequence::setProgramStartDate( DBDatePP date ) { program_start_date = date; }
void IrrigationSequence::setProgramEndDate( DBDatePP date ) { program_end_date = date; }
void IrrigationSequence::setProgramStartTime( long time ) { program_start_time = time; }
void IrrigationSequence::setProgramEndTime( long time ) { program_end_time = time; }
void IrrigationSequence::setCycleDelay( long delay ) { cycle_delay = delay; }
void IrrigationSequence::setProgramMaxRestTime( long time ) { max_rest_time = time; }
void IrrigationSequence::setSequenceNumberCyclesSet( byte cycles ) { number_cycles_set = cycles; }
void IrrigationSequence::setSequenceNumberCyclesElapsed( byte cycles ) { number_of_cycles_elapsed = cycles; }
void IrrigationSequence::incrementSequenceNumberCyclesElapsed( ) { ++number_of_cycles_elapsed; }
void IrrigationSequence::setSequenceNumberCyclesLeft( byte cycles ) { number_cycles_left = cycles; }
void IrrigationSequence::decrementSequenceNumberCyclesLeft( ) 
{ 
	if ( number_cycles_left > 0 ) 
		--number_cycles_left;
	else
		number_cycles_left = 0;
}
void IrrigationSequence::setProgramSensorType( SensorType type ) { sensor_type = type; }
void IrrigationSequence::setProgramSetPoint( float point ) { set_point = point; }
void IrrigationSequence::setAccumulatedSetPoint( float set_point ) { accumulated_set_point = set_point; }
void IrrigationSequence::updateAccumulatedSetPoint( float set_point ) { accumulated_set_point += set_point; }
void IrrigationSequence::setProgramCropFactor( int factor ) { crop_factor = factor; }

// VALVE (IRRIGATION) SEQUENCE PROPERTIES
void IrrigationSequence::setSequenceState( IrrigationSequenceState state ) { this->state = state; }
void IrrigationSequence::setManagerRequest( IrrigationManagerRequest request ) { this->request = request; }		
void IrrigationSequence::setSequenceResponse( IrrigationSequenceResponse response ) { this->response = response; }	
void IrrigationSequence::setIrrigationMethod( IrrigationMethod method ) { irrigation_method = method; }
void IrrigationSequence::setIrrigationType( IrrigationType type ) { irrigation_type = type; }
// void IrrigationSequence::setFertigationRecipe( const IFertigationRecipe recipe[] ) { memcpy( this->recipe, recipe, sizeof(this->recipe) ); }
void IrrigationSequence::setGroupNumber( int number ) { group_number = number; }
void IrrigationSequence::setNumberGroups( int number ) { number_of_groups = number; }
void IrrigationSequence::setNumberValves( int number ) { number_of_valves = number; }
void IrrigationSequence::setValveGroup( const ValveGroup & group ) { valve_group = group; }
void IrrigationSequence::setMessageBox( const MessageBox & box ) { message_box = box; }

// IRRIGATION BY (DEPTH, QUANTITY, DURATION)
void IrrigationSequence::setSequenceIrrigationSet( IIrrigationSetLeft set ) { this->set = set; }
void IrrigationSequence::setSequenceIrrigationLeft( IIrrigationSetLeft left ) { this->left = left; }
// void IrrigationSequence::setSequenceNominalIrrigationLeft( IIrrigationSetLeft left ) { nominal_left = left; }
// void IrrigationSequence::setSequenceMeasuredIrrigationLeft( IIrrigationSetLeft left ) { measured_left = left; }
// void IrrigationSequence::setValveGroupIrrigationSet( IIrrigationSetLeft set ) { valve_group.setIrrigationSet( set ); }
// void IrrigationSequence::setValveGroupNominalIrrigationLeft( IIrrigationSetLeft left ) { valve_group.setNominalIrrigationLeft( left ); }
// void IrrigationSequence::setValveGroupMeasuredIrrigationLeft( IIrrigationSetLeft left ) { valve_group.setMeasuredIrrigationLeft( left ); }
IIrrigationSetLeft IrrigationSequence::updateSequenceIrrigationLeft( long elapsed_time ) { return valve_group.updateIrrigationLeft( elapsed_time ); }
// IIrrigationSetLeft IrrigationSequence::updateValveGroupNominalIrrigationLeft( long elapsed_time ) { return valve_group.updateNominalIrrigationLeft( elapsed_time ); }
// IIrrigationSetLeft IrrigationSequence::updateValveGroupMeasuredIrrigationLeft( long elapsed_time ) { return valve_group.updateMeasuredIrrigationLeft( elapsed_time ); }

// IRRIGATION BY DEPTH
void IrrigationSequence::setSequenceDepthSet( float depth ) { this->set.depth = depth; }
void IrrigationSequence::setSequenceDepthLeft( float depth ) { this->left.depth = depth; }
// void IrrigationSequence::setSequenceNominalDepthLeft( float left ) { nominal_left.depth = left; }
// void IrrigationSequence::setSequenceMeasuredDepthLeft( float left ) { measured_left.depth = left; }
// void IrrigationSequence::setValveGroupDepthSet( float set ) { valve_group.setDepthSet( set ); }
// void IrrigationSequence::setValveGroupNominalDepthLeft( float left ) { valve_group.setNominalDepthLeft( left ); }
// void IrrigationSequence::setValveGroupMeasuredDepthLeft( float left ) { valve_group.setMeasuredDepthLeft( left ); }
// float IrrigationSequence::updateValveGroupDepthLeft( long elapsed_time ) { return valve_group.updateDepthLeft( elapsed_time ); }
// float IrrigationSequence::updateValveGroupNominalDepthLeft( long elapsed_time ) { return valve_group.updateNominalDepthLeft( elapsed_time ); }
// float IrrigationSequence::updateValveGroupMeasuredDepthLeft( long elapsed_time ) { return valve_group.updateMeasuredDepthLeft( elapsed_time ); }

// IRRIGATION BY QUANTITY
void IrrigationSequence::setSequenceQuantitySet( float quantity ) { this->set.quantity = quantity; }
void IrrigationSequence::setSequenceQuantityLeft( float quantity ) { this->left.quantity = quantity; }
// void IrrigationSequence::setSequenceNominalQuantityLeft( float left ) { nominal_left.quantity = left; }
// void IrrigationSequence::setSequenceMeasuredQuantityLeft( float left ) { measured_left.quantity = left; }
// void IrrigationSequence::setValveGroupQuantitySet( float set ) { valve_group.setQuantitySet( set ); }
// void IrrigationSequence::setValveGroupNominalQuantityLeft( float left ) { valve_group.setNominalQuantityLeft( left ); }
// void IrrigationSequence::setValveGroupMeasuredQuantityLeft( float left ) { valve_group.setMeasuredQuantityLeft( left ); }
// float IrrigationSequence::updateValveGroupQuantityLeft( long elapsed_time ) { return valve_group.updateQuantityLeft( elapsed_time ); }
// float IrrigationSequence::updateValveGroupNominalQuantityLeft( long elapsed_time ) { return valve_group.updateNominalQuantityLeft( elapsed_time ); }
// float IrrigationSequence::updateValveGroupMeasuredQuantityLeft( long elapsed_time ) { return valve_group.updateMeasuredQuantityLeft( elapsed_time ); }

// IRRIGATION BY DURATION
void IrrigationSequence::setSequenceDurationSet( long duration ) { duration_set = duration; }
// void IrrigationSequence::setSequenceDurationLeft( long duration ) { duration_left = duration; }
// void IrrigationSequence::setSequenceNominalDurationLeft( long left ) { nominal_left.duration = left; }
// void IrrigationSequence::setSequenceMeasuredDurationLeft( long left ) { measured_left.duration = left; }
// void IrrigationSequence::setValveGroupDurationSet( long set ) { valve_group.setDurationSet( set ); }
// void IrrigationSequence::setValveGroupNominalDurationLeft( long left ) { valve_group.setNominalDurationLeft( left ); }
// void IrrigationSequence::setValveGroupMeasuredDurationLeft( long left ) { valve_group.setMeasuredDurationLeft( left ); }
// long IrrigationSequence::updateValveGroupDurationLeft( long elapsed_time ) { return valve_group.updateDurationLeft( elapsed_time ); }
// long IrrigationSequence::updateValveGroupNominalDurationLeft( long elapsed_time ) { return valve_group.updateNominalDurationLeft( elapsed_time ); }
// long IrrigationSequence::updateValveGroupMeasuredDurationLeft( long elapsed_time ) { return valve_group.updateMeasuredDurationLeft( elapsed_time ); }

// TIME STAMPS
void IrrigationSequence::setSequenceStartTime( long time ) { sequence_start_time = time; }
void IrrigationSequence::setSequenceEndTime( long time ) { sequence_end_time = time; }
void IrrigationSequence::setSequenceTimeStamp( long time ) { sequence_time_stamp = time; }
void IrrigationSequence::setSequenceTimeStampAndState( long time, IrrigationSequenceState state )
{
	this->state = state;		// setSequenceState( state );
	sequence_time_stamp = time;	// setSequenceTimeStamp( time );
}

void IrrigationSequence::setCycleStartTime( long time ) { cycle_start_time = time; }
void IrrigationSequence::setCycleEndTime( long time ) { cycle_end_time = time; }
void IrrigationSequence::setCycleTimeStamp( long time ) { cycle_time_stamp = time; }

void IrrigationSequence::setValveGroupStartTime( long time ) { valve_group.setStartTime( time ); }
void IrrigationSequence::setValveGroupEndTime( long time ) { valve_group.setEndTime( time ); }
void IrrigationSequence::setValveGroupTimeStamp( long time ) { valve_group.setTimeStamp( time ); }

void IrrigationSequence::setIntegralCalculationTimeStamp( long time )  { integral_calculation_time_stamp = time; }

// WATER FLOW
// Not needed here for now

// Note: Irrigation Sequence incorporates some of its Irrigation Program properties
bool IrrigationSequence::initSequence( )
{  
	bool initialized = false, db_error = false;

	// FOR DEBUGGING PURPOSES:
	// int valve_group_size = sizeof(ValveGroup);					// 2208
	// int db_program_size = sizeof(DBIrrigationProgram);			// 272
	// int db_irrigation_sequence_size = sizeof(DBValveSequence);	// 211

	// Get the memory slot for temporarily storing the Irrigation Program record
	DBIrrigationProgram * db_program = (DBIrrigationProgram *) DB_GetMemorySlot(task_id);

	// Retrieve Irrigation Program record from the database that has the info about the Irrigation Program type
	// Each Irrigation Cycle is viewed as an atomic unit. Once it starts, it runs until all cycles are completed.
	// Hence, sequences can run one after another until the last one completes, unless interrupted by the Irrigation Process Scheduler
	if ( DB_GetRecord( DBIRRIGATIONPROGRAM_TYPE, task_id, program_id, db_program ) )
	{
		// SET APPLICABLE IRRIGATION PROGRAM STATES

		// The following properties are common to all types of Irrigation Programs:
		setIrrigationMethod( db_program->irrigation_method );
		program_start_date.Initialize( db_program->start_date );
		program_end_date.Initialize( db_program->end_date );
		program_start_time = convertTimeToMilliseconds( db_program->start_time );
		program_end_time = convertTimeToMilliseconds( db_program->end_time );
		cycle_delay = convertTimeToMilliseconds( db_program->delay );
		
		// The following properties are specific to the Irrigation Method type:
		switch ( irrigation_method )
		{
			case IRRIGATION_METHOD_CYCLES:

				setSequenceNumberCyclesElapsed( 0 );
				setSequenceNumberCyclesSet( db_program->specific_properties.cycles.number_of_cycles );
				setSequenceNumberCyclesLeft( db_program->specific_properties.cycles.number_of_cycles );
				break;

			case IRRIGATION_METHOD_SENSOR:

				setProgramSensorId( db_program->specific_properties.sensor.sensor_id );
				setProgramSensorType( db_program->specific_properties.sensor.sensor_type );
				max_rest_time = convertTimeToMilliseconds( db_program->specific_properties.sensor.max_rest );

				if ( ( sensor_type == SENSOR_TYPE_VPD_SUM ) || ( sensor_type == SENSOR_TYPE_RAD_SUM ) )
					setProgramSetPoint( db_program->specific_properties.sensor.condition.set_point );
				else // sensor_type == SENSOR_TYPE_CONDITIONS
					setProgramConditionId( db_program->specific_properties.sensor.condition.condition_id );	

				break;
				
			case IRRIGATION_METHOD_ET:

				setProgramSensorId( db_program->specific_properties.et.sensor_id );
				setProgramCropFactor( db_program->specific_properties.et.crop_factor );
				setSequenceNumberCyclesSet( db_program->specific_properties.et.number_of_cycles );
				setSequenceNumberCyclesLeft( db_program->specific_properties.et.number_of_cycles );
				break;

			default: // IRRIGATION_METHOD_LOOP:

				// nothing

				break;
		}
	}
	else
	{
		db_error = true;
	}

	// Get the memory slot for temporarily storing the Valve Sequence record
	DBValveSequence * db_sequence = (DBValveSequence *) DB_GetMemorySlot(task_id);

	// Retrieve Irrigation Sequence record from the database
	if ( DB_GetRecord( DBVALVESEQUENCE_TYPE, task_id, sequence_id, db_sequence ) )
	{
		setManagerRequest( db_sequence->request );
		setAlarmId( db_sequence->alarm_id );
		setIrrigationType( db_sequence->irrigation_type );
		setNumberValves( db_sequence->number_of_valves );
		setSequenceIrrigationSet( db_sequence->set );
		setSequenceIrrigationLeft( db_sequence->set ); // TO DO: May not be necessary!
	}
	else
	{
		db_error = true;
	}

	if ( db_error ) 
	{
		// Retrieve the last Database error code associated with the task id
		error_code = DB_GetLastErrorCode( task_id );
	}

	// Reset running instance properties
	accumulated_set_point = 0;
	number_of_cycles_elapsed = 0;
	// flow_rate_liters_per_second = 0;
	// flow_rate_cubic_meters_per_hour = 0;

	// CONSTRUCT HYPOTHETICAL VALVE GROUPS

	// Reset tracking variable
	group_number = 1;
	
	// Iterate over all the Irrigation Valves in the sequence; Range for valve numbers: [0 - 99],  Range for valve ids: [1 - 255]
	for ( int i = 0, j = 0; i < number_of_valves; i++ )
	{
		if ( db_sequence->valves[i] != 0 )	
		{
			Trinity[j].valve_id = db_sequence->valves[i];
			Trinity[j].group_number = group_number;
			j++;	// Note: At the end of the for loop j must be equal to the number_of_valves in the sequence

			// Group valves within the sequence
			// The sign bit value "0" denotes "+", which means there are more valves in the group
			// The sign bit value "1" denotes ",", which means there are no more valves in the group
			if ( (bool) GetBit( db_sequence->signs, i ) )
				group_number++;
		}
	}

	// Retrieve and set Valve Sequence Dosing Station Id; 0 means the Dosing Station is absent
	// Note: Though the number of Irrigation Valves might vary from Valve Group to Valve Group,
	// the Dosing Station will remain the same for all Valve Groups since it is set per Irrigation Sequence
	retrieveDosingStationId( Trinity[0].valve_id );
	
	// Note: Do not change the order of the following statements
	number_of_groups = group_number - 1;
	group_number = 1;
	valve_group.initGroup( task_id, number_of_groups, irrigation_type, dosing_station_id );

	if ( !db_error )
		initialized = true;

	return initialized;
}

bool IrrigationSequence::constructValveGroup( )
{
	DBIndex main_line_id, main_valve_id;
	bool previous = true, constructed = false, db_error = false;
	
	valve_group.resetGroup( group_number );
	
	// Consruct ONE Valve Group that is part of ONLY ONE Irrigation Valve Sequence at a time
	for ( int i = 0; i < number_of_valves; ++i )
	{
		// Check to see if this Irrigation Valve appears in this Valve Group 
		if ( Trinity[i].group_number != MAX_GROUPS_PER_SEQUENCE && Trinity[i].group_number == group_number ) 
		{
			// The following function first checks to see if the Irrigation Valve is NOT 
			// already in the Valve Group (left over from the previous Valve Group) 
			// and ONLY then adds it to the Valve Group
			valve_group.addIrrigationValveToTheCurrentValveGroup( Trinity[i].valve_id );

			// Determine the value stored in the Database in the Id field of the Main Valve which opens the Main Line 
			// Since a Valve Group can be spread over more than one Main Line, 
			// the algorithm for determining which Main Valves to open is as follows:
			// For each Irrigation Valve in the Valve Group find the Main Line it is "sitting" on;
			// For each Main Line find the Main Valve (if there is one)	that is installed on that Main Line
			// Note that there can be no Main Valve installed on the Main Line
			// The same algorithm applies to finding the Dosing Station Id
			
			if ( DB_GetField(DBIRRIGATIONVALVE_TYPE, task_id, Trinity[i].valve_id, DBIRRIGATIONVALVE_LINE_ID, &main_line_id) )
			{
				// if ( DB_Exist(DBMAINLINE_TYPE, task_id, main_valve_id )
				// bool DB_Exist(DBType type, DBTask reader, DBIndex index);
				if ( DB_GetField(DBMAINLINE_TYPE, task_id, main_line_id, DBMAINLINE_MAIN_VALVE_ID, &main_valve_id) )
				{
					// Check to see if there is a Main Valve installed on the Main Line; Valid Main Valve Id range [1 - 10]
					if ( main_valve_id >= MIN_INDEX && main_valve_id <= MAX_MAIN_VALVES )
					{
						// The following function first checks to see if the Main Valve is NOT already in the Valve Group 
						// (either left over from the previous Valve Group or has already been added in this Valve Group) 
						// and ONLY then adds it to the current Valve Group
						valve_group.addMainValveToTheCurrentValveGroup( main_valve_id );
					}
				}
				else
					db_error = true;
			}
			else
				db_error = true;

			if ( db_error ) 
				error_code = DB_GetLastErrorCode( task_id );	

		} // end if ( Trinity[i].group_number != MAX_GROUPS_PER_SEQUENCE && Trinity[i].group_number == group_number ) 

		// Save Irrigation Valve Ids and Main Valve Ids that appear in the next Valve Group (if there is one)
		// There is a need to determine which Main Valves and Irrigation Valves will be operational in the next Valve Group 
		// because only the Main/Irrigation Valves that are NOT shared by the adjacent Valve Groups must be closed
		if ( Trinity[i].group_number != MAX_GROUPS_PER_SEQUENCE && Trinity[i].group_number == group_number + 1 )
		{
			valve_group.addIrrigationValveToTheNextValveGroup( Trinity[i].valve_id );

			if ( DB_GetField(DBIRRIGATIONVALVE_TYPE, task_id, Trinity[i].valve_id, DBIRRIGATIONVALVE_LINE_ID, &main_line_id) )
			{
				if ( DB_GetField(DBMAINLINE_TYPE, task_id, main_line_id, DBMAINLINE_MAIN_VALVE_ID, &main_valve_id) )
				{
					valve_group.addMainValveToTheNextValveGroup( main_valve_id );
				}
			}
		}

	} // end for

	valve_group.setIrrigationSet( set );
	// valve_group.setIrrigationLeft( left );

	// Update Total Irrigation Area, Total Nominal Flow, apply Delays and register Water Meters
	valve_group.updateGroup( );
	group_number++;

	if ( !db_error )
		constructed = true;

	return constructed;
}

// Starts the Water Pumps
bool IrrigationSequence::startPumps( )
{
	// TO DO: Develop Pump Manager which will have an algorithm for selecting the right number of pumps and for starting them
	// Valve Sequence has Dosing Station Id - INDEX dosing_station_id;
	// Dosing Station has Main Line Id and Pump Station Id - INDEX main_line_id and INDEX pump_station_id:
	// Currently Pump Station scrip does NOT include Pumps

	// bool pre_state, post_state, operation_status = true;
	// pre_state = valve_group.getState( );
	// valve_group.startPumps( );
	// post_state = valve_group.getState( );
	//if ( pre_state != post_state )
	//	operation_status = true;
	//else
	//	operation_status = false;

	// true means that Pumps started; false means that the Pumps failed to start
	return valve_group.startPumps( );
}

// Stops the Water Pumps
bool IrrigationSequence::stopPumps( )
{
	// bool pre_state, post_state, operation_status = true;
	// pre_state = valve_group.getState( );
	// valve_group.stopPumps( );
	// post_state = valve_group.getState( );
	//if ( pre_state != post_state )
	//	operation_status = true;
	//else
	//	operation_status = false;

	// true means that the Pumps stopped; false means that Pumps failed to stop
	return valve_group.stopPumps( );
}

// Opens Main Valve
bool IrrigationSequence::openMainValves( )
{
	// bool pre_state, post_state, operation_status = true;
	// pre_state = valve_group.getState( );
	// valve_group.openMainValves( );
	// post_state = valve_group.getState( );
	//if ( pre_state != post_state )
	//	operation_status = true;
	//else
	//	operation_status = false;

	// true means that all Main Valves opened; false means that not all Main Valves opened
	return valve_group.openMainValves( );
}
		
// Closes Main Valve
bool IrrigationSequence::closeMainValves( )
{
	// bool pre_state, post_state, operation_status = true;
	// pre_state = valve_group.getState( );
	// valve_group.closeMainValves( );
	// post_state = valve_group.getState( );
	//if ( pre_state != post_state )
	//	operation_status = true;
	//else
	//	operation_status = false;

	// true means that all Main Valves closed; false means that not all Main Valves closed
	return valve_group.closeMainValves( );
}

// Opens Irrigation Valves
bool IrrigationSequence::openIrrigationValves( )
{
	bool pre_state, post_state, operation_status;

	pre_state = valve_group.getState( );
	valve_group.openIrrigationValves( );
	post_state = valve_group.getState( );

	if ( pre_state != post_state )
		operation_status = true;
	else
		operation_status = false;

	// false means that either the Irrigation Valves were already open, or they failed to open
	return operation_status;
}

// Closes Irrigation Valves
bool IrrigationSequence::closeIrrigationValves( )
{
	bool pre_state, post_state, operation_status;

	pre_state = valve_group.getState( );
	valve_group.closeIrrigationValves( );
	post_state = valve_group.getState( );

	if ( pre_state != post_state )
		operation_status = true;
	else
		operation_status = false;

	// false means that either the Irrigation Valves were already closed, or they failed to close
	return operation_status;
}

void IrrigationSequence::processWaterMeterMessage( const DB_LISTENER_MESSAGE_P db_message ) 
{ 
	valve_group.processWaterMeterMessage( db_message );
}

IrrigationSequenceState IrrigationSequence::processDatabaseMessages( IrrigationSequenceState _state )
{
	IrrigationSequenceState state = _state;

	// Loop over the messages in the Message Box until all messages have been cleared from it
	for ( DB_LISTENER_MESSAGE_P db_message = message_box.GetMessage( ); db_message != NULL; db_message = message_box.GetNextMessage( ) )
	{
		//typedef struct 
		//{
		//	DBType type;
		//	DBIndex index;										Mission: Save unique messages based on the index field value
		//	DBColumn column;									Question: What happenes when more than one column has been changed ????
		//	DBOperation operation;
		//} DB_LISTENER_MESSAGE_T, *DB_LISTENER_MESSAGE_P;

		// Check to see if any of the record's fields have been changed
		if ( db_message->operation == RECORD_CHANGE )
		{		
			// Select the table type
			DBType table_type = db_message->type;
			switch ( table_type )
			{
				case DBWATERMETER_TYPE:

					// Add the Water Meter message to the respective Water Meter object in the active Valve Group
					processWaterMeterMessage( db_message );

					// Note: Processing Water Meter messages as soon as they arrive is not feasible in the current design 
					// since the logic that oversees the timing of the arrival of messages from Water Meters is incorporated
					// at the Valve Group level (lower level) and the arrival of the messages is registered at the Irrigation 
					// Sequence level (upper level)

					break;

				case DBVALVESEQUENCE_TYPE:
					
					// Process message intended for the Valve Sequence
					state = processValveSequenceMessage( db_message );
					break;

				case DBIRRIGATIONPROGRAM_TYPE:

					// Process message from the Irrigation Program
					// THIS CASE IS RESERVED FOR POSSIBLE FUTURE USE
					break;
		
					default:
			}	// end of switch ( table_type )

		}	
		else  // if ( db_message.operation == RECORD_CHANGE ) - end checking to see if any of the record's fields have been changed
		{
			// CONTINUE SEQUENCE OPERATIONS AS USUAL
		}
	
	}; // end of FOR loop that checks to see if there were new message waiting to be read

	return state;
}


IrrigationSequenceState IrrigationSequence::processValveSequenceMessage( const DB_LISTENER_MESSAGE_P db_message )
{
	IIrrigationSetLeft set, left;
	IrrigationSequenceState state;
	IrrigationManagerRequest request, previous_request;

	// Check if a single relevant record field has been updated
	if ( db_message->column != MULTIPLE_COLUMNS )
	{
		switch ( db_message->column )
		{
			// DBVALVESEQUENCE_REQUEST, DBVALVESEQUENCE_RESPONSE
			case DBVALVESEQUENCE_REQUEST:
						
				DB_GetField( db_message->type, task_id, db_message->index, DBVALVESEQUENCE_REQUEST, &request );
				state = getIrrigationSequenceResponse( request );
				break;

				// TO DO: Develop algorithm for each case
				// How do I stall/revive the system? 
				// Which devices should be deactivated/activated, such as pumps, main valves, irrigation valves and in what order? 
				// Are there any timeouts for staying in the PAUSED and PREEMPTED states? 

			case DBVALVESEQUENCE_SET:
						
				DB_GetField( db_message->type, task_id, db_message->index, DBVALVESEQUENCE_SET, &set );
				setSequenceIrrigationSet( set );
				break;
					
			case DBVALVESEQUENCE_LEFT:
						
				DB_GetField( db_message->type, task_id, db_message->index, DBVALVESEQUENCE_LEFT, &left );
				setSequenceIrrigationLeft( left );
				break;

			default:
		} // end of switch ( db_message.column )
	}
	else 
	{
		// TO DO: Might be a source of tricky bugs !!!

		// Check all three record fields
		DB_GetField( db_message->type, task_id, db_message->index, DBVALVESEQUENCE_REQUEST, &request );
		DB_GetField( db_message->type, task_id, db_message->index, DBVALVESEQUENCE_SET, &set );
		DB_GetField( db_message->type, task_id, db_message->index, DBVALVESEQUENCE_LEFT, &left );

		state = getIrrigationSequenceResponse( request );
		
		setSequenceIrrigationSet( set );
		setSequenceIrrigationLeft( left );

	} // end if else ( db_message.column != MULTIPLE_COLUMNS )

	return state;
}


IrrigationSequenceState IrrigationSequence::getIrrigationSequenceResponse( IrrigationManagerRequest request )
{
	IrrigationSequenceState state;

	switch ( request )
	{
		case PAUSE_IRRIGATION: state = SEQUENCE_PAUSE; break;
		case RESUME_IRRIGATION: state = SEQUENCE_RESUME; break;
		case PREEMPT_IRRIGATION: state = SEQUENCE_PREEMPT; break;
		case ABORT_IRRIGATION: state = SEQUENCE_ABORT; break;
		case START_ONE_CYCLE_IRRIGATION: state = SEQUENCE_RUN_ONE_CYCLE_ONLY; break;
	}

	return state;
}

long IrrigationSequence::convertTimeToMilliseconds( DBTime time )
{
	return ( time.hour * 3600000 + time.minute * 60000 + time.second * 1000 );
}

void IrrigationSequence::retrieveDosingStationId( DBIndex valve_id )
{ 
	// Each Irrigation Valve is physically connected to ONLY one Main Line
	// Each Main Line may or may not have a Main Valve
	// Each Main Line is physically connected to ONLY one Dosing Station
	// Each Dosing Station can be physically connected to more than one Main Line

	bool db_error = false;
	DBIndex main_line_id = 0;

	// Retrieve Dosing Station Id
	if ( DB_GetField(DBIRRIGATIONVALVE_TYPE, task_id, valve_id, DBIRRIGATIONVALVE_LINE_ID, &main_line_id) )
	{
		if ( DB_GetField(DBMAINLINE_TYPE, task_id, main_line_id, DBMAINLINE_DOSING_STATION_ID, &dosing_station_id) )
		{
			// do nothing
		}
		else
			db_error = true;
	}
	else
		db_error = true;

	if ( db_error ) 
		error_code = DB_GetLastErrorCode( task_id );
}

//void IrrigationSequence::setSequence( const IrrigationSequence & sequence )
//{
//	if ( this != &sequence )    
//	{
//		setTaskId( sequence.task_id );
//		setProgramId( sequence.program_id );
//		setSequenceId( sequence.sequence_id );
//		setDosingStationId( sequence.dosing_station_id );
//		setProgramSensorId( sequence.sensor_id );
//		setProgramConditionId( sequence.condition_id );
//		setAlarmId( sequence.alarm_id );
//		
//		// IRRIGATION PROGRAM PROPERTIES
//		setProgramStartDate( sequence.program_start_date );
//		setProgramEndDate( sequence.program_end_date);
//		setProgramStartTime( sequence.program_start_time );
//		setProgramEndTime( sequence.program_end_time );
//		setCycleDelay( sequence.cycle_delay );
//		setProgramMaxRestTime( sequence.max_rest_time );
//		setSequenceNumberCyclesSet( sequence.number_cycles_set );
//		setSequenceNumberCyclesElapsed( sequence.number_of_cycles_elapsed );
//		setSequenceNumberCyclesLeft( sequence.number_cycles_left );
//		setProgramSensorType( sequence.sensor_type );
//		setProgramSetPoint( sequence.set_point );
//		setAccumulatedSetPoint( sequence.accumulated_set_point );
//		setProgramCropFactor( sequence.crop_factor );
//
//		// VALVE (IRRIGATION) SEQUENCE PROPERTIES
//		setSequenceState( sequence.state );
//		setManagerRequest( sequence.request );	
//		setSequenceResponse( sequence.response );
//		setIrrigationMethod( sequence.irrigation_method );
//		setIrrigationType( sequence.irrigation_type );
//		// setFertigationRecipe( sequence.recipe );
//		setGroupNumber( sequence.group_number );
//		setNumberGroups( sequence.number_of_groups );
//		setNumberValves( sequence.number_of_valves );
//		setValveGroup( sequence.valve_group );
//		setMessageBox( sequence.message_box );
//
//		// IRRIGATION BY (DEPTH, QUANTITY, DURATION)
//		setSequenceIrrigationSet( sequence.set );
//		setSequenceIrrigationLeft( sequence.left );
//		// setSequenceNominalIrrigationLeft( sequence.nominal_left );
//		// setSequenceMeasuredIrrigationLeft( sequence.measured_left );
//
//		// TIMESTAMPS
//		setSequenceStartTime( sequence.sequence_start_time );
//		setSequenceEndTime( sequence.sequence_end_time );
//		setSequenceTimeStamp( sequence.sequence_time_stamp );
//
//		setCycleStartTime( sequence.cycle_start_time );
//		setCycleEndTime( sequence.cycle_end_time );
//		setCycleTimeStamp( sequence.cycle_time_stamp );
//
//		setValveGroupStartTime( sequence.getValveGroupStartTime( ) );
//		setValveGroupEndTime( sequence.getValveGroupEndTime( ) );
//		setValveGroupTimeStamp( sequence.getValveGroupTimeStamp( ) );
//
//		for( int v = 0; v < MAX_VALVES_PER_SEQUENCE; ++v )
//		{
//			Trinity[v].valve_id = sequence.Trinity[v].valve_id;	
//			Trinity[v].group_number = sequence.Trinity[v].group_number;
//		}
//	}
//}


// IRRIGATION BY DEPTH
// setSequenceDepthSet( db_sequence.set.depth );
// setSequenceNominalDepthLeft( db_sequence.nominal_left.depth );
// setSequenceMeasuredDepthLeft( db_sequence.measured_left.depth );

// IRRIGATION BY QUANTITY
// setSequenceQuantitySet( db_sequence.set.quantity );
// setSequenceNominalQuantityLeft( db_sequence.nominal_left.quantity );
// setSequenceMeasuredQuantityLeft( db_sequence.measured_left.quantity = left );

// IRRIGATION BY DURATION
// setSequenceDurationSet( db_sequence.set.duration )
// setSequenceNominalDurationLeft( db_sequence.nominal_left.duration );
// setSequenceMeasuredDurationLeft( db_sequence.measured_left.duration );

// setSequenceIrrigationSet( db_sequence.set );
// setSequenceIrrigationLeft( db_sequence.set );

//switch ( irrigation_type )
//{
//	case IRRIGATION_BY_DEPTH:		// Measured in mm

//		setSequenceDepthSet( db_sequence.set.depth );
//		setSequenceDepthLeft( db_sequence.set.depth );
//		break;

//	case IRRIGATION_BY_QUANTITY:	// Measured in m3

//		setSequenceQuantitySet( db_sequence.set.quantity );
//		setSequenceQuantityLeft( db_sequence.set.quantity );

//		// AT THIS POINT IT IS IMPOSSIBLE TO ESTIMATE THE IRRIGATION DURATION BASED ON THE 
//		// IRRIGATION WATER QUANTITY BECAUSE THE IRRIGATION DURATION VARIES FROM GROUP TO GROUP 
//		// BASED ON THE NUMBER OF IRRIGATION VALVES IN THE GROUP AND THEIR RESPECTIVE IRRIGATION 
//		// AREAS AND NOMINAL FLOWS

//		break;

//	case IRRIGATION_BY_DURATION:	// Measured in hr:mm:ss

//		setSequenceDurationSet( db_sequence.set.duration );
//		setSequenceDurationLeft( db_sequence.set.duration );
//		break;	
//
//	default:
//}


//switch ( irrigation_type )
//{
//	case IRRIGATION_BY_DEPTH:		// Measured in mm

//		setSequenceDepthSet( sequence.set.depth );		
//		setSequenceDepthLeft( sequence.left.depth );
//		break;

//	case IRRIGATION_BY_QUANTITY:	// Measured in m3

//		setSequenceQuantitySet( sequence.set.quantity );
//		setSequenceQuantityLeft( sequence.left.quantity );
//		break;

//	case IRRIGATION_BY_DURATION:	// Measured in hr:mm:ss

//		setSequenceDurationSet( sequence.set.duration );
//		setSequenceDurationLeft( sequence.left.duration );
//		break;

//	default:
//}

// IRRIGATION BY DEPTH
// setSequenceDepthSet( sequence.set.depth );
// setSequenceNominalDepthLeft( sequence.nominal_left.depth );
// setSequenceMeasuredDepthLeft( sequence.measured_left.depth );

// IRRIGATION BY QUANTITY
// setSequenceQuantitySet( sequence.set.quantity );
// setSequenceNominalQuantityLeft( sequence.nominal_left.quantity );
// setSequenceMeasuredQuantityLeft( sequence.measured_left.quantity );

// IRRIGATION BY DURATION
// setSequenceDurationSet( sequence.set.duration )
// setSequenceNominalDurationLeft( sequence.nominal_left.duration );
// setSequenceMeasuredDurationLeft( sequence.measured_left.duration );