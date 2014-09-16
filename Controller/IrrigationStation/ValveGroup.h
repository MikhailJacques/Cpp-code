
// Valve Group class unsigned interface file

#ifndef VALVE_GROUP_H
#define VALVE_GROUP_H

#include "DB.h"
#include "DBBase.h"
#include "tasks.h"
#include "MainValve.h"
#include "WaterMeter.h"
#include "IrrigationValve.h"
#include "WaterMeterMessage.h"
#include "EASTL\fixed_set.h"
#include "EASTL\fixed_map.h"
#include "EASTL\fixed_vector.h"
#include "EASTL\algorithm.h"
#include "EASTL\sort.h"
#include "NMC_MML_Platform.h"

using namespace eastl;

class ValveGroup 
{ 
	public:

		explicit ValveGroup( );
		// explicit ValveGroup( const ValveGroup & group );
		~ValveGroup( );
		// ValveGroup & operator=( const ValveGroup & group );

		/////////////////////////////////////////////////////////////
		// ACCESSOR FUNCTIONS
		/////////////////////////////////////////////////////////////
		DBIndex getTaskId( ) const;
		int getGroupNumber( ) const;
		int getNumberGroups( ) const;
		int getNumberValves( ) const;
		DBIndex getDosingStationId( ) const;
		IrrigationType getIrrigationType( ) const;
		float getIrrigationArea( ) const;
		long getStartTime( ) const;
		long getEndTime( ) const;
		long getTimeStamp( ) const;
		bool getState( ) const;
		const ValveGroup & getGroup( ) const;

		// IRRIGATION BY (DEPTH, QUANTITY, DURATION)
		// IIrrigationSetLeft getIrrigationSet( ) const;
		// IIrrigationSetLeft getIrrigationLeft( ) const;
		IIrrigationSetLeft getNominalIrrigationLeft( ) const;
		IIrrigationSetLeft getMeasuredIrrigationLeft( ) const;

		// IRRIGATION BY DEPTH
		float getDepthSet( ) const;
		float getNominalDepthLeft( ) const;
		float getMeasuredDepthLeft( ) const;

		// IRRIGATION BY QUANTITY
		float getQuantitySet( ) const;
		float getNominalQuantityLeft( ) const;
		float getMeasuredQuantityLeft( ) const;

		// IRRIGATION BY DURATION
		long getDurationSet( ) const;
		long getNominalDurationLeft( ) const;
		long getMeasuredDurationLeft( ) const;

		// WATER FLOW
		float getFlowRateLitersPerSecond( ) const;
		float getFlowRateCubicMetersPerHour( ) const;
		float getNominalFlowRateLitersPerSecond( ) const;
		float getMeasuredFlowRateLitersPerSecond( ) const;
		float getNominalFlowRateCubicMetersPerHour( ) const;
		float getMeasuredFlowRateCubicMetersPerHour( ) const;


		/////////////////////////////////////////////////////////////
		// MUTATOR FUNCTIONS
		/////////////////////////////////////////////////////////////		
		void setTaskId( DBTask task_id );
		void setGroupNumber( int group_number );
		void setNumberGroups( int number_of_groups );
		void setValves( const fixed_vector<IrrigationValve, MAX_VALVES_PER_GROUP, false> & valves );
		void setIrrigationType( IrrigationType irrigation_type );
		void setIrrigationArea( float irrigation_area );
		void setStartTime( long time );
		void setEndTime( long time );
		void setTimeStamp( long time );
		void setState( bool state );
		void setDosingStationId( DBIndex dosing_station_id );
		// void setGroup( const ValveGroup & group );
		// void ValveGroup::setFertigationRecipe( const IFertigationRecipe recipe[] );

		// IRRIGATION BY (DEPTH, QUANTITY, DURATION)
		void setIrrigationSet( IIrrigationSetLeft set );
		// void setIrrigationLeft( IIrrigationSetLeft left );
		// void setNominalIrrigationDelivered( IIrrigationSetLeft nominal_delivered );
		// void setMeasuredIrrigationDelivered( IIrrigationSetLeft measured_delivered );
		// void setNominalIrrigationLeft( IIrrigationSetLeft nominal_left );
		// void setMeasuredIrrigationLeft( IIrrigationSetLeft measured_left );

		// IRRIGATION BY DEPTH
		void setDepthSet( float set );
		// void setNominalDepthDelivered( float delivered );
		// void setMeasuredDepthDelivered( float delivered );
		void setNominalDepthLeft( float left );
		void setMeasuredDepthLeft( float left );

		// IRRIGATION BY QUANTITY
		void setQuantitySet( float set);
		// void setNominalQuantityDelivered( float delivered );
		// void setMeasuredQuantityDelivered( float delivered );
		void setNominalQuantityLeft( float left );
		void setMeasuredQuantityLeft( float left );

		// IRRIGATION BY DURATION
		void setDurationSet( long set );
		// void setNominalDurationElapsed( long elapsed );
		// void setMeasuredDurationElapsed( long elapsed );
		void setNominalDurationLeft( long left );
		void setMeasuredDurationLeft( long left );

