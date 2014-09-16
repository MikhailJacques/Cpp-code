
// Water Meter class interface file

#ifndef WATER_METER_H
#define WATER_METER_H

#include "DB.h"
#include "DBBase.h"
#include "string.h"
#include "DBCommon.h"
#include "DBDevices.h"
#include "DBDateTime.h"
#include "NMC_MML_Platform.h"
#include "IrrigationIncludes.h"

class WaterMeter : private DBWaterMeter 
{
	public:

		explicit WaterMeter( );  
		explicit WaterMeter( DBTask task_id, DBIndex water_meter_id );                
		// explicit WaterMeter( const WaterMeter & water_meter );
		// const WaterMeter & operator=( const WaterMeter & water_meter );
		~WaterMeter( );

		/////////////////////////////////////////////////////////////
		// Accessor functions
		/////////////////////////////////////////////////////////////
		DBTask getTaskId( ) const;
		DBIndex getWaterMeterId( ) const;
		byte getInputId( ) const;
		IO_LOCATION getRelayLocation( ) const;
		byte getAllocateeType( ) const;
		DBIndex getAllocateeId( ) const;
		DBIndex getMainLineId( ) const;
		float getRatio( ) const;
		byte getIntervalPulses( ) const;
		int getTotalPulses( ) const;
		int retrieveIntervalPulses( ) const;
		long getFirstPulseTimestamp( ) const;					
		long getLastPulseTimestamp( ) const;						
		float getWaterLeakQuantity( ) const;
		DBTime getWaterLeakPeriod( ) const;
		bool getState( ) const;
		// const WaterMeter & getWaterMeter( ) const;

		bool operator<( const WaterMeter & WaterMeter ) const;
		// bool operator==( const WaterMeter & WaterMeter ) const;

		int getMissedPulsesCounter( ) const;
		float getNominalFlowRateLitersPerSecond( ) const;
		float getMeasuredFlowRateLitersPerSecond( ) const;
		// long getElapsedTimeSinceLastPulse( ) const;
		long getNominalTimeIntervalBetweenPulses( ) const;
		long getMeasuredTimeIntervalBetweenPulses( ) const;
		long getMaxAllowedTimeIntervalBetweenPulses( ) const;
		

		/////////////////////////////////////////////////////////////
		// Mutator functions
		////////////////////////////////////////////////////////////
		void setTaskId( DBTask );
		void setWaterMeterId( DBIndex water_meter_id );
		void setInputId( byte input_id );
		void setRelayLocation( IO_LOCATION input_location );
		void setAllocateeType( byte allocatee_type );
		void setAllocateeId( DBIndex allocatee_id );
		void setMainLineId( DBIndex main_line_id );
		void setRatio( float ratio );
		void setIntervalPulses( byte interval_pulses );
		void addIntervalPulses( int interval_pulses );
		void setTotalPulses( int total_pulses );
		void setFirstPulseTimestamp( long first_pulse_timestamp );					
		void setLastPulseTimestamp( long last_pulse_timestamp );
		void setWaterLeakQuantity( float water_leak_quantity );
		void setWaterLeakPeriod( DBTime water_leak_time );
		void setState( bool state );
		void setWaterMeter( const WaterMeter & water_meter );

		void setMissedPulsesCounter( int missed_pulses_counter );
		void setNominalFlowRateLitersPerSecond( float flow_rate );
		void setMeasuredFlowRateLitersPerSecond( float flow_rate );
		// void setElapsedTimeSinceLastPulse( long milliseconds );
		void setNominalTimeIntervalBetweenPulses( long time );
		void setMeasuredTimeIntervalBetweenPulses( long time );
		void setMaxAllowedTimeIntervalBetweenPulses( long time );
		void setNominalAndMaximumAllowedTimeIntervalsBetweenPulses( );
		
		void updateMeasurements( );

	private:

		DBTask task_id;									// Task id
		DBIndex water_meter_id;							// Main WaterMeter id; Range [1 - 10]
		
		// THE FOLLOWING PROPERTIES ARE DECLARED/INHERITED IN/FROM THE DBWaterMeter STRUCTURE:

		// byte input_id;								// Id of the Digital Input that receives the Device state; Range [0 - 255]
		// IO_LOCATION input_location;					// Input location where the input is physically installed [input_LOCATION_NONE, input_LOCATION_LOCAL, input_LOCATION_EXPANSION_BOX_1, input_LOCATION_EXPANSION_BOX_2, input_LOCATION_EXPANSION_BOX_3, input_LOCATION_REMOTE]
		
		// byte allocatee_type;							// System component (allocatee) type on which the Water Meter is physically installed
														// [Water Source, Pump Station, Filter Station, Dosing Station, Main Line, Irrigation Valve]
		// DBIndex allocatee_id;						// DBIndex - System component (allocatee) Id on which the Water Meter is physically installed
 		//												// Note: Water Meter Allocation is comprised of the table type and type id, e.g. Line 1, Valve 27, etc.
		// DBIndex main_line_id;						// Main Line Id on which the Water Meter is connected either before (e.g. Water Source,
														// Pump Station, Filter Station, Dosing Station) on (e.g. Main Line) or after (e.g. Irrigation Valve)
		// float ratio;									// Water Meter ratio (Liters/Pulse); Range [0 - 9999.99]
		// byte interval_pulses;						// Number of pulses accumulated since the last read
		// int total_pulses: DEFAULT = 0;				// Number of pulses accumulated since the last reset in the UI Screen 4.4
		// long first_pulse_timestamp;					// Timestamp (in milliseconds) when the first pulse was input
		// long last_pulse_timestamp;					// Timestamp (in milliseconds) when the last pulse was input
		// float water_leak_quantity; 					// Water leak sets the boundary on the amount of leaked water quantity; Range [0 - 99.99]
		//												// how much water flowed through the water meter when it should NOT flow at all
		// DBTime water_leak_period;					// Time period during which the water leak is measured
		// bool state;									// Water Meter state; Range (On/Off) 

		// The following variables ONLY apply to the Water Meter once it has been installed and the nominal flow through it, 
		// i.e. from its physical location and down the system hierarchy, is known:
		int missed_pulses_counter;						// Counts missed pulses - pulses that did not arrive while they were expected	
		float nominal_flow_rate_liters_per_second;
		float measured_flow_rate_liters_per_second;				
		// long elapsed_time_since_last_pulse;			// Elapsed time since the acquisition of the last pulse
		long nominal_time_interval_between_pulses;		// Nominal time interval (milliseconds) between Water Meter pulses
		long measured_time_interval_between_pulses;		// Measured time interval (milliseconds) between Water Meter pulses							
		long max_allowed_time_interval_between_pulses;	// Maximum allowed time interval (milliseconds) between Water Meter pulses before raising an alarm
		
		
		WORD error_code;								// Serves as a "bucket" to store Database error code
};

#endif


