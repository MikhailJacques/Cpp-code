
// Irrigation Valve class interface file

#ifndef IRRIGATION_VALVE_H
#define IRRIGATION_VALVE_H

#include "DB.h"
#include "DBBase.h"
#include "string.h"
#include "DBDateTime.h"
#include "DBDevices.h"
#include "DBDateTime.h"
#include "NMC_MML_Platform.h"
#include "IrrigationIncludes.h"

class IrrigationValve : protected DBIrrigationValve
{
	public:

		IrrigationValve( );
		explicit IrrigationValve( DBTask task_id, DBIndex valve_id );
		// IrrigationValve( const IrrigationValve & other );
		~IrrigationValve( );
		// IrrigationValve & operator=( const IrrigationValve & other );

		/////////////////////////////////////////////////////////////
		// Accessor functions
		/////////////////////////////////////////////////////////////
		DBTask getTaskId( ) const;
		DBIndex getValveId( ) const;
		byte getRelayId( ) const;
		IO_LOCATION getRelayLocation( ) const;
		DBIndex getLineId( ) const;
		DBIndex getWaterMeterId( ) const;
		IrrigationAreaUnits getIrrigationAreaUnits( ) const;
		float getIrrigationArea( ) const;
		float getNominalFlowRateCubicMetersPerHour( ) const;
		float getNominalFlowRateLitersPerSecond( ) const;
		DBTimePP getOnDelay( ) const;
		DBTimePP getOffDelay( ) const;
		// DBTimePP getStaggerDelay( ) const;
		// DBTimePP getOverlapTime( ) const;
		// DBTimePP getOpenDelay( ) const;
		// DBTimePP getCloseDelay( ) const;
		DeviceState getDeviceState( ) const;
		int getNumberCyclesElapsed( ) const;
		bool getState( ) const;
		// const IrrigationValve & getValve( ) const;
		// bool operator==( const IrrigationValve & valve ) const;
		bool operator<( const IrrigationValve & valve ) const { return valve_id < valve.getValveId( ); }

		/////////////////////////////////////////////////////////////
		// Mutator functions
		////////////////////////////////////////////////////////////
		void setTaskId( DBTask task_id );
		void setValveId( DBIndex valve_id );
		void setRelayId( byte relay_id );
		void setRelayLocation( IO_LOCATION relay_location );
		void setLineId( DBIndex line_id );
		void setWaterMeterId( DBIndex water_meter_id );
		void setIrrigationAreaUnits( IrrigationAreaUnits irrigation_area_units );
		void setIrrigationArea( float irrigation_area );
		void setNominalFlowRateCubicMetersPerHour( float nominal_flow );
		void setOnDelay( DBTimePP delay );
		void setOffDelay( DBTimePP delay );
		// void setStaggerDelay( DBTimePP stagger_delay );
		// void setOverlapTime( DBTimePP ovelap_time );
		// void setOpenDelay( DBTimePP open_delay );
		// void setCloseDelay( DBTimePP close_delay );
		void setDeviceState( DeviceState device_state );
		void setNumberCyclesElapsed( int number_cycles_elapsed );
		void incrementNumberCyclesElapsed( );
		void setState( bool state );
		// void setValve( const IrrigationValve & valve );

		// Initializes Irrigation Valve object
		bool Initialize( DBTask task_id, DBIndex valve_id );

		bool openValve( );
		bool closeValve( );

	private:

		DBTask task_id;						// Task id
		DBIndex valve_id;					// Irrigation Valve id; Range [1 - 256]

		DeviceState device_state;			// The Irrigation Valve device state [DEVICE_STATE_NONE, DEVICE_STATE_WAIT_ON, DEVICE_STATE_ON, DEVICE_STATE_WAIT_OFF, DEVICE_STATE_OFF]

		// DBDatePP start_date;
		DBTimePP start_time;
		IrrigationFinishTrigger trigger;
		int number_cycles_elapsed;

