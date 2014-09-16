
// WaterSource class implementation file

#include "WaterSource.h"

//************************************************************************
// @@ Function Name	: WaterSource Constructor
// @ Description	: Initialization and creation of a WaterSource object from DB
// @ Input			: TaskID - Relevant control thread (described at tasks.h) <-------- Defined at tasks.h as - WATER_SOURCE_SLOT
//					  WaterSourceID - Index of DB record to be retrieved for the WaterSource object  
// @ Output			:
// @ Notes			: 
//************************************************************************
WaterSource::WaterSource(DBTask TaskID, DBIndex WaterSourceID)
{
	m_TaskID = TaskID;
	m_WaterSourceID = WaterSourceID;

	// Check if the record exists in the DB 
    if (DB_Exist(DBWATERSOURCE_TYPE, TaskID, WaterSourceID))	
    {
		// Allocate memory (on stack) for WaterSource structure
		DBWaterSource tmpWaterSource;

		// Retrieve the record from the database and store it in the tmpWaterSource structure
		if (DB_GetRecord(DBWATERSOURCE_TYPE, TaskID, WaterSourceID, &tmpWaterSource))
		{
			// Initialize class members with retrieved data
			memcpy(m_PumpStationID, tmpWaterSource.PumpStationID, sizeof(DBIndex * MAX_PUMP_STATIONS));
			m_PressureMeter				= tmpWaterSource.PressureMeter;
			m_WaterTemperature			= tmpWaterSource.WaterTemperature;
			m_RequiredWaterTemperature	= tmpWaterSource.RequiredWaterTemperature;
			m_WaterLevel				= tmpWaterSource.WaterLevel;

			return;
		}
	}

	// In case of error - set defaults
	memset (m_PumpStationID, 0, sizeof(DBIndex * MAX_PUMP_STATIONS));
	m_PressureMeter				= 0;
	m_WaterTemperature			= 0;
	m_RequiredWaterTemperature	= 0;
	m_WaterLevel				= 0;
}

//************************************************************************
// @@ Function Name	: WaterSource Copy Constructor
// @ Description	: Copy constructor
// @ Input			: waterSource - Other WaterSource object   
// @ Output			:
// @ Notes			: 
//************************************************************************
WaterSource::WaterSource(const WaterSource& waterSource)
{
	// Check for self-assignment!
	if ( this != &waterSource )    
	{
		SetValues(waterSource);
	}
}

//************************************************************************
// @@ Function Name	: WaterSource Overloaded Operator =
// @ Description	: Overloaded operator =
// @ Input			: waterSource - Other WaterSource object   
// @ Output			:
// @ Notes			: 
//************************************************************************
const WaterSource& WaterSource::operator=(const WaterSource& waterSource)
{
	// Check for self-assignment!
	if (this != &waterSource)    
	{
		SetValues(waterSource);
	}

	return *this;
}

//************************************************************************
// @@ Function Name	: WaterSource Destructor
// @ Description	: Destructor
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
WaterSource::~WaterSource( );


/////////////////////////////////////////////////////////////
// Accessor Functions
/////////////////////////////////////////////////////////////

//************************************************************************
// @@ Function Name	: GetTaskID
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
DBTask WaterSource::GetTaskID( )
{
	return m_TaskID;
}

//************************************************************************
// @@ Function Name	: GetWaterSourceID
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
DBIndex WaterSource::GetWaterSourceID( )
{
	return m_WaterSourceID;
}

//************************************************************************
// @@ Function Name	: GetPumpStationID
// @ Description	: 
// @ Input			:   
// @ Output			: An array of all connected pumps to this water source
// @ Notes			: 
//************************************************************************
const DBIndex[] WaterSource::GetPumpStationID( )
{
	return m_PumpStationID;
}

//************************************************************************
// @@ Function Name	: GetPressureMeter
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
byte WaterSource::GetPressureMeter()
{
	return m_PressureMeter;
}

//************************************************************************
// @@ Function Name	: GetWaterTemperature
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
byte WaterSource::GetWaterTemperature()
{
	return m_WaterTemperature;
}

//************************************************************************
// @@ Function Name	: GetRequiredWaterTemperature
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
byte WaterSource::GetRequiredWaterTemperature()
{
	return m_RequiredWaterTemperature;
}

//************************************************************************
// @@ Function Name	: GetWaterLevel
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
WATER_SOURCE_LEVEL WaterSource::GetWaterLevel()
{
	return m_WaterLevel;
}

/////////////////////////////////////////////////////////////
// Mutator functions
////////////////////////////////////////////////////////////

//************************************************************************
// @@ Function Name	: SetTaskID
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void WaterSource::SetTaskID(DBTask TaskID)
{
	m_TaskID = TaskID;
}

//************************************************************************
// @@ Function Name	: SetWaterSourceID
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void WaterSource::SetWaterSourceID(DBIndex WaterSourceID)
{
	m_WaterSourceID = WaterSourceID;
}

//************************************************************************
// @@ Function Name	: SetPumpStationID
// @ Description	: 
// @ Input			: An array of all connected pumps to this water source
// @ Output			:
// @ Notes			: 
//************************************************************************
void WaterSource::SetPumpStationID(DBIndex[] PumpStationID)
{
	memcpy(m_PumpStationID, PumpStationID, sizeof(DBIndex * MAX_PUMP_STATIONS));
}

//************************************************************************
// @@ Function Name	: SetPressureMeter
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void WaterSource::SetPressureMeter(byte PressureMeter)
{
	m_PressureMeter = PressureMeter;
}

//************************************************************************
// @@ Function Name	: SetWaterTemperature
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void WaterSource::SetWaterTemperature(byte WaterTemperature)
{
	m_WaterTemperature = WaterTemperature;
}

//************************************************************************
// @@ Function Name	: SetRequiredWaterTemperature
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void WaterSource::SetRequiredWaterTemperature(byte RequiredWaterTemperature)
{
	m_RequiredWaterTemperature = RequiredWaterTemperature;
}

//************************************************************************
// @@ Function Name	: SetWaterLevel
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void WaterSource::SetWaterLevel(WATER_SOURCE_LEVEL WaterLevel)
{
	m_WaterLevel = WaterLevel;
}

//************************************************************************
// @@ Function Name	: SetValues
// @ Description	: Copy other object values to current object
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void WaterSource::SetValues(const WaterSource& waterSource)
{
	// Aliasing test
	if ( this != &waterSource )    
	{
		SetTaskID(waterSource.TaskID);
		SetWaterSourceID(waterSource.WaterSourceID);
		SetPumpStationID(waterSource.PumpStationID);
		SetPressureMeter(waterSource.PressureMeter);
		SetWaterTemperature(waterSource.WaterTemperature);
		SetRequiredWaterTemperature(waterSource.RequiredWaterTemperature);
		SetWaterLevel(waterSource.WaterLevel);
	}
}