		// ADDITION OF SYSTEM COMPONENTS
		void addMainValve( DBIndex valve_id );
		void addValve( const IrrigationValve & valve );
		void addMainValveToTheCurrentValveGroup( DBIndex main_valve_id );
		void addMainValveToTheNextValveGroup( DBIndex main_valve_id );
		void addIrrigationValveToTheCurrentValveGroup( DBIndex valve_id );
		void addIrrigationValveToTheNextValveGroup( DBIndex valve_id );
		void ValveGroup::addWaterMeter( const WaterMeter & water_meter );
		void processWaterMeterMessage( const DB_LISTENER_MESSAGE_P db_message );
		// void processWaterMeterMessage( const DB_LISTENER_MESSAGE_P db_message );

		// INITIALIZATION AND RESETTING
		void initGroup( DBTask task_id, int number_of_groups, IrrigationType irrigation_type, DBIndex dosing_station_id );
		void resetGroup( int group_number ); 
		void updateGroup( );

		// UPDATE
		IIrrigationSetLeft updateIrrigationLeft( long elapsed_time );

		// ACTIVATION/DEACTIVATION OF SYSTEM COMPONENTS
		bool startPumps( );				// Starts Water Pumps	
		bool stopPumps( );				// Stops Water Pumps
		bool openMainValves( );			// Opens Main Valves for the group
		bool closeMainValves( );		// Closes ONLY those Main Valves that does NOT appear in the next Valve Group (if there is one)	
		bool openIrrigationValves( );	// Opens Irrigation Valves in the group
		bool closeIrrigationValves( ); 	// Closes ONLY those Irrigation Valves that does NOT appear in the next Valve Group (if there is one)
		void removeClosedValves( );		// Removes closed Main and Irrigation Valves from the Valve Group

	private:

		// UPDATES
		float updateMeasuredFlowRate( long elapsed_time );
		void updateNominalDepthLeft( long elapsed_time );
		void updateMeasuredDepthLeft( long elapsed_time );
		void updateNominalQuantityLeft( long elapsed_time );
		void updateMeasuredQuantityLeft( long elapsed_time );
		void updateNominalDurationLeft( long elapsed_time );
		void updateMeasuredDurationLeft( long elapsed_time );
		void setNominalAndMeasuredLeft( );
		// IIrrigationSetLeft updateNominalIrrigationLeft( DBTimePP elapsed_time );
		// IIrrigationSetLeft updateMeasuredIrrigationLeft( DBTimePP elapsed_time );

		// CONVERSIONS
		void convertIrrigationAreaIntoSquareMeters( );
		void convertFlowRateCubicMetersPerHourIntoLitersPerSecond( );
		float convertDepthIntoQuantity( float depth );
		float convertQuantityIntoDepth( float quantity );
		long convertQuantityIntoDuration( float quantity, float flow );
		float convertDurationIntoQuantity( long duration, float flow );

		// QUERRIES
		bool doesMainValveExist( DBIndex valve_id );
		bool doesMainValveExistInTheCurrentValveGroup( DBIndex main_valve_id );
		bool doesMainValveExistInTheNextValveGroup( DBIndex main_valve_id );
		bool doesIrrigationValveExistInTheCurrentValveGroup( DBIndex valve_id );
		bool doesIrrigationValveExistInTheNextValveGroup( DBIndex valve_id );

		void setOpenedMainValvesCounter( int counter );
		void setOpenedIrrigationValvesCounter( int counter );
		void setNominalFlowRateCubicMetersPerHour( float nominal_flow );
		void setNominalFlowLitersPerSecond( float nominal_flow );

		void initIrrigationSetLeftZeroAndTimeZero( );

		fixed_vector<MainValve, MAX_MAIN_VALVES_PER_GROUP, false> main_valves;				// Range [1 - 10]
		fixed_vector<MainValve, MAX_MAIN_VALVES_PER_GROUP, false>::iterator main_valve_itr;

		fixed_vector<IrrigationValve, MAX_VALVES_PER_GROUP, false> valves;					// Range [1 - 20]
		fixed_vector<IrrigationValve, MAX_VALVES_PER_GROUP, false>::iterator valve_itr;

		fixed_vector<DBIndex, MAX_MAIN_VALVES_PER_GROUP, false> main_valve_ids;				// Range [1 - 10]
		fixed_vector<DBIndex, MAX_MAIN_VALVES_PER_GROUP, false>::iterator main_valve_ids_itr;

		fixed_vector<DBIndex, MAX_VALVES_PER_GROUP, false> valve_ids;						// Range [1 - 20]
		fixed_vector<DBIndex, MAX_VALVES_PER_GROUP, false>::iterator valve_ids_itr;

		// TO DO: Consider replacing WaterMeter with DigitalInput union !!!
		fixed_map<DBIndex, WaterMeter, MAX_WATER_METERS_PER_GROUP, false> water_meters;					// Range [1 - 5]
		fixed_map<DBIndex, WaterMeter, MAX_WATER_METERS_PER_GROUP, false>::iterator water_meters_itr;	// Range [1 - 5]

