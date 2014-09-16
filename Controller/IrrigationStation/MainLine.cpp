
// Main Line class implementation file

#include "MainLine.h"

// Default constructor
MainLine::MainLine( ) { }

// Constructor
MainLine::MainLine( DBTask task_id, DBIndex line_id ) : task_id( task_id ), line_id( line_id )
{
	// Get the memory slot for temporarily storing the Main Line record
	DBMainLine * db_line = (DBMainLine *) DB_GetMemorySlot(task_id);

	// Retrieve record from the database
	DB_GetRecord( DBMAINLINE_TYPE, task_id, line_id, db_line );

	// Set Main Line object properties
	setMainValveId( db_line->main_valve_id );
	setDosingStationId( db_line->dosing_station_id );
	setWaterMeterId( db_line->water_meter_id );
	setWaterFillUpTime( db_line->water_fill_up_time );
	setWaterBeforeAndAfter( db_line->water_before_and_after );
	setFlowAlarm( db_line->flow_alarm );
	setLowPressureAlarm( db_line->low_pressure_alarm );
	setLineAlarmSettings( db_line->line_alarm_settings );
	setState( false );
	}

// Destructor
MainLine::~MainLine( ) { }

/////////////////////////////////////////////////////////////
// Accessor functions
/////////////////////////////////////////////////////////////
DBTask MainLine::getTaskId( ) const { return task_id; }
DBIndex MainLine::getLineId( ) const { return line_id; }
DBIndex MainLine::getMainValveId( ) const { return main_valve_id; }
DBIndex MainLine::getDosingStationId( ) const { return dosing_station_id; }
DBIndex MainLine::getWaterMeterId( ) const { return water_meter_id; }
DBTime MainLine::getWaterFillUpTime( ) const { return water_fill_up_time; }
byte MainLine::getWaterBeforeAndAfter( ) const { return water_before_and_after; }
byte MainLine::getFlowAlarm( ) const { return flow_alarm; }
byte MainLine::getLowPressureAlarm( ) const { return low_pressure_alarm; }

IMainLineAlarmSettings MainLine::getLineAlarmSettings( ) const
{
	// TO DO: Ensure that it works properly using debugger
	return line_alarm_settings;
}

bool MainLine::getState( ) const { return state; }

/////////////////////////////////////////////////////////////
// Mutator functions
////////////////////////////////////////////////////////////
void MainLine::setTaskId( DBTask id ) { task_id = id; }
void MainLine::setLineId( DBIndex id ) { line_id = id;}
void MainLine::setMainValveId( DBIndex id ) { main_valve_id = id; }
void MainLine::setDosingStationId( DBIndex id ) { dosing_station_id = id; }
void MainLine::setWaterMeterId( DBIndex id ) { water_meter_id = id; }
void MainLine::setWaterFillUpTime( DBTime time ) { water_fill_up_time = time; }
void MainLine::setWaterBeforeAndAfter( byte water ) { water_before_and_after = water; }
void MainLine::setFlowAlarm( byte alarm ) { flow_alarm = alarm; }
void MainLine::setLowPressureAlarm( byte alarm ) { low_pressure_alarm = alarm; }
void MainLine::setLineAlarmSettings( IMainLineAlarmSettings settings )
{
	// TO DO: Ensure that it works properly using debugger
	line_alarm_settings = settings;
}

void MainLine::setState( bool state ) { this->state = state; }

//void MainLine::setLine( const MainLine & line )
//{
//	// Perform aliasing test
//	if ( this != &line )    
//	{
//		setTaskId( line.task_id );
//		setLineId( line.line_id );
//		setWaterMeterId( line.water_meter_id );
//		setWaterFillUpTime( line.water_fill_up_time );
//		setWaterBeforeAndAfter( line.water_before_and_after );
//		setFlowAlarm( line.flow_alarm );
//		setLowPressureAlarm( line.low_pressure_alarm );
//		setLineAlarmSettings( line.line_alarm_settings );
//	}
//}