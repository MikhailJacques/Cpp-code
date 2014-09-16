
// Irrigation Process Implementation file

#include "DBDateTime.h"
#include "MessageBox.h"
#include "IrrigationProcess.h"
#include "IrrigationSequence.h"

#ifdef IRRIGATION_PROCESS_DEBUG
   #define DebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId) SendDebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId);
#else
   #define DebugOutputRequest(DebugStringId, DebugStringValue, TaskId, DeviceId_OR_CycleId) /**/;
#endif

// Global arrays of indices that is stored in the ProcessExecution_Manager.c file
// extern DBIndex IrrigationProgramID[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
extern DBIndex IrrigationSequenceID[MAX_CONCURRENT_IRRIGATION_SEQUENCES];

// Allocate global memory sufficient enough to store maximum number of Irrigation Sequences
static byte sequences[MAX_CONCURRENT_IRRIGATION_SEQUENCES][sizeof(IrrigationSequence)];

// Allocate global memory sufficient enough to store MAX_CONCURRENT_IRRIGATION_SEQUENCES MessageBox objects
// static // byte msgBoxes[MAX_CONCURRENT_IRRIGATION_SEQUENCES][sizeof(MessageBox)];

static bool error[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static WORD error_code[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static SensorType sensor_type[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
// static IrrigationSequence * sequence[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static IrrigationType irrigation_type[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static IrrigationMethod irrigation_method[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static IrrigationManagerRequest current_request[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static IrrigationManagerRequest previous_request[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static IrrigationSequenceResponse response[MAX_CONCURRENT_IRRIGATION_SEQUENCES];

static const float VPD_coefficient = 0.61088;
static const long one_hundred_milliseconds = MILLISECONDS_IN_SECOND / 10;		
static const long one_thousand_milliseconds = MILLISECONDS_IN_SECOND;		
static const long water_pumps_timeout = MILLISECONDS_IN_SECOND;			
static const long main_valves_timeout = MILLISECONDS_IN_SECOND;			
static const long irrigation_valves_timeout = MILLISECONDS_IN_SECOND;

static DBDatePP present_date[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static long current_time[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static long previous_time[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static long elapsed_time[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static long previous_cycle_end_time[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static long cycle_delay[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static long max_rest_time[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static long duration_left[MAX_CONCURRENT_IRRIGATION_SEQUENCES];

// static IIrrigationSetLeft set[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static IIrrigationSetLeft left[MAX_CONCURRENT_IRRIGATION_SEQUENCES];

static float eo[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static float ea[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
// static float program_set_point[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
// static float accumulated_set_point[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static float humidity[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static float temperature[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static float temperature_coefficient[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
// static float vpd_sum_counter[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
// static float rad_sum_counter[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static float flow_rate_liters_per_second[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static float flow_rate_cubic_meters_per_hour[MAX_CONCURRENT_IRRIGATION_SEQUENCES];

// static int crop_factor[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
// static DBIndex sensor_id[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
// static DBIndex condition_id[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
static byte current_cycle_number[MAX_CONCURRENT_IRRIGATION_SEQUENCES];
// static byte number_of_cycles_left[MAX_CONCURRENT_IRRIGATION_SEQUENCES];

extern "C"
{
	#include "extern.h"

IrrigationSequenceState Irrigation_Sequence_Handler( DBTask task_id, IrrigationSequenceState state )
{
	// FOR DEBUGGING PURPOSES
	// int sequence_size = sizeof(IrrigationSequence);	// 2696

	IrrigationSequence * sequence;

	// Sequence number is a Valve (Irrigation) Sequence number; Range [0 - 9]
	const int sequence_number = task_id - MLINE1_SLOT;

	// Sequence Id is an index of the Valve (Irrigation) Sequence record in the Database; Range [0 - 9]
 	const DBIndex sequence_id = IrrigationSequenceID[sequence_number];

	DBTimePP time;

	// Check to see if the sequence has NOT been initialized yet
	if ( state == SEQUENCE_UNINITIALIZED )
	{
		// Instantiate an Irrigation Sequence object
		sequence = new (sequences[sequence_number]) IrrigationSequence( task_id );
		
		// Instantiate sequence Message Box
		// msgBox = new (msgBoxes[sequence_number]) MessageBox( sequence->getMessageBoxNumber( ) );

		// Initialize the Irrigation Sequence object by retrieving the sequence data from the Database. It occurs only once.
		if ( sequence->initSequence( ) )
		{
			state = SEQUENCE_INITIALIZED;
			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			DebugOutputRequest( 5, SEQUENCE_INITIALIZED, task_id, ( sequence->getSequenceNumberCyclesElapsed( ) + 1 ) );
		}
		else
		{
			// TO DO: Develop algorithm for shutting the system down and notifying the Manager !!!
			state = SEQUENCE_FAILURE;
		}
	}
	else	// Retrieve the sequence from memory because the sequence has already been initialized with sequence data from
			// the Database and is already in one of the Irrigation Sequence states other than the SEQUENCE_UNINITIALIZED state
	{
		sequence = (IrrigationSequence *) sequences[sequence_number];
		// msgBox = reinterpret_cast<MessageBox *>( msgBoxes[sequence_number] );
	}

	// PROCESS MAIL MESSAGES
	// Invoke the function that checks to see if there are new messages in the message box waiting to be read
	// Should there be new messages, it processes them accordingly and updates the Irrigation Process state if necessary
	// Note: Water Meter pulses should NOT arrive more than twice a second
	state = sequence->processDatabaseMessages( state );

	// TO DO: For each Irrigation Process state develop algorithms for contingency situations, such as for example:
	// Pumps failed to start/stop, Main Valves or Irrigation Valves failed to open/close, etc.
	// and incorporate checking for timeouts so as to NOT block the Irrigation Process
	// When a timeout expires and the device failed to operate, madvance Irrigation Process state to PROGRAM_FAILURE

	// BASED ON THE IRRIGATION PROCESS's CURRENT STATE PERFORM THE APPROPRIATE OPERATIONS
	switch ( state )
	{
		// This state occurs ONLY once for each sequence
		case SEQUENCE_RUN_ONE_CYCLE_ONLY:

			state = SEQUENCE_INITIALIZED;
			sequence->setSequenceNumberCyclesSet( 1 );
			
		// This state occurs ONLY once for each sequence
		case SEQUENCE_INITIALIZED:

			state = SEQUENCE_CHECK_FOR_NEXT_CYCLE;
			cycle_delay[sequence_id] = 0;
			previous_cycle_end_time[sequence_id] = get_current_time(IN_MILLISECONDS);
			sequence->setIntegralCalculationTimeStamp( previous_cycle_end_time[sequence_id] );
			sequence->setSequenceStartTime( previous_cycle_end_time[sequence_id] );

			// The trigger to start the next irrigation cycle has NOT been reached yet
			response[sequence_id] = SEQUENCE_STATUS_WAITING;
			DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_RESPONSE, &response[sequence_id] );
			DebugOutputRequest( 5, SEQUENCE_CHECK_FOR_NEXT_CYCLE, task_id, sequence->getSequenceNumberCyclesElapsed( ) );

		// This state occurs once per each execution of each irrigation sequence cycle
		case SEQUENCE_CHECK_FOR_NEXT_CYCLE:

			// Check to see if it is the first cycle in this sequence
			if ( sequence->getSequenceNumberCyclesElapsed( ) == 0 )
			{
				// do nothing
				// cycle_delay[sequence_id] = 0;
				// previous_cycle_end_time[sequence_id] = get_current_time(IN_MILLISECONDS);
			}
			else
			{
				// These two apply to any subsequent cycles, but the first one
				cycle_delay[sequence_id] = sequence->getCycleDelay( );
				previous_cycle_end_time[sequence_id] = sequence->getCycleEndTime( );
			}

			// TO DO: Find out if the algorithm should check whether the irrigation is due today (recurrence) !!!
			present_date[sequence_id] = DB_DateNow( );
			current_time[sequence_id] = get_current_time(IN_MILLISECONDS);

			irrigation_method[sequence_id] = sequence->getIrrigationMethod( );
			switch ( irrigation_method[sequence_id] )
			{
				case IRRIGATION_METHOD_CYCLES:

					// VERIFY THIS WITH THE MANAGEMENT !!!
					// Check to see if it is time to finish the Irrigation program
					//if ( present_date >= sequence->getProgramEndDate( ) && current_time[sequence_id] >= sequence->getProgramEndTime( ) )
					//{
					//	state = SEQUENCE_COMPLETE_PROGRAM;
					//	break;
					//}

					// AT THIS POINT IN CODE THE IRRIGATION PROGRAM IS STILL WITHIN THE TIME WINDOW OF RUNNING OPPORTUNITY

					// Check to see if there are Irrigation Cycles left to irrigate
					if ( sequence->getSequenceNumberCyclesLeft( ) > 0 )
					{
						// Check to see if the delay to start the next irrigation cycle has expired
						if ( time_interval_expired_in_milliseconds( previous_cycle_end_time[sequence_id], cycle_delay[sequence_id] ) )
						{
							state = SEQUENCE_COMMENCE_CYCLE;
							sequence->decrementSequenceNumberCyclesLeft( );
							sequence->incrementSequenceNumberCyclesElapsed( );
						}
					}
					else	// No more cycles left to irrigate
					{
						state = SEQUENCE_COMPLETE_PROGRAM;
					}
					break;

				case IRRIGATION_METHOD_LOOP:

					// Check to see if the delay to start the next irrigation cycle has expired
					if ( time_interval_expired_in_milliseconds( previous_cycle_end_time[sequence_id], cycle_delay[sequence_id] ) )
					{
						state = SEQUENCE_COMMENCE_CYCLE;
						sequence->incrementSequenceNumberCyclesElapsed( );
					}

					// AT THIS POINT IN CODE THE IRRIGATION PROGRAM IS STILL WITHIN THE TIME WINDOW OF RUNNING OPPORTUNITY

					// Check to see if it is time to finish the Loop Irrigation program
					if ( present_date[sequence_id] >= sequence->getProgramEndDate( ) && current_time[sequence_id] >= sequence->getProgramEndTime( ) )
					{
						state = SEQUENCE_COMPLETE_PROGRAM;
					}

					// AT THIS POINT IN CODE THE IRRIGATION PROGRAM MIGHT NO LONGER BE WITHIN THE TIME WINDOW OF RUNNING OPPORTUNITY
					// AND MIGHT VERY WELL BE IN THE SEQUENCE_COMPLETE_PROGRAM STATE

					break;

				case IRRIGATION_METHOD_SENSOR:

					// AT THIS POINT IN CODE THE DELAY TO COMMENCE THE NEXT IRRIGATION CYCLE HAS EXPIRED
						
					// In order for next irrigation cycle to commence either the set point must be reached or
					// the maximum resting time must expire

					// Check to see if the set point has been reached
					sensor_type[sequence_id] = sequence->getProgramSensorType( );
					switch ( sensor_type[sequence_id] )
					{
						case SENSOR_TYPE_VPD_SUM:
									
							// The VPD Sum Limit is used as a trigger for starting the next cycle of irrigation
							// The irrigation according to the VPD Sum trigger is available only during the 'Day time' as defined by the User
							// The system starts accumulating the VPD Sum counters, according to the definition of – 'Start Day time for VPD Sum', 
							// and resets and stops accumulating according to the definition of 'End Day time for VPD Sum'. 
							// During the 'Night time' the VPD counters should be zero.
									
							// The VPD Sum is calculated according to the measured temperature and humidity
							// The VPD Sum is measured in Kpa
							// The VPD Sum is an integral of the VPD measurements
							// The calculation is based on the current measurement of the relative Humidity and the Temperature
							// and the time interval between two consecutive calculations

							// ALGORITHM:
							// Get the VPD Sum Limit
							// Get the time interval for the VPD Sum
							// Calculate the VPD Sum Counter
							// Compare the VPD Sum Counter to the VPD Sum Limit
							// If VPD Sum Counter >= VPD Sum Limit, then verify that the Cycle Delay has expired and then commence the next irrigation cycle
							// Once the irrigation cycle has started, reduce the VPD Sum Counter by 1 * VPD Sum Limit

							// EXAMPLE:
							// VPD Sum Limit = 400 [kPa]
							// Case 1: Cycle Delay has expired
							// VPD Sum Counter = 450
							// VPD Sum Counter = VPD Sum Counter - VPD Sum Limit = 450 - 400 = 50
							// Case 2: Max Rest Time has expired
							// VPD Sum Counter = 350
							// VPD Sum Counter = VPD Sum Counter - VPD Sum Limit = 350 - 400 = -50 = 0 (reset)

							// ASSUMPTIONS:
							// Temperature is measured in degrees Celcius
							// Humidity is measured in Percent
							// Time interval between two consecutive VPD Sum calculations is approximately 1 second

							// Calculate VPD Sum Counter using the following formula:
							// VPD = e^o - e^a
							// e^o (Temperature) = 0.61088 exp[(17.27 * Temperature) / (Temperature + 237.3)]
							// e^a = e^o * (Humidity % / 100)

							// TO DO:
							// Retrieve temperature, humidity and time interval from the Database when they become available !!!
							// sensor_id[sequence_id] = sequence->getProgramSensorId( );
							// DB_GetRecord(DBVPDSUM_TYPE, task_id, sequence_id, &db_vpd_sum_record );
							// DB_GetField(DBVPDSUM_TYPE, task_id, sequence_id, DBVPDSUM_TEMPERATURE, &temperature[sequence_id]);
							// DB_GetField(DBVPDSUM_TYPE, task_id, sequence_id, DBVPDSUM_HUMIDITY, &humidity[sequence_id]);
							// DB_GetField(DBVPDSUM_TYPE, task_id, sequence_id, DBVPDSUM_START_TIME, &start_time);
							// DB_GetField(DBVPDSUM_TYPE, task_id, sequence_id, DBVPDSUM_END_TIME, &end_time);

							previous_time[sequence_id] = sequence->getSequenceTimeStamp( );
							if ( ( cycle_delay[sequence_id] == 0 ) || ( time_interval_expired_in_milliseconds( previous_time[sequence_id], ( one_thousand_milliseconds * 10 ) ) ) )
							{
								// Temporary settings
								temperature[sequence_id] = 36;		// degrees Celcius
								humidity[sequence_id] = 50;			// percent

								temperature_coefficient[sequence_id] = (17.27 * temperature[sequence_id]) / (temperature[sequence_id] + 237.3);
								eo[sequence_id] = pow( VPD_coefficient, temperature_coefficient[sequence_id] );
								ea[sequence_id] = eo[sequence_id] * ( humidity[sequence_id] / 100 );
									
								float vpd_sum_counter = eo[sequence_id] - ea[sequence_id];	// 1.629480
								// vpd_sum_counter[sequence_id] = eo[sequence_id] - ea[sequence_id];

								// TO DO: The current accumulation is temporary because of the absence of the real one
								// It is implemented this way solely for testing purposes !!!
								// accumulated_set_point[sequence_id] = sequence->getAccumulatedSetPoint( );
								sequence->updateAccumulatedSetPoint( vpd_sum_counter );
									
								// sequence->setAccumulatedSetPoint( accumulated_set_point[sequence_id] );
								// accumulated_set_point[sequence_id] = sequence->getAccumulatedSetPoint( );
								// program_set_point[sequence_id] = sequence->getProgramSetPoint( );

								// TO DO: The two lines below are temporary assignment to temporary variables for testing purposes only !!!

								// vpd_sum_limit stores user-defined value of the VPD Sum Limit
								float vpd_sum_limit = sequence->getProgramSetPoint( );
									
								// vpd_sum_counter stores the accumulated values of the VPD Sum Counter
								vpd_sum_counter = sequence->getAccumulatedSetPoint( );
									
								// Check to see if the VPD Sum counter has been reached
								if ( vpd_sum_counter >= vpd_sum_limit )
								{
									// Check to see if the delay to start the next Irrigation Cycle has expired
									if ( time_interval_expired_in_milliseconds( previous_cycle_end_time[sequence_id], cycle_delay[sequence_id] ) )
									{
										state = SEQUENCE_COMMENCE_CYCLE;

										// Update VPD Sum Counter
										// TO DO: Verify by how much to reduce the accumulated set point !!!
										// Right now it reduces it by one set point value
										// However, it might very well be the case that before the cycle delay has expired the 
										// accumulated set point value has become many times more than the user defined set point value
										sequence->updateAccumulatedSetPoint( (-1) * vpd_sum_limit );
										// accumulated_set_point[sequence_id] -= vpd_sum_limit[sequence_id];
										// sequence->setAccumulatedSetPoint( vpd_sum_counter[sequence_id] );
										sequence->incrementSequenceNumberCyclesElapsed( );
									}
								} 
								else	// Check to see if the maximum resting time to start the next irrigation cycle has expired
								{
									max_rest_time[sequence_id] = sequence->getProgramMaxRestTime( );
									if ( time_interval_expired_in_milliseconds( previous_cycle_end_time[sequence_id], max_rest_time[sequence_id] ) )
									{
										state = SEQUENCE_COMMENCE_CYCLE;
										sequence->setAccumulatedSetPoint( 0 );
										sequence->incrementSequenceNumberCyclesElapsed( );
									}
								}

								sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
							}

							break;

						case SENSOR_TYPE_RAD_SUM:

							// Radiation sum is calculated from the energy flux multiplied by time:
							// 1,000 Measured Radiation (w/m2) x 3600 (seconds per hour) = 3,600,000 J/m2.
								
							// Radiation sum is usually expressed in Joul/cm2:
							// 3,600,000 J/m2 / 10,000 = 360 J/cm2.

							// In the NMC's case defining a resolution of 10 seconds is sufficient, meaning the radiation 
							// can be sampled every 10 seconds, the value can be multiplied by 10 and summed up:
							// Previous sum + ( current sample (w/m2) * 10 (sec) ) / 10,000 = new sum (J/cm2)

							previous_time[sequence_id] = sequence->getSequenceTimeStamp( );
							if ( ( cycle_delay[sequence_id] == 0 ) || ( time_interval_expired_in_milliseconds( previous_time[sequence_id], one_thousand_milliseconds ) ) )
							{
								// TO DO: Retrieve real current_radiation from the Database when it becomes available !!!
								float current_radiation = 300;
								// DB_GetField(DBRADIATION_TYPE, task_id, record_id, DBCURRENT_RADIATION, &current_radiation);
								
								// Check to see if the sensor supplies valid solar radiation readings
								if ( current_radiation != SENSOR_N_A ) 
								{
									// TO DO: Remove temporarily declared variable after testing is finished
									long last_integral_calculation_time_stamp = sequence->getIntegralCalculationTimeStamp( );
									current_time[sequence_id] = get_current_time(IN_MILLISECONDS);
									
									// Calculate the integral / 10000 (W/m2 --> J/cm2) and add it to the total
									// sequence->updateAccumulatedSetPoint( ( current_radiation * ( ( current_time[sequence_id] - last_integral_calculation_time_stamp ) / ( SECOND * 10 ) ) ) / 10000 );
									sequence->updateAccumulatedSetPoint( (  current_radiation * ( current_time[sequence_id] - last_integral_calculation_time_stamp ) ) / ( SECOND * 100000 ) );	
									sequence->setIntegralCalculationTimeStamp( current_time[sequence_id] );
								}
									
								float radiation_limit = sequence->getProgramSetPoint( );
								float radiation_sum_counter = sequence->getAccumulatedSetPoint( );
								
								if ( radiation_sum_counter >= radiation_limit )
								{
									// Check to see if the delay to start the next Irrigation Cycle has expired
									if ( time_interval_expired_in_milliseconds( previous_cycle_end_time[sequence_id], cycle_delay[sequence_id] ) )
									{
										state = SEQUENCE_COMMENCE_CYCLE;

										// Update RAD Sum counter (based on the 2009 specifications for NMC 64)
										radiation_sum_counter = fmod( radiation_sum_counter, radiation_limit );
										sequence->setAccumulatedSetPoint( radiation_sum_counter );
										sequence->incrementSequenceNumberCyclesElapsed( );

										// TO DO: Reset the radiation sum counter every time the irrigation program is started, 
										// regardless of the reason: time start, Condition, RAD Sum, manual, etc. ???
										// Do NOT reset the radiation sum counter due to manual valve start
										// Do NOT reset the radiation sum counter when the radiation limit is changed
										// Do NOT reset the radiation sum counter beyween periods or on end daytime
										// sequence->setIntegralCalculationTimeStamp( long time );
									}
								}
								else	// Check to see if the maximum resting time to start the next irrigation cycle has expired
								{
									max_rest_time[sequence_id] = sequence->getProgramMaxRestTime( );
									if ( time_interval_expired_in_milliseconds( previous_cycle_end_time[sequence_id], max_rest_time[sequence_id] ) )
									{
										state = SEQUENCE_COMMENCE_CYCLE;
										sequence->setAccumulatedSetPoint( 0 );
										sequence->incrementSequenceNumberCyclesElapsed( );
									}
								}

								sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
							}

							break;

						case SENSOR_TYPE_CONDITIONS:

							previous_time[sequence_id] = sequence->getSequenceTimeStamp( );
							if ( ( cycle_delay[sequence_id] == 0 ) || ( time_interval_expired_in_milliseconds( previous_time[sequence_id], ( one_thousand_milliseconds * 10 ) ) ) )
							{
								// TO DO: Retrieve a real condition trigger from the Database when one becomes available !!!
								int condition_trigger = 7;
								// DB_GetField(DBCONDITION_TYPE, task_id, record_id, DBCONDITION_NUMBER, &condition_trigger);
								
								int condition_id = sequence->getProgramConditionId( );

								if ( condition_id == condition_trigger )
								{
									// Check to see if the delay to start the next Irrigation Cycle has expired
									if ( time_interval_expired_in_milliseconds( previous_cycle_end_time[sequence_id], cycle_delay[sequence_id] ) )
									{
										state = SEQUENCE_COMMENCE_CYCLE;
										sequence->incrementSequenceNumberCyclesElapsed( );
									}
								}
								else
								{
									// Check to see if the maximum resting time to start the next irrigation cycle has expired
									max_rest_time[sequence_id] = sequence->getProgramMaxRestTime( );
									if ( time_interval_expired_in_milliseconds( previous_cycle_end_time[sequence_id], max_rest_time[sequence_id] ) )
									{
										state = SEQUENCE_COMMENCE_CYCLE;
										// TO DO: Possibly need to reset condition trigger here
										sequence->incrementSequenceNumberCyclesElapsed( );
									}
								}

								sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
							}

							break;

						default: // nothing

							break;

					} // end of switch ( sensor_type )


					// AT THIS POINT IN CODE THE IRRIGATION PROGRAM IS STILL WITHIN THE TIME WINDOW OF RUNNING OPPORTUNITY

					// VERIFY THIS WITH THE MANAGEMENT !!!
					// Check to see if it is time to finish the Sensor program
					// if ( present_date[sequence_id] >= sequence->getProgramEndDate( ) && current_time[sequence_id] >= sequence->getProgramEndTime( ) )
					// {
					//	state = SEQUENCE_COMPLETE_PROGRAM;
					// }

					// AT THIS POINT IN CODE THE IRRIGATION PROGRAM MIGHT NO LONGER BE WITHIN THE TIME WINDOW OF RUNNING OPPORTUNITY
					// AND MIGHT VERY WELL BE IN THE SEQUENCE_COMPLETE_PROGRAM STATE

					break;

				case IRRIGATION_METHOD_ET:

					// Check to see if the delay to start the next irrigation cycle has expired
					if ( time_interval_expired_in_milliseconds( previous_cycle_end_time[sequence_id], cycle_delay[sequence_id] ) )
					{
						previous_time[sequence_id] = sequence->getSequenceTimeStamp( );
						if ( ( cycle_delay[sequence_id] == 0 ) || ( time_interval_expired_in_milliseconds( previous_time[sequence_id], ( one_thousand_milliseconds * 10 ) ) ) )
						{
							// TO DO: Retrieve a real crop factor value from the Database when one becomes available !!!
							int crop_factor_trigger = 42;
							// DB_GetField(DBCROPFACTOR_TYPE, task_id, record_id, DBCROP_FACTOR, &crop_factor_trigger);
							int crop_factor = sequence->getProgramCropFactor( );
							
							if ( crop_factor >= crop_factor_trigger )
							{
								// Check to see if the delay to start the next Irrigation Cycle has expired
								if ( time_interval_expired_in_milliseconds( previous_cycle_end_time[sequence_id], cycle_delay[sequence_id] ) )
								{
									state = SEQUENCE_COMMENCE_CYCLE;
									// TO DO: Possibly need to reset crop factor here
									sequence->incrementSequenceNumberCyclesElapsed( );
								}
							}

							sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
						}
					}

					// AT THIS POINT IN CODE THE IRRIGATION PROGRAM IS STILL WITHIN THE TIME WINDOW OF RUNNING OPPORTUNITY

					// VERIFY THIS WITH THE MANAGEMENT !!!
					// Check to see if it is time to finish the ET program
					// if ( present_date[sequence_id] >= sequence->getProgramEndDate( ) && current_time[sequence_id] >= sequence->getProgramEndTime( ) )
					// {
					//	state = SEQUENCE_COMPLETE_PROGRAM;
					// }

					// AT THIS POINT IN CODE THE IRRIGATION PROGRAM MIGHT NO LONGER BE WITHIN THE TIME WINDOW OF RUNNING OPPORTUNITY
					// AND MIGHT VERY WELL BE IN THE SEQUENCE_COMPLETE_PROGRAM STATE

					break;

				default: // nothing

					break;

			} // end of switch ( irrigation_method[sequence_id] )

			break;

		case SEQUENCE_COMMENCE_CYCLE:

			state = SEQUENCE_CONSTRUCT_VALVE_GROUP;
			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			DebugOutputRequest( 5, SEQUENCE_COMMENCE_CYCLE, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;

		case SEQUENCE_CONSTRUCT_VALVE_GROUP:	// CONSTRUCT ACTIVE GROUP OF IRRIGATION VALVES BEFORE STARTING THE PUMPS

			if ( sequence->getGroupNumber( ) <= sequence->getNumberGroups( ) )
			{
				if ( sequence->constructValveGroup( ) )
				{
					// Initially the flow rate is nominal and it is needed for Dosing Process prior to the SEQUENCE_RUNNING state
					flow_rate_liters_per_second[sequence_id] = sequence->getValveGroupNominalFlowRateLitersPerSecond( );
					flow_rate_cubic_meters_per_hour[sequence_id] = sequence->getValveGroupNominalFlowRateCubicMetersPerHour( );

					DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_FLOW_RATE_LITERS_PER_SECOND, &flow_rate_liters_per_second[sequence_id] );
					DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_FLOW_RATE_CUBIC_METERS_PER_HOUR, &flow_rate_cubic_meters_per_hour[sequence_id] );

					state = SEQUENCE_START_PUMPS;
				}
				else
				{
					state = SEQUENCE_FAILURE;
				}
			}
			else	// No more active Valve Groups left in the Irrigation Sequence
			{
				// Reset Valve Group number counter
				sequence->setGroupNumber( 1 );
				state = SEQUENCE_COMPLETE_CYCLE;
			}

			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			DebugOutputRequest( 5, SEQUENCE_CONSTRUCT_VALVE_GROUP, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;

		case SEQUENCE_START_PUMPS:

			DebugOutputRequest( 5, SEQUENCE_START_PUMPS, task_id, sequence->getSequenceNumberCyclesElapsed( ) );

			if ( sequence->startPumps( ) )
			{
				response[sequence_id] = SEQUENCE_STATUS_IRRIGATING;
				left[sequence_id] = sequence->getValveGroupNominalIrrigationLeft( ); // sequence->getSequenceIrrigationLeft( );

				current_cycle_number[sequence_id] = sequence->getSequenceNumberCyclesElapsed( ) % 256;

				current_time[sequence_id] = get_current_time(IN_MILLISECONDS);
				sequence->setCycleStartTime( current_time[sequence_id] );

				time = DB_TimeNow();

				// Update Database and User Interface
				DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_RESPONSE, &response[sequence_id] );
				DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_START_TIME, &time );
				DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_LEFT, &left[sequence_id] );
				DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_NUMBER_OF_CYCLES_ELAPSED, &current_cycle_number[sequence_id] );

				state = SEQUENCE_OPEN_MAIN_VALVES;
				sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			}
			else
			{
				// Check to see if timeout has expired
				previous_time[sequence_id] = sequence->getSequenceTimeStamp( );
				if ( time_interval_expired_in_milliseconds( previous_time[sequence_id], water_pumps_timeout ) )
				{
					// Failed to start the Water Pumps
					// TO DO: Develop algorithm for shutting the system down and notifying the Manager !!!
					state = SEQUENCE_FAILURE;
					sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
				}
			}

			// sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			break;

		case SEQUENCE_OPEN_MAIN_VALVES:

			DebugOutputRequest( 5, SEQUENCE_OPEN_MAIN_VALVES, task_id, sequence->getSequenceNumberCyclesElapsed( ) );

			if ( sequence->openMainValves( ) )
			{
				state = SEQUENCE_OPEN_IRRIGATION_VALVES;
			}
			else
			{
				// Check to see if timeout has expired
				previous_time[sequence_id] = sequence->getSequenceTimeStamp( );
				if ( time_interval_expired_in_milliseconds( previous_time[sequence_id], main_valves_timeout ) )
				{
					// Failed to open Main Valves
					// TO DO: Develop algorithm for shutting the system down and notifying the Manager !!!
					state = SEQUENCE_FAILURE;
					sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
				}
			}

			// sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			break;

		case SEQUENCE_OPEN_IRRIGATION_VALVES:

			previous_time[sequence_id] = sequence->getSequenceTimeStamp( );
			if ( time_interval_expired_in_milliseconds( previous_time[sequence_id], one_thousand_milliseconds ) )
			{
				DebugOutputRequest( 5, SEQUENCE_OPEN_IRRIGATION_VALVES, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
				sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			}

			// RETURNS TRUE ONLY AFTER ALL THE IRRIGATION VALVES IN THE CURRENT ACTIVE GROUP OF VALVES HAVE BEEN OPENED
			if ( sequence->openIrrigationValves( ) )
			{
				state = SEQUENCE_COMMENCE_VALVE_GROUP;
			}
			else
			{
				// Check to see if timeout has expired
				// TO DO: Replace previous_time with another variable as time_interval_expired_in_milliseconds( previous_time, xxx ) 
				// cannot be nested within another time_interval_expired_in_milliseconds( previous_time, xxx )
				previous_time[sequence_id] = sequence->getSequenceTimeStamp( );
				if ( time_interval_expired_in_milliseconds( previous_time[sequence_id], irrigation_valves_timeout ) )
				{
					// Failed to open Irrigation Valves
					// TO DO: Develop algorithm for shutting the system down and notifying the Process Manager !!!
					state = SEQUENCE_FAILURE;
					sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
				}
			}

			break;

		case SEQUENCE_COMMENCE_VALVE_GROUP:

			state = SEQUENCE_RUNNING;
			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			DebugOutputRequest( 5, SEQUENCE_COMMENCE_VALVE_GROUP, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;

		case SEQUENCE_RUNNING:

			// MONITOR IRRIGATION TIME, IRRIGATION WATER QUANTITY AND/OR DEPTH, WATER FLOW, ETC.
			// WRITE LOGS AND UPDATE DATABASE PERIODICALLY (once a second)

			// Update the runtime values of the Irrigation Process and check to see if it is time to finish irrigating an active Valve Group
			previous_time[sequence_id] = sequence->getSequenceTimeStamp( );
			if ( time_interval_expired_in_milliseconds( previous_time[sequence_id], one_thousand_milliseconds ) )
			{
				// Calculate the elapsed time
				current_time[sequence_id] = get_current_time(IN_MILLISECONDS);
				elapsed_time[sequence_id] = current_time[sequence_id] - previous_time[sequence_id];

				// Note: The following function returns either nominal or measured irrigation left based on the algorithm in the Valve Group
				left[sequence_id] = sequence->updateSequenceIrrigationLeft( elapsed_time[sequence_id] );
				flow_rate_liters_per_second[sequence_id] = sequence->getValveGroupNominalFlowRateLitersPerSecond( );
				flow_rate_cubic_meters_per_hour[sequence_id] = sequence->getValveGroupNominalFlowRateCubicMetersPerHour( );

				// Update Database and User Interface
				DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_LEFT, &left[sequence_id] );
				DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_FLOW_RATE_LITERS_PER_SECOND, &flow_rate_liters_per_second[sequence_id] );
				DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_FLOW_RATE_CUBIC_METERS_PER_HOUR, &flow_rate_cubic_meters_per_hour[sequence_id] );

				irrigation_type[sequence_id] = sequence->getIrrigationType( );
				switch ( irrigation_type[sequence_id] )
				{
					case IRRIGATION_BY_DEPTH:		// MONITOR IRRIGATION PROCESS BY DEPTH

						if ( left[sequence_id].depth == 0 )
							state = SEQUENCE_COMPLETE_VALVE_GROUP;

						break;

					case IRRIGATION_BY_QUANTITY:	// MONITOR IRRIGATION PROCESS BY QUANTITY

						if ( left[sequence_id].quantity == 0 )
							state = SEQUENCE_COMPLETE_VALVE_GROUP;

						break;

					case IRRIGATION_BY_DURATION:	// MONITOR IRRIGATION PROCESS BY DURATION

						duration_left[sequence_id] = sequence->getValveGroupNominalDurationLeft( );

						if ( duration_left[sequence_id] == 0 )
							state = SEQUENCE_COMPLETE_VALVE_GROUP;

						break;

					default:

						break;

				} // end switch ( irrigation_type[sequence_id] )

				irrigation_method[sequence_id] = sequence->getIrrigationMethod( );
				switch ( irrigation_method[sequence_id] )
				{
					case IRRIGATION_METHOD_CYCLES:
						break;

					case IRRIGATION_METHOD_LOOP:
						break;

					case IRRIGATION_METHOD_SENSOR:
					
						sensor_type[sequence_id] = sequence->getProgramSensorType( );
						switch ( sensor_type[sequence_id] )
						{
							case SENSOR_TYPE_VPD_SUM:

								break;
					
							case SENSOR_TYPE_RAD_SUM:

								// WHILE THE IRRIGATION IS TAKING PLACE KEEP TRACK OF THE SOLAR RADIATION ACCUMULATION

								// TO DO: Retrieve real current_radiation from the Database when it becomes available !!!
								float current_radiation = 300;
								// DB_GetField(DBRADIATION_TYPE, task_id, record_id, DBCURRENT_RADIATION, &current_radiation);
								
								// Check to see if the sensor supplies valid solar radiation readings
								if ( current_radiation != SENSOR_N_A ) 
								{
									// TO DO: Remove temporarily declared variable after testing is finished
									long last_integral_calculation_time_stamp = sequence->getIntegralCalculationTimeStamp( );
									current_time[sequence_id] = get_current_time(IN_MILLISECONDS);
									
									// Calculate the integral / 10000 (W/m2 --> J/cm2) and add it to the total
									// sequence->updateAccumulatedSetPoint( ( current_radiation * ( ( current_time[sequence_id] - last_integral_calculation_time_stamp ) / ( SECOND * 10 ) ) ) / 10000 );
									sequence->updateAccumulatedSetPoint( ( current_radiation * ( current_time[sequence_id] - last_integral_calculation_time_stamp ) ) / ( SECOND * 100000 ) );	
									sequence->setIntegralCalculationTimeStamp( current_time[sequence_id] );
								}

								break;

							case SENSOR_TYPE_CONDITIONS:

								break;

							default:

								break;

						} // end switch ( sensor_type[sequence_id] )

						break;
					
					case IRRIGATION_METHOD_ET:
						break;

					default:
						break;

				} // end switch ( irrigation_method[sequence_id] )

				sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
				DebugOutputRequest( 5, SEQUENCE_RUNNING, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			}

			break;

		case SEQUENCE_COMPLETE_VALVE_GROUP:

			// TO DO: At the end of each irrigation cycle regardless of the Irrigation Program type,
			// notify the Program Manager about cycle completion.
			// The Program Manager in turn responds to me with either a permission to continue with the next
			// Irrigation Cycle or to irrigate no more. Does it ????
			// DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_RESPONSE, (IrrigationSequenceResponse *) SEQUENCE_STATUS_IDLE );

			state = SEQUENCE_CLOSE_IRRIGATION_VALVES;
			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			DebugOutputRequest( 5, SEQUENCE_COMPLETE_VALVE_GROUP, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;

		case SEQUENCE_CLOSE_IRRIGATION_VALVES:

			previous_time[sequence_id] = sequence->getSequenceTimeStamp( );
			if ( time_interval_expired_in_milliseconds( previous_time[sequence_id], one_hundred_milliseconds ) )
			{
				// RETURNS TRUE ONLY AFTER ALL THE IRRIGATION VALVES IN THE CURRENT ACTIVE GROUP OF IRRIGATION VALVES HAVE BEEN CLOSED
				if ( sequence->closeIrrigationValves( ) )
				{
					state = SEQUENCE_CLOSE_MAIN_VALVES;
					sequence->setValveGroupEndTime( get_current_time(IN_MILLISECONDS) );
					break;
				}
				else
				{
					// Check to see if timeout has expired
					previous_time[sequence_id] = sequence->getSequenceTimeStamp( );
					if ( time_interval_expired_in_milliseconds( previous_time[sequence_id], irrigation_valves_timeout ) )
					{
						// Failed to close Irrigation Valves
						// TO DO: Develop algorithm for shutting the system down and notifying the Manager !!!
						state = SEQUENCE_FAILURE;
					}
				}

				sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
				DebugOutputRequest( 5, SEQUENCE_CLOSE_IRRIGATION_VALVES, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			}

			break;

		case SEQUENCE_CLOSE_MAIN_VALVES:

			DebugOutputRequest( 5, SEQUENCE_CLOSE_MAIN_VALVES, task_id, sequence->getSequenceNumberCyclesElapsed( ) );

			if ( sequence->closeMainValves( ) )
			{
				state = SEQUENCE_STOP_PUMPS;
			}
			else
			{
				// Check to see if timeout has expired
				previous_time[sequence_id] = sequence->getSequenceTimeStamp( );
				if ( time_interval_expired_in_milliseconds( previous_time[sequence_id], main_valves_timeout ) )
				{
					// Failed to close Main Valves
					// TO DO: Develop algorithm for shutting the system down and notifying the Manager !!!
					state = SEQUENCE_FAILURE;
					sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
				}
			}

			break;

		case SEQUENCE_STOP_PUMPS:

			DebugOutputRequest( 5, SEQUENCE_STOP_PUMPS, task_id, sequence->getSequenceNumberCyclesElapsed( ) );

			if ( sequence->stopPumps( ) )
			{
				state = SEQUENCE_CONSTRUCT_VALVE_GROUP;
			}
			else
			{
				// Check to see if timeout has expired
				previous_time[sequence_id] = sequence->getSequenceTimeStamp( );
				if ( time_interval_expired_in_milliseconds( previous_time[sequence_id], water_pumps_timeout ) )
				{
					// Failed to shut down the Water Pumps
					// TO DO: Develop algorithm for shutting the system down and notifying the Manager !!!
					state = SEQUENCE_FAILURE;
					sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
				}
			}

			break;

		case SEQUENCE_COMPLETE_CYCLE:

			// response = SEQUENCE_CYCLE_COMPLETED;
			// DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_RESPONSE, &response );

			// TO DO: At the end of the last Valve Group in the Irrigation Sequence cycle check for the following special condition:
			// If there are Irrigation Valves in the last Valve Group that also appear in the first Valve Group in the next Irrigation Sequence,
			// and if the Cycle Delay is 0, then do NOT close those Irrigation Valves and do NOT stop the Irrigation Process

			state = SEQUENCE_CHECK_FOR_NEXT_CYCLE;

			sequence->setCycleEndTime( get_current_time(IN_MILLISECONDS) );
			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );

			response[sequence_id] = SEQUENCE_STATUS_WAITING;
			DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_RESPONSE, &response[sequence_id] );

			DebugOutputRequest( 5, SEQUENCE_COMPLETE_CYCLE, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;

		case SEQUENCE_COMPLETE_PROGRAM:

			state = SEQUENCE_UPDATE_DATABASE;
			sequence->setSequenceEndTime( get_current_time(IN_MILLISECONDS) );
			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			DebugOutputRequest( 5, SEQUENCE_COMPLETE_PROGRAM, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;

		case SEQUENCE_UPDATE_DATABASE:

			// TO DO: Write Log of operations and save sequence in the Database
			// extern bool DB_GetRecord(DBType type, DBTask reader, DBIndex index, void * output);
			// Make sure to test the return value of the DB_UpdateRecord for succes or failure; repeat the update operation if necessary
			// extern bool DB_UpdateRecord(DBType type, DBTask updater, DBIndex index, const void * update);
			// extern bool DB_UpdateField(DBType type, DBTask updater, DBIndex index, DBColumn column, void * input);

			response[sequence_id] = SEQUENCE_STATUS_COMPLETED;
			DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_RESPONSE, &response[sequence_id] );

			state = SEQUENCE_FINISHED;
			sequence->setSequenceEndTime( get_current_time(IN_MILLISECONDS) );
			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			DebugOutputRequest( 5, SEQUENCE_UPDATE_DATABASE, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;

		case SEQUENCE_PAUSE:

			// TO DO: Develop algorithm for pausing/stalling the Irrigation Process
			// Which devices should be deactivated, such as pumps, main valves, irrigation valves and in what order?
			// Are there any timeouts for staying in the PAUSED state?

			response[sequence_id] = SEQUENCE_STATUS_PAUSED;
			DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_RESPONSE, &response[sequence_id] );

			state = SEQUENCE_FINISHED;	// for now
			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			DebugOutputRequest( 5, SEQUENCE_PAUSE, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;

		case SEQUENCE_RESUME:

			// TO DO: Develop algorithm for resuming/revive the Irrigation Process
			// Which devices should be re-activated, such as pumps, main valves, irrigation valves and in what order?
			// Are there any timeouts for staying in the PAUSED state?

			state = SEQUENCE_FINISHED;	// for now
			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			DebugOutputRequest( 5, SEQUENCE_RESUME, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;

		case SEQUENCE_ABORT:

			// TO DO: Develop algorithm for aborting the Irrigation Process

			response[sequence_id] = SEQUENCE_STATUS_ABORTED;
			DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_RESPONSE, &response[sequence_id] );

			state = SEQUENCE_FINISHED;	// for now
			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			DebugOutputRequest( 5, SEQUENCE_ABORT, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;

		case SEQUENCE_PREEMPT:

			// TO DO: Develop algorithm for preempting the Irrigation Process
			// Which devices should be deactivated, such as pumps, main valves, irrigation valves and in what order?
			// Are there any timeouts for staying in the PREEMPTED state?

			state = SEQUENCE_UNCOMPLETED;
			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			DebugOutputRequest( 5, SEQUENCE_PREEMPT, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;

		case SEQUENCE_FAILURE:


			state = SEQUENCE_UNCOMPLETED;
			DebugOutputRequest( 5, SEQUENCE_FAILURE, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;

		case SEQUENCE_UNCOMPLETED:

			response[sequence_id] = SEQUENCE_STATUS_UNCOMPLETED;
			DB_UpdateField( DBVALVESEQUENCE_TYPE, task_id, sequence_id, DBVALVESEQUENCE_RESPONSE, &response[sequence_id] );

			state = SEQUENCE_FINISHED;	// for now
			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			DebugOutputRequest( 5, SEQUENCE_UNCOMPLETED, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;

		case SEQUENCE_FINISHED:

			// THIS PLACE IN CODE MUST NEVER BE REACHED
			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			DebugOutputRequest( 5, SEQUENCE_FINISHED, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;

		default:

			// THIS PLACE IN CODE MUST NEVER BE REACHED
			state = SEQUENCE_FINISHED;
			sequence->setSequenceTimeStampAndState( get_current_time(IN_MILLISECONDS), state );
			DebugOutputRequest( 5, SEQUENCE_FINISHED, task_id, sequence->getSequenceNumberCyclesElapsed( ) );
			break;
	}

	return state;
}

}; // end of extern "C"

// SEQUENCE_STATUS_WAITING,
// SEQUENCE_STATUS_IRRIGATING,
// SEQUENCE_STATUS_PAUSED,
// SEQUENCE_STATUS_ALARMED,
// SEQUENCE_STATUS_ABORTED,
// SEQUENCE_STATUS_UNCOMPLETED,
// SEQUENCE_STATUS_COMPLETED

// DBTask task_id = sequence.getTaskId( );
// DBType table_type = db_message->type;
// DBIndex record_id = db_message->index;
// DBColumn column_id = db_message->column;
// DBOperation operation_type = db_message->operation;

// MessageBox *msgBox = reinterpret_cast<MessageBox *>( msgBoxes[sequence_number] );
// Copy the message into the local variable
// DB_LISTENER_MESSAGE_T db_message = *(DB_LISTENER_MESSAGE_P) raw_message;
// Reset the message inside the message box (make it NULL)
// K_Mem_FB_Release( &MEM_POOL, raw_message );
// DB_LISTENER_MESSAGE_T, *DB_LISTENER_MESSAGE_P: { DBType type; DBIndex index; DBColumn column; DBOperation operation; }
// Check the manager_reguest field (IrrigationManagerRequest) in the db_message.type table indexed by db_message.index
// bool DB_GetField(DBType type, DBTask reader, DBIndex index, DBColumn column, void * result);

//case SEQUENCE_COMMENCE_CYCLE:

//	DebugOutputRequest( 5, SEQUENCE_COMMENCE_CYCLE, task_id, sequence->getSequenceNumberCyclesElapsed( ) );

// TO DO: Test if using the following two lines successfully replace the switch statement below !!!
// set = sequence->getSequenceIrrigationSet( );
// left = sequence->getSequenceIrrigationLeft( );
// left = sequence->getSequenceMeasuredIrrigationLeft( );

//irrigation_type = sequence->getIrrigationType( );
//switch ( irrigation_type )
//{
//	case IRRIGATION_BY_DEPTH:

//		set.depth = sequence->getSequenceDepthSet( );
//		left.depth = sequence->getSequenceDepthLeft( );
//		break;

//	case IRRIGATION_BY_QUANTITY:

//		set.quantity = sequence->getSequenceQuantitySet( );
//		left.quantity = sequence->getSequenceQuantityLeft( );
//		break;

//	case IRRIGATION_BY_DURATION:

//		//duration_set = sequence->getSequenceDurationSet( );
//		//duration_left = sequence->getSequenceDurationLeft( );
//		//set.duration = duration_set;
//		//left.duration = duration_left;

//		set.duration = sequence->getSequenceDurationSet( );
//		left.duration = sequence->getSequenceDurationLeft( );
//		break;
//
//	default:
//}