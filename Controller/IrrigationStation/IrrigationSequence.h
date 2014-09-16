
// Irrigation Sequence class interface file

#ifndef IRRIGATION_SEQUENCE_H
#define IRRIGATION_SEQUENCE_H

#include "DB.h"
#include "DBBase.h"
#include "MessageBox.h"
#include "DBDateTime.h"
#include "ValveGroup.h"
#include "NMC_MML_Platform.h"
#include "IrrigationIncludes.h"
#include "DBIrrigationSequence.h"

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

class IrrigationSequence : private DBValveSequence
{
	public:

		explicit IrrigationSequence( );
		explicit IrrigationSequence( DBTask task_id );
		// explicit IrrigationSequence( const IrrigationSequence & sequence );
		~IrrigationSequence( );
		// IrrigationSequence & operator=( const IrrigationSequence & program );
		
		/////////////////////////////////////////////////////////////
		// ACCESSOR FUNCTIONS
		/////////////////////////////////////////////////////////////
		DBTask  getTaskId( ) const;
		DBIndex getProgramId( ) const;
		DBIndex getSequenceId( ) const;
		DBIndex getDosingStationId( ) const;
		DBIndex getProgramSensorId( ) const;
		DBIndex getProgramConditionId( ) const;
		DBIndex getAlarmId( ) const;
		// byte getMessageBoxNumber( ) const;
		
		// IRRIGATION PROGRAM PROPERTIES
		DBDatePP getProgramStartDate( ) const;
		DBDatePP getProgramEndDate( ) const;
		long getProgramStartTime( ) const;
		long getProgramEndTime( ) const;
		long getCycleDelay( ) const;
		long getProgramMaxRestTime( ) const;
		byte getSequenceNumberCyclesSet( ) const;
		byte getSequenceNumberCyclesElapsed( ) const;
		byte getSequenceNumberCyclesLeft( ) const;
		SensorType getProgramSensorType( ) const;
		float getProgramSetPoint( ) const;
		float getAccumulatedSetPoint( ) const;
		int getProgramCropFactor( ) const;

		// VALVE (IRRIGATION) SEQUENCE PROPERTIES
		IrrigationSequenceState getSequenceState( ) const;
		IrrigationManagerRequest getManagerRequest( ) const;	
		IrrigationSequenceResponse getSequenceResponse( ) const;	
		IrrigationMethod getIrrigationMethod( ) const;
		IrrigationType getIrrigationType( ) const;
		int getGroupNumber( ) const;
		int getNumberGroups( ) const;
		int getNumberValves( ) const;
		const MessageBox & getMessageBox( ) const;
		const IrrigationSequence & getSequence( ) const;
		// IFertigationRecipe * getFertigationRecipe( );

		// IRRIGATION BY (DEPTH, QUANTITY, DURATION)
		IIrrigationSetLeft getSequenceIrrigationSet( ) const;
		IIrrigationSetLeft getSequenceIrrigationLeft( ) const;
		// IIrrigationSetLeft getSequenceNominalIrrigationLeft( ) const;
		// IIrrigationSetLeft getSequenceMeasuredIrrigationLeft( ) const;
		// IIrrigationSetLeft getValveGroupIrrigationSet( ) const;
		IIrrigationSetLeft getValveGroupNominalIrrigationLeft( ) const;
		IIrrigationSetLeft getValveGroupMeasuredIrrigationLeft( ) const;

		// IRRIGATION BY DEPTH
		float getSequenceDepthSet( ) const;
		float getSequenceDepthLeft( ) const;
		// float getSequenceNominalDepthLeft( ) const;
		// float getSequenceMeasuredDepthLeft( ) const;
		// float getValveGroupDepthSet( ) const;
		float getValveGroupNominalDepthLeft( ) const;
		float getValveGroupMeasuredDepthLeft( ) const;

