
// Main MainLine class interface file

#ifndef MAIN_LINE_H
#define MAIN_LINE_H

#include "DB.h"
#include "DBBase.h"
#include "DBMainLine.h"
#include "NMC_MML_Platform.h"

class MainLine : private DBMainLine
{
	public:

		// Default constructor
		explicit MainLine( );   
		// Constructor
		explicit MainLine( DBTask task_id, DBIndex line_id );   
		// explicit MainLine( const MainLine & line );
		~MainLine( );
		// const MainLine & operator=( const MainLine & line );

		/////////////////////////////////////////////////////////////
		// Accessor functions
		/////////////////////////////////////////////////////////////
		DBTask getTaskId( ) const;
		DBIndex getLineId( ) const;
		DBIndex getMainValveId( ) const;
		DBIndex getDosingStationId( ) const;
		DBIndex getWaterMeterId( ) const;
		DBTime getWaterFillUpTime( ) const;
		byte getWaterBeforeAndAfter( ) const;
		byte getFlowAlarm( ) const;
		byte getLowPressureAlarm( ) const;
		IMainLineAlarmSettings getLineAlarmSettings( ) const;
		bool getState( ) const;
		// const MainLine & getLine( ) const;
		// bool operator==( const MainLine & line ) const;

		/////////////////////////////////////////////////////////////
		// Mutator functions
		////////////////////////////////////////////////////////////
		void setTaskId( DBTask task_id );
		void setLineId( DBIndex line_id );
		void setMainValveId( DBIndex main_valve_id );
		void setDosingStationId( DBIndex dosing_station_id );
		void setWaterMeterId( DBIndex water_meter_id );
		void setWaterFillUpTime( DBTime water_fill_up_time );
		void setWaterBeforeAndAfter( byte water_before_and_after );
		void setFlowAlarm( byte flow_alarm );
		void setLowPressureAlarm( byte low_pressure_alarm );
		void setLineAlarmSettings( IMainLineAlarmSettings line_alarm_settings );
		void setState( bool state );
		// void setLine( const MainLine & line );

	private:

		// Allocate memory on stack for the following variables:
		DBTask task_id;						// Task id
		DBIndex line_id;					// Main Line id; Range [0 - 9]

		// THE FOLLOWING PROPERTIES ARE DECLARED/INHERITED IN/FROM THE DBMainLine STRUCTURE
		// DBIndex main_valve_id;			// Id of the Main Valve which opens Main Line
		// DBIndex water_meter_id;			// Id of the Water Meter if present; 0 otherwise
		// DBTime water_fill_up_time;		// Hydraulic system water fill up time (hh:mm)
		// byte water_before_and_after;		// Water before and after the dosing (%); Range [0 - 100], Default 5; Percentage refers to the Irrigation program Depth/Quantity/Time
		// byte flow_alarm;					// Flow alarm (%); Range [0 - 100], Default 25; Percentage refers to the threshould at which the the system raises the alarm when the flow is either below or above the specified threshold
		// float low_pressure_alarm;					// Low pressure alarm; 0.x - 10 Bar or 0.x - 145 psi. Range [0.0 - ???], Defaults: 2.5 and 36.3, respectively.	
		// IMainLineAlarmSettings line_alarm_settings;	// { DBTime alarm_delay; MainLineAlarmAction alarm_action {Continue, Pause}; }
		// bool state: DEFAULT = 0;						// Main line state (On/Off)

		WORD error_code;
};

#endif