		// Sum total of the nominal flow of ALL the Irrigation Valves in the Valve Group (L/sec and m3/hr)
		float nominal_flow_rate_liters_per_second;
		float nominal_flow_rate_cubic_meters_per_hour;

		// Calculated sum total of the measured flow of ALL the Water Meters for this Valve Group (L/sec and m3/hr)
		float measured_flow_rate_liters_per_second;
		float measured_flow_rate_cubic_meters_per_hour;

		// Sum total of ALL the Irrigation Valves' areas in the Valve Group (m2)
		float irrigation_area;				
		
		DBTask task_id;
		int group_number;						// Valve Group number in the Valve Sequence; Range [1 - 100]
		int number_of_groups;					// Number of Valve Groups in the Valve Sequence
		int opened_irrigation_valves_counter;	// Keeps track of the number of open Irrigation Valves in the Valve Group 
		int opened_main_valves_counter;			// Keeps track of the number of open Main Valves in the Valve Group
		bool water_meter_pulse_timeout;			// 
		bool state;								// Valve Group state (On/Off)

		DBIndex dosing_station_id;				// Id of the Dosing Station to which the Valve Sequence is phisically connected
		
		// Fertigation recipes for each Dosing Channel of the Dosing Station the Valve Sequence is physically connected to
		// IFertigationRecipe recipe[MAX_DOSING_CHANNELS_PER_STATION];	
		
		long start_time;						// Timestamp when the valve group started irrigation (milliseconds)
		long end_time;							// Timestamp when the valve group finished irrigation (milliseconds)
		long time_stamp;						// Timestamp of the previous state change / operation (milliseconds)
		long water_meter_time_stamp;			// Timestamp for the purpose of counting Water Meter pulses (milliseconds)

		IrrigationType irrigation_type;				// Irrigation type; Range [IRRIGATION_BY_DEPTH, IRRIGATION_BY_QUANTIY, IRRIGATION_BY_DURATION]
		
		// IIrrigationSetLeft set;					// Irrigation set
		// IIrrigationSetLeft left;					// Irrigation left
		// IIrrigationSetLeft nominal_delivered;	// Irrigation delivered so far by nominal
		// IIrrigationSetLeft measured_delivered;	// Irrigation delivered so far by measured
		IIrrigationSetLeft nominal_left;			// Nominal irrigation (depth, quantity, duration) left until completion
		IIrrigationSetLeft measured_left;			// Measured irrigation (depth, quantity, duration) left until completion
		IIrrigationSetLeft set_left_zero;			// Irrigation set left zero

		float depth_set;							// Irrigation depth set until completion; Range [0 - 9999999]
		// float depth_elapsed;						// Irrigation depth set until completion; Range [0 - 9999999]
		// float depth_left;						// Irrigation depth set until completion; Range [0 - 9999999]
		float nominal_depth_delivered;				// Nominal irrigation depth delivered so far; Range [0 - 9999999]
		float measured_depth_delivered;				// Measured irrigation depth delivered so far; Range [0 - 9999999]
		float nominal_depth_left;					// Nominal irrigation depth left until completion; Range [0 - 9999999]
		float measured_depth_left;					// Measured irrigation depth left until completion; Range [0 - 9999999]

		float quantity_set;							// Irrigation quantity set by the user; Range [0.0 - 9999999]
        // float quantity_elapsed;					// Irrigation quantity set by the user; Range [0.0 - 9999999]
		// float quantity_left;						// Irrigation quantity set by the user; Range [0.0 - 9999999]
		float nominal_quantity_delivered;			// Nominal irrigation quantity delivered so far; Range [0.0 - 9999999]
		float measured_quantity_delivered;			// Measured irrigation quantity delivered so far; Range [0.0 - 9999999]
		float nominal_quantity_left;				// Nominal irrigation quantity left until completion; Range [0.0 - 9999999]
		float measured_quantity_left;				// Measured irrigation quantity left until completion; Range [0.0 - 9999999]

		long duration_set;					// Duration set (milliseconds)
		// long duration_elapsed;			// Duration set (milliseconds)
		// long duration_left;				// Duration set (milliseconds)
		long nominal_duration_elapsed;		// Nominal duration elapsed so far (milliseconds)
		long measured_duration_elapsed;	// Measured duration elapsed so far (milliseconds)
		long nominal_duration_left;		// Nominal duration left (milliseconds)
		long measured_duration_left;		// Measured duration left (milliseconds)
		
		// DBTimePP duration_set;					// Irrigation duration set by the user; Range [00:00:00 - 23:59:59]
        // DBTimePP nominal_duration_left;			// Nominal irrigation duration left until completion; Range [00:00:00 - 23:59:59]
		// DBTimePP measured_duration_left;			// Measured irrigation duration left until completion; Range [00:00:00 - 23:59:59]

		WORD error_code;
};

#endif