		// IRRIGATION BY QUANTITY
		float getSequenceQuantitySet( ) const;
		float getSequenceQuantityLeft( ) const;
		// float getSequenceNominalQuantityLeft( ) const;
		// float getSequenceMeasuredQuantityLeft( ) const;
		// float getValveGroupQuantitySet( ) const;
		float getValveGroupNominalQuantityLeft( ) const;
		float getValveGroupMeasuredQuantityLeft( ) const;

		// IRRIGATION BY DURATION
		long getSequenceDurationSet( ) const;
		long getSequenceDurationLeft( ) const;
		// long getSequenceNominalDurationLeft( ) const;
		// long getSequenceMeasuredDurationLeft( ) const;
		// long getValveGroupDurationSet( ) const;
		long getValveGroupNominalDurationLeft( ) const;
		long getValveGroupMeasuredDurationLeft( ) const;
		
		// TIME STAMPS
		long getSequenceStartTime( ) const;
		long getSequenceEndTime( ) const;
		long getSequenceTimeStamp( ) const;

		long getCycleStartTime(  ) const;
		long getCycleEndTime( ) const;
		long getCycleTimeStamp( ) const;

		long getValveGroupStartTime( ) const;
		long getValveGroupEndTime( ) const;
		long getValveGroupTimeStamp( ) const;

		long getIntegralCalculationTimeStamp( );

		// WATER FLOW
		float getValveGroupNominalFlowRateLitersPerSecond( ) const;
		float getValveGroupNominalFlowRateCubicMetersPerHour( ) const;	
		float getValveGroupMeasuredFlowRateLitersPerSecond( );
		float getValveGroupMeasuredFlowRateCubicMetersPerHour( );


		////////////////////////////////////////////////////////////
		// MUTATOR FUNCTIONS
		////////////////////////////////////////////////////////////
		void setTaskId( DBTask id );
		void setProgramId( DBIndex id );
		void setSequenceId( DBIndex id );
		void setDosingStationId( DBIndex dosing_station_id );
		void setProgramSensorId( DBIndex sensor_id );
		void setProgramConditionId( DBIndex condition_id );
		void setAlarmId( DBIndex alarm_id );
		// void setMessageBoxNumber( byte message_box_number );

		// IRRIGATION PROGRAM PROPERTIES
		void setProgramStartDate( DBDatePP date );
		void setProgramEndDate( DBDatePP date );
		void setProgramStartTime( long time );
		void setProgramEndTime( long time );
		void setCycleDelay( long delay );
		void setProgramMaxRestTime( long max_rest );
		void setSequenceNumberCyclesSet( byte number_cycles_set );
		void setSequenceNumberCyclesElapsed( byte number_of_cycles_elapsed );
		void incrementSequenceNumberCyclesElapsed( );
		void setSequenceNumberCyclesLeft( byte number_cycles_left );
		void decrementSequenceNumberCyclesLeft( );
		void setProgramSensorType( SensorType sensor_type );
		void setProgramSetPoint( float set_point );
		void setAccumulatedSetPoint( float set_point );
		void updateAccumulatedSetPoint( float set_point );
		void setProgramCropFactor( int crop_factor );
		
		// VALVE (IRRIGATION) SEQUENCE PROPERTIES
		void setSequenceState( IrrigationSequenceState state );
		void setManagerRequest( IrrigationManagerRequest request );
		void setSequenceResponse( IrrigationSequenceResponse response );
		void setIrrigationMethod( IrrigationMethod irrigation_method );
		void setIrrigationType( IrrigationType irrigation_type );
		// void IrrigationSequence::setFertigationRecipe( const IFertigationRecipe recipe[] );
		void setGroupNumber( int next_group_number );
		void setNumberGroups( int number_of_groups );
		void setNumberValves( int number_of_valves );
		void setValveGroup( const ValveGroup & group );
		void setMessageBox( const MessageBox & box );
		// void setSequence( const IrrigationSequence & sequence );