		long on_delay;						// The time delay between receiving the command to activate (open) and actually activating (opening) this irrigation valve (milliseconds)
		long off_delay;						// The time between receiving the command to de-activate (close) and actually de-activating (closing) this irrigation valve (milliseconds)
		long timestamp;
		float nominal_flow_rate_liters_per_second;	// Nominal flow rate of the irrigation valve (liters/second)

		WORD error_code;
		// DBTimePP on_delay;				// The time between receiving the command to activate (open) and actually activating (opening) this irrigation valve (mm:ss); Range [00:00 - 59:59]
		// DBTimePP off_delay;				// The time between receiving the command to de-activate (close) and actually de-activating (closing) this irrigation valve (mm:ss); Range [00:00 - 59:59]
		// DBTimePP overlap_time;			// The time between opening this irrigation valve and closing the previous irrigation valve (mm:ss); Range [00:00 - 59:59]
		// DBTimePP stagger_delay;			// The time between receiving the command to de-activate (close) and actually de-activating (closing) this irrigation valve (mm:ss); Range [00:00 - 59:59]
											// Applies ONLY to the first and last valve groups.
		// DBTimePP open_delay;				// System-calculated valve open delay in seconds
		// DBTimePP close_delay;			// System-calculated valve close delay in seconds
		// DBTimePP timestamp;
		// long current_time_in_seconds;	// Current time in seconds
		// long open_delay_in_seconds;		// User-defined valve open delay in seconds
		// long close_delay_in_seconds;		// User-defined valve close delay in seconds
		// long overlap_time_in_seconds;	// User-defined valve overlap time in seconds
		// long stagger_delay_in_seconds;	// User-defined valve stagger delay in seconds

		// THE FOLLOWING PROPERTIES ARE DECLARED/INHERITED IN/FROM THE DBIrrigationValve STRUCTURE
		// byte relay_id;							// Id of the Digital Output relay that activates the Irrigation Valve; Range [0 - 255]
		// IO_LOCATION relay_location;				// Relay location where the relay is physically installed [RELAY_LOCATION_NONE, RELAY_LOCATION_LOCAL, RELAY_LOCATION_EXPANSION_BOX_1, RELAY_LOCATION_EXPANSION_BOX_2, RELAY_LOCATION_EXPANSION_BOX_3, RELAY_LOCATION_REMOTE]
		// DBIndex line_id;							// Id of the Main Line on which the Irrigation Valve is physically installed; Range [0 - 9]
		// DBIndex water_meter_id;					// Id of the Water Meter if present; 0 otherwise
		// IrrigationAreaUnits irrigation_area_units;// Irrigation area units; Range [HECTARE, ACRE]
		// float irrigation_area;					// Irrigated area covered by the irrigation valve (m2); Range [0 - 999.999] Note: 1 Hectare = 2.47105381 Acres
		// float nominal_flow;						// Nominal flow of the irrigation valve (m3/hour, Gallon/minute); Range [0 - 99999] Note: 1 cubic meter per hour = 4.40286754 gallons per minute.
		// DBTime on_delay;							// The time between receiving the command to activate (open) and actually activating (opening) this irrigation valve (mm:ss); Range [00:00 - 59:59]
		// DBTime off_delay;						// The time between receiving the command to de-activate (close) and actually de-activating (closing) this irrigation valve (mm:ss); Range [00:00 - 59:59]
		// DEPRECATED: DBTime overlap_time;			// The time period between opening this irrigation valve and closing the previous irrigation valve (mm:ss); Range [00:00 - 59:59]
		// DEPRECATED: DBTime stagger_delay;		// The time period between activating (opening/closing) this irrigation valve and activating (opening/closing) the next irrigation valve (mm:ss); Range [00:00 - 59:59]
		// bool state;								// Irrigation Valve state; Range [true = Open, false = Closed]

		// The nominal flow rate may be measured in liters per minute (LPM) or in gallons per minute (GPM)
		// HECTARE -> m3/hour or 16.7 liters/minute or 0.2777 liters/second
		// ACRE -> gallon/min or 0.0631 liters/second
		// float nominal_flow_rate_gallons_per_second;	// Nominal flow rate of the irrigation valve (gallons/second)		
};

#endif