
// Main Valve class interface file

#ifndef MAIN_VALVE_H
#define MAIN_VALVE_H

#include "DB.h"
#include "DBBase.h"
#include "string.h"
#include "DBDevices.h"
#include "DBCommon.h"
#include "NMC_MML_Platform.h"
#include "IrrigationIncludes.h"

class MainValve : private DBMainValve 
{
	public:

		// Default constructor
		explicit MainValve( );  

		// Constructor
		// Input: DBTask that specifies the task id and DBIndex that specifies the Main Valve id
		// Purpose: To create a MainValve object 
		explicit MainValve( DBTask task_id, DBIndex valve_id );                

		// Copy constructor
		// Input: MainValve object
		// Purpose: To create a MainValve object and initialize it with another MainValve object
		// explicit MainValve( const MainValve & valve );

		// Overloaded assignment operator
		// Input: MainValve object
		// Purpose: To initialize existing MainValve object with another MainValve object
		// const MainValve & operator=( const MainValve & valve );

		// Destructor
		~MainValve( );

		/////////////////////////////////////////////////////////////
		// Accessor functions
		/////////////////////////////////////////////////////////////
		DBTask getTaskId( ) const;
		DBIndex getValveId( ) const;
		byte getRelayId( ) const;
		IO_LOCATION getRelayLocation( ) const;
		DBIndex getLineId( ) const;
		// DBIndex getWaterMeterId( ) const;
		// int getValveGroupCounter( ) const;
		bool getState( ) const;
		// const MainValve & getValve( ) const;
		// bool operator==( const MainValve & valve ) const;
		bool operator<( const MainValve & valve ) const { return valve_id < valve.getValveId( ); }

		/////////////////////////////////////////////////////////////
		// Mutator functions
		////////////////////////////////////////////////////////////
		void setTaskId( DBTask task_id );
		void setValveId( DBIndex valve_id );
		void setRelayId( byte relay_id );
		void setRelayLocation( IO_LOCATION relay_location );
		void setLineId( DBIndex line_id );
		// void setWaterMeterId( DBIndex water_meter_id );
		// void setValveGroupCounter( int valve_group_counter );
		void setState( bool state );
		// void setValve( const MainValve & valve );

		// Initializes Main Valve object
		bool Initialize( DBTask task_id, DBIndex valve_id );

		bool openValve( );
		bool closeValve( );

	private:

		DBTask task_id;						// Task id
		DBIndex valve_id;					// Main Valve id; Range [1 - 10]
		// int valve_group_counter;			// Keeps track of how many Valve Groups use this Main Valve simulteneously (should NOT be allowed to happen)

		// THE FOLLOWING PROPERTIES ARE DECLARED/INHERITED IN/FROM THE DBMainValve STRUCTURE
		// byte relay_id;					// Id of the Digital Output relay that activates the Main Valve; Range [0 - 255]
		// IO_LOCATION relay_location;		// Relay location where the relay is physically installed [RELAY_LOCATION_NONE, RELAY_LOCATION_LOCAL, RELAY_LOCATION_EXPANSION_BOX_1, RELAY_LOCATION_EXPANSION_BOX_2, RELAY_LOCATION_EXPANSION_BOX_3, RELAY_LOCATION_REMOTE]
		// DBIndex line_id;					// Id of the Main Line on which the Main Valve is "sitting"; Range [1 - 10]
		// DBIndex water_meter_id;          // Id of the Water Meter if present; 0 otherwise
		// bool state;						// Main Valve state; Range (On/Off)

		WORD error_code;					// Serves as a "bucket" to store Database error code
};

#endif