		// IRRIGATION BY (DEPTH, QUANTITY, DURATION)
		void setSequenceIrrigationSet( IIrrigationSetLeft set );
		void setSequenceIrrigationLeft( IIrrigationSetLeft left );
		// void setSequenceNominalIrrigationLeft( IIrrigationSetLeft left );
		// void setSequenceMeasuredIrrigationLeft( IIrrigationSetLeft left );
		// void setValveGroupIrrigationSet( IIrrigationSetLeft set );
		// void setValveGroupNominalIrrigationLeft( IIrrigationSetLeft left );
		// void setValveGroupMeasuredIrrigationLeft( IIrrigationSetLeft left );
		IIrrigationSetLeft updateSequenceIrrigationLeft( long elapsed_time );
		// IIrrigationSetLeft updateValveGroupNominalIrrigationLeft( long elapsed_time );
		// IIrrigationSetLeft updateValveGroupMeasuredIrrigationLeft( long elapsed_time );

		// IRRIGATION BY DEPTH
		void setSequenceDepthSet( float depth );
		void setSequenceDepthLeft( float depth );
		// void setSequenceNominalDepthLeft( float left );
		// void setSequenceMeasuredDepthLeft( float left );
		// void setValveGroupDepthSet( float set );
		// void setValveGroupNominalDepthLeft( float left );
		// void setValveGroupMeasuredDepthLeft( float left );
		// float updateValveGroupDepthLeft( long elapsed_time );	
		// float updateValveGroupNominalDepthLeft( long elapsed_time );	
		// float updateValveGroupMeasuredDepthLeft( long elapsed_time );

		// IRRIGATION BY QUANTITY
		void setSequenceQuantitySet( float quantity );
		void setSequenceQuantityLeft( float quantity );
		// void setSequenceNominalQuantityLeft( float left );
		// void setSequenceMeasuredQuantityLeft( float left );
		// void setValveGroupQuantitySet( float set );
		// void setValveGroupNominalQuantityLeft( float left );
		// void setValveGroupMeasuredQuantityLeft( float left );
		// float updateValveGroupQuantityLeft( long elapsed_time );
		// float updateValveGroupNominalQuantityLeft( long elapsed_time );
		// float updateValveGroupMeasuredQuantityLeft( long elapsed_time );

		// IRRIGATION BY DURATION
		void setSequenceDurationSet( long time );
		// void setSequenceDurationLeft( long time );
		// void setSequenceNominalDurationLeft( long left );
		// void setSequenceMeasuredDurationLeft( long left );
		// void setValveGroupDurationSet( long set );
		// void setValveGroupNominalDurationLeft( long left );
		// void setValveGroupMeasuredDurationLeft( long left );
		// long updateValveGroupDurationLeft( long elapsed_time );
		// long updateValveGroupNominalDurationLeft( long elapsed_time );
		// long updateValveGroupMeasuredDurationLeft( long elapsed_time );

		// TIME STAMPS
		void setSequenceStartTime( long time );
		void setSequenceEndTime( long time );
		void setSequenceTimeStamp( long time );
		void setSequenceTimeStampAndState( long time, IrrigationSequenceState state );
		
		void setCycleStartTime( long time );
		void setCycleEndTime( long time );
		void setCycleTimeStamp( long time );

		void setValveGroupStartTime( long time );
		void setValveGroupEndTime( long time );
		void setValveGroupTimeStamp( long time );

		void setIntegralCalculationTimeStamp( long time );
		
		// WATER FLOW
		// Not needed here for now

		// CONSTRUCTION AND OPERATION
		bool initSequence( );				// Initializes Irrigation Sequence object
		bool constructValveGroup( );		// Constructs an active group of Irrigation Valves
		bool startPumps( );					// Starts Water Pumps	
		bool stopPumps( );					// Stops Water Pumps
		bool openMainValves( );				// Opens Main Valves
		bool closeMainValves( );			// Closes Main Valves
		bool openIrrigationValves( );		// Opens Irrigation Valves in ONE group of Irrigation Valves ONLY
		bool closeIrrigationValves( );		// Closes Irrigation Valves in ONE group of Irrigation Valves ONLY

		IrrigationSequenceState processDatabaseMessages( IrrigationSequenceState _state );
		IrrigationSequenceState getIrrigationSequenceResponse( IrrigationManagerRequest request );

	private:

		long convertTimeToMilliseconds( DBTime time ); 
		void retrieveDosingStationId( DBIndex irrigation_valve_id );
		void processWaterMeterMessage( const DB_LISTENER_MESSAGE_P message );
		IrrigationSequenceState processValveSequenceMessage( const DB_LISTENER_MESSAGE_P db_message );
		
		// THE FOLLOWING PROPERTIES ARE DECLARED IN THE DBIrrigationSequence STRUCTURE:	
		// DBTime start_time;					// Start time of the Irrigation Sequence that is set and reported by the Irrigation Process 
		// IrrigationManagerRequest request;	// Request for irrigation sequence operation received from the Irrigation Manager
		// IrrigationSequenceResponse response;	// Response sent by Irrigation Sequence to the Irrigation Manager
		// DBIndex alarm_id;					// Id of the alarm
		// byte number_of_cycles_elapsed;		// Number of cycles elapsed in the irrigation sequence
		// float accumulated_set_point;			// Accumulated set point (VPD Summary Counter, RAD Summary Counter)
		// BITFIELD[100] signs;					// 104 bits out of which a maximum of the first 99 are used for either "," or "+' separators ["0" = "+", "1" = ","]
		// byte number_of_valves;			    // Number of Irrigation Valves in the sequence; Range [0 - 99]
		// DBIndex valves[100];					// Indices of the Irrigation Valves; Values Range [1 - 256]		
		// IrrigationType irrigation_type;		// Irrigation type; Range [IRRIGATION_BY_DEPTH, IRRIGATION_BY_QUANTITY, IRRIGATION_BY_DURATION]
		// IFertigationRecipe[8] recipe;		// Fertigation recipes for each Dosing Channel of the Dosing Station the Valve Sequence is physically connected to
		// IIrrigationSetLeft set;				// Irrigation set; Respective ranges are: [0 - 9999999 [0.0 - 9999999], [00:00:00 - 23:59:59]
		// IIrrigationSetLeft left;				// Irrigation left until completion; Respective ranges are: [0 - 9999999 [0.0 - 9999999], [00:00:00 - 23:59:59]
		// float flow_rate_liters_per_second;		// Irrigation flow rate (L/sec)
		// float flow_rate_cubic_meters_per_hour;	// Irrigation flow rate (m3/hr)

		// IIrrigationSetLeft nominal_left;		// Nominal irrigation (depth, quantity, duration) left until completion; Respective ranges are: [0 - 9999999 [0.0 - 9999999], [00:00:00 - 23:59:59]
		// IIrrigationSetLeft measured_left;	// Measured irrigation (depth, quantity, duration) left until completion; Respective ranges are: [0 - 9999999 [0.0 - 9999999], [00:00:00 - 23:59:59]

		// float depth_set;						// Irrigation depth set until completion; Range [0.0 - 9999999] 
		// float nominal_depth_left;			// Nominal irrigation depth left until completion; Range [0.0 - 9999999]
		// float measured_depth_left;			// Measured irrigation depth left until completion; Range [0.0 - 9999999]
		
		// float quantity_set;					// Irrigation quantity set by the user; Range [0.0 - 9999999]
		// float nominal_quantity_left;			// Nominal irrigation quantity left until completion; Range [0.0 - 9999999]
		// float measured_quantity_left;		// Measured irrigation quantity left until completion; Range [0.0 - 9999999]
		
		long duration_set;				// Irrigation duration set by the user; Range [00:00:00 - 23:59:59]
		long duration_left;			// Irrigation duration left by the user; Range [00:00:00 - 23:59:59]
		// long nominal_duration_left;	// Nominal irrigation duration left until completion; Range [00:00:00 - 23:59:59]
		// long measured_duration_left;// Measured irrigation duration left until completion; Range [00:00:00 - 23:59:59]
		
		// UNION IIrrigationSetLeft { int depth; float quantity; Time duration; }
		// UNION UDosingSetLeft { float quantity; Time duration; }
		// INTERFACE IFertigationRecipe { DosingMethodType irrigation_method; UDosingSetLeft set; UDosingSetLeft left; float ec; float ph; }
		
		// Allocate memory (in the global space) for the following variables:
		DBTask task_id;						// Task id; Range [? - ?]
		DBIndex program_id;					// Irrigation Program id; Range [0 - 49]
		DBIndex sequence_id;				// Sequence id; Range [0 - 9]
		DBIndex dosing_station_id;			// Id of the Dosing Station to which the Valve Sequence is phisically connected' Range [0 - 9]
		MessageBox message_box;				// Message Box object
		
		// DB_LISTENER_MESSAGE_T message;	// Message
		// byte message_box_id;				// Id of the message box; Range [0 - 255]

		// The following properties are common to all types of Irrigation Programs:
		long cycle_delay;
		IrrigationMethod irrigation_method;
		IrrigationSequenceState state;

		//  The following properties are specific to an Irrigation Program type:

		// The following property is common to Cycles and ET Irrigation Program:
		byte number_cycles_set;				// Number of cycles to run between the dates and within the time window of opportunity
		byte number_cycles_left;			// Number of cycles left to run between the dates and within the time window of opportunity

		// The following property is common to Sensor (VPD, RadSum, Conditions) and ET Irrigation Program:
		DBIndex sensor_id;					// specific_properties.sensor.sensor_id;				// Sensor id; Range [??? - ???]
		
		// The following properties are common to Sensor (VPD, RadSum, Conditions) Irrigation Program:
		long max_rest_time;		// specific_properties.sensor.max_rest;					// Maximum resting time between the irrigation cycles (hh:mm); Range [00:00 - 23:59]
		SensorType sensor_type;				// specific_properties.sensor.sensor_type;				// Sensor type [SENSOR_TYPE_VPD_SUM, SENSOR_TYPE_RAD_SUM, SENSOR_TYPE_CONDITIONS]
		float set_point;					// specific_properties.sensor.condition.set_point;		// Set point
		DBIndex condition_id;				// specific_properties.sensor.condition.condition_id;	// Condition id

		// The following property is specific to ET Irrigation Program:					
		// DBIndex sensor_id;				// specific_properties.et.sensor_id;					// Sensor id; Range [??? - ???]
		int crop_factor;					// specific_properties.et.crop_factor;					// Crop factor
		// byte number_of_cycles;			// specific_properties.et.number_of_cycles;				// Number of cycles

		// Needed ONLY for Loop Irrigation Program
		DBDatePP program_start_date;		// Start date of the Irrigation Program
		DBDatePP program_end_date;			// End date of the Irrigation Program
		long program_start_time;			// Start time of the Irrigation Program
		long program_end_time;				// End time of the Irrigation Program
		
		long sequence_start_time;			// Start time of the Irrigation Sequence
		long sequence_end_time;				// End time of the Irrigation Sequence
		long sequence_time_stamp;			// Timestamp of the previous state change / operation

		long cycle_start_time;				// Start time of the Irrigation Cycle
		long cycle_end_time;				// End time of the Irrigation Cycle
		long cycle_time_stamp;				// Timestamp of the previous state change / operation

		long integral_calculation_time_stamp;	// Timestamp of the last integral calculation; needed for RAD Sum Irrigation method

		ValveGroup valve_group;				// Serves as a dynamicaly expanding and shrinking container to store Active Valve Group
		int number_of_groups;				// Number of Valve Groups in the Valve Sequence
		int group_number;					// Keeps track of the Valve Group number in the Sequence
		
		WORD error_code;

		IrrigationValveGroup Trinity[MAX_VALVES_PER_SEQUENCE];
};

